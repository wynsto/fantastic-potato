#include <potato/greeter.h>
#include <potato/version.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/asio.hpp>

#include <sqlite3.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <ctime>
#include <fmt/format.h>


#include <chrono>
#include <cstdlib>
#include <memory>
#include <map>
#include <sstream>

using boost::asio::ip::tcp;
using namespace boost::beast::detail::base64;
using namespace std;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  std::string result = ctime(&now);
  result += "hello";
  return result;
}

std::map<std::string, std::string> parse_query_string(const std::string& query) {
  std::map<std::string, std::string> result;
  std::istringstream ss(query);
  std::string item;
  while (std::getline(ss, item, '&')) {
    std::size_t pos = item.find('=');
    if (pos != std::string::npos) {
      std::string key = item.substr(0, pos);
      std::string value = item.substr(pos + 1);
      result[key] = value;
    }
  }
  return result;
}

namespace my_program_state
{
    std::size_t
    request_count()
    {
        static std::size_t count = 0;
        return ++count;
    }

    std::time_t
    now()
    {
        return std::time(0);
    }
}

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    http_connection(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    // Initiate the asynchronous operations associated with the connection.
    void
    start()
    {
        read_request();
        check_deadline();
    }

private:
    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    beast::flat_buffer buffer_{8192};

    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    net::steady_timer deadline_{
        socket_.get_executor(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
    void
    read_request()
    {
        auto self = shared_from_this();

        http::async_read(
            socket_,
            buffer_,
            request_,
            [self](beast::error_code ec,
                std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);
                if(!ec)
                    self->process_request();
            });
    }

    // Determine what needs to be done with the request message.
    void
    process_request()
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch(request_.method())
        {
        case http::verb::get:
            response_.result(http::status::ok);
            response_.set(http::field::server, "Beast");
            create_response();
            break;

        default:
            // We return responses indicating an error if
            // we do not recognize the request method.
            response_.result(http::status::bad_request);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body())
                << "Invalid request-method '"
                << std::string(request_.method_string())
                << "'";
            break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void
    create_response()
    {
        if(request_.target().starts_with("/callback")) {
          boost::beast::string_view queryStrView = request_.target();
          queryStrView.remove_prefix(10);
          string s = {queryStrView.begin(), queryStrView.end()};
          string code = "";
          if(!s.empty()) {
            std::map<std::string, std::string> query = parse_query_string(s);
            cout << query["code"] << endl;
            code = query["code"];
          }
    
          
          string callbackUrl = getenv("SCHWAB_CALLBACK_URL");
          string baseUrl = "https://api.schwabapi.com/v1";

          string postData = fmt::format("grant_type=authorization_code&code={}&redirect_uri={}", code, callbackUrl);
        
          string tokenUrl = baseUrl + "/oauth/token";
          fantastic_potato::Potato potato("potato");
          string resp = potato.post(tokenUrl, postData);
          response_.result(http::status::ok);
          response_.set(http::field::content_type, "application/json");
          beast::ostream(response_.body()) << resp;
        }
        else
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body()) << "File not found\r\n";
        }
    }

    // Asynchronously transmit the response message.
    void
    write_response()
    {
        auto self = shared_from_this();

        response_.content_length(response_.body().size());

        http::async_write(
            socket_,
            response_,
            [self](beast::error_code ec, std::size_t)
            {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->deadline_.cancel();
            });
    }

    // Check whether we have spent enough time on this connection.
    void
    check_deadline()
    {
        auto self = shared_from_this();

        deadline_.async_wait(
            [self](beast::error_code ec)
            {
                if(!ec)
                {
                    // Close socket to cancel any outstanding operation.
                    self->socket_.close(ec);
                }
            });
    }
};

// "Loop" forever accepting new connections.
void
http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
  acceptor.async_accept(socket,
      [&](beast::error_code ec)
      {
          if(!ec)
              std::make_shared<http_connection>(std::move(socket))->start();
          http_server(acceptor, socket);
      });
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i< argc; i++) {
    cout << azColName[i] << argv[i] <<endl;
  }
  return 0;
}

auto main(int argc, char** argv) -> int {
  string baseUrl = "https://api.schwabapi.com/v1";
  string appKey = std::getenv("SCHWAB_APP_KEY");
  string appSecret = std::getenv("SCHWAB_APP_SECRET");
  string callbackUrl = std::getenv("SCHWAB_CALLBACK_URL");
  string oauth = "oauth/authorize";
  string url = fmt::format("{}/oauth/authorize?client_id={}&redirect_uri={}", baseUrl, appKey, callbackUrl);

  cout << url << endl;
  
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  if (rc){
    cout << rc << endl;
    string msg = sqlite3_errmsg(db);
    cout << "Cannot open database: " << msg <<endl;
    sqlite3_close(db);
    return 1;
  }

  rc = sqlite3_exec(db, argv[1], callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  }
  rc = sqlite3_close(db);
  if (rc != SQLITE_OK) {
    cout << "Cannot close database::"<< sqlite3_errmsg(db)  <<endl;
    return 1;
  }
  try 
  {
      auto const address = net::ip::make_address("0.0.0.0");
      unsigned short port = static_cast<unsigned short>(std::atoi("8081"));

      net::io_context ioc{1};

      tcp::acceptor acceptor{ioc, {address, port}};
      tcp::socket socket{ioc};
      http_server(acceptor, socket);

      ioc.run();
  }
  catch(std::exception const& e)
  {
      std::cerr << "Error: " << e.what() << std::endl;
      return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
