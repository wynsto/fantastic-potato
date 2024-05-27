
#pragma once

#include <schwab/greeter.h>
#include <schwab/db.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>
#include <string>
#include <map>
#include <fmt/format.h>
#include <nlohmann/json.hpp>


namespace net = boost::asio;            // from <boost/asio.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using namespace std;
using json = nlohmann::json;

map<string, string> parse_query_string(const string& query) {
  map<string, string> result;
  istringstream ss(query);
  string item;
  while (getline(ss, item, '&')) {
    size_t pos = item.find('=');
    if (pos != string::npos) {
      string key = item.substr(0, pos);
      string value = item.substr(pos + 1);
      result[key] = value;
    }
  }
  return result;
}

class http_connection : public enable_shared_from_this<http_connection> {
public:
  http_connection(tcp::socket socket) : socket_(std::move(socket)) {}

  // Initiate the asynchronous operations associated with the connection.
  void start() {
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
  net::steady_timer deadline_{socket_.get_executor(), chrono::seconds(60)};

  // Asynchronously receive a complete request message.
  void read_request() {
    auto self = shared_from_this();

    http::async_read(socket_, buffer_, request_,
                     [self](beast::error_code ec, size_t bytes_transferred) {
                       boost::ignore_unused(bytes_transferred);
                       if (!ec) self->process_request();
                     });
  }

  // Determine what needs to be done with the request message.
  void process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);

    switch (request_.method()) {
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
            << "Invalid request-method '" << string(request_.method_string()) << "'";
        break;
    }

    write_response();
  }

  // Construct a response message based on the program state.
  void create_response() {
    if (request_.target().starts_with("/callback")) {
      boost::beast::string_view queryStrView = request_.target();
      queryStrView.remove_prefix(10);
      string s = {queryStrView.begin(), queryStrView.end()};
      string code = "";
      if (!s.empty()) {
        map<string, string> query = parse_query_string(s);
        cout << query["code"] << endl;
        code = query["code"];
      }

      schwab_personal_trading::Potato potato("potato");
      string accessToken = potato.getAccessToken("authorization_code", code);
      response_.result(http::status::ok);
      response_.set(http::field::content_type, "application/json");
      cout << accessToken << endl;
      beast::ostream(response_.body()) << accessToken;
    } else {
      response_.result(http::status::not_found);
      response_.set(http::field::content_type, "text/plain");
      beast::ostream(response_.body()) << "File not found\r\n";
    }
  }

  // Asynchronously transmit the response message.
  void write_response() {
    auto self = shared_from_this();

    response_.content_length(response_.body().size());

    http::async_write(socket_, response_, [self](beast::error_code ec, size_t) {
      self->socket_.shutdown(tcp::socket::shutdown_send, ec);
      self->deadline_.cancel();
    });
  }

  // Check whether we have spent enough time on this connection.
  void check_deadline() {
    auto self = shared_from_this();

    deadline_.async_wait([self](beast::error_code ec) {
      if (!ec) {
        // Close socket to cancel any outstanding operation.
        self->socket_.close(ec);
      }
    });
  }
};