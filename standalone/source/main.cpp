#include <schwab/greeter.h>
#include <schwab/db.h>
#include <schwab/version.h>
#include <schwab/http_connection.h>
#include <nlohmann/json.hpp>


#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/asio.hpp>


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

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace boost::beast::detail::base64;
using namespace std;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

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

auto main(int argc, char** argv) -> int {
    time_t timer;
    timer = time(NULL);
    cout << "Time: " << timer << endl;
  schwab_personal_trading::SchwabDB db("test.db");
  // db.query("create table if not exists schwab_kv (key text, value text)");
  string accessToken = db.getValueByType("accessToken");
  clog << "accessToken: " << accessToken << endl;

  string refreshToken = db.getValueByType("refreshToken");
  clog << "refreshToken: " <<refreshToken << endl;

  string baseUrl = "https://api.schwabapi.com/v1";
  string appKey = std::getenv("SCHWAB_APP_KEY");
  string appSecret = std::getenv("SCHWAB_APP_SECRET");
  string callbackUrl = std::getenv("SCHWAB_CALLBACK_URL");
  string oauth = "oauth/authorize";
  if (accessToken != "" && refreshToken != "") {
    schwab_personal_trading::Potato potato("potato");
    accessToken = potato.getAccessToken("refresh_token", refreshToken);
    // string marketData = "https://api.schwabapi.com/marketdata/v1";
    string quotesUrl = "https://api.schwabapi.com/marketdata/v1/quotes?symbols=GBP/USD,USD/HKD,USD/CNH,USD/JPY,EUR/USD,USD/CAD,AUD/USD,USD/SGD,USD/THB";
    potato.setBearerAuth(accessToken);
    string resp = potato.getAfterAuth(quotesUrl);
    cout << resp << endl;
    auto quotes = json::parse(resp);
    float usd = 1.0;
    float gbp = quotes["GBP/USD"]["quote"]["lastPrice"];
    gbp = usd/gbp;
    float hkd = quotes["USD/HKD"]["quote"]["lastPrice"];
    float cnh = quotes["USD/CNH"]["quote"]["lastPrice"];
    float jpy = quotes["USD/JPY"]["quote"]["lastPrice"];
    float eur = quotes["EUR/USD"]["quote"]["lastPrice"];
    eur = usd/eur;
    float cad = quotes["USD/CAD"]["quote"]["lastPrice"];
    float aud = quotes["AUD/USD"]["quote"]["lastPrice"];
    uad = usd/aud;
    float sgd = quotes["USD/SGD"]["quote"]["lastPrice"];
    float thb = quotes["USD/THB"]["quote"]["lastPrice"];

    // create table fx_rate (hkd real, usd real, cny real, jpy real, gbp real, eur real, cad real, aud real, sgd real, thb real, time timestamp, date string);
    string sql = fmt::format("INSERT INTO fx_rate (hkd, usd, cny, jpy, gbp, eur, cad, aud, sgd, thb, time, date) values ({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, date())", hkd, usd, cnh, jpy, gbp, eur, cad, aud, sgd, thb, timer);
    clog << sql << endl;
    schwab_personal_trading::SchwabDB fxRateDb("/home/wynston/dev/flask-project/instance/home.sqlite");

    fxRateDb.query(sql);

    return EXIT_SUCCESS;
  } else {
    string url = fmt::format("{}/oauth/authorize?client_id={}&redirect_uri={}", baseUrl, appKey, callbackUrl);
    cout << url << endl;
    // create a http server to receive requests
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
  }

  return EXIT_SUCCESS;
}
