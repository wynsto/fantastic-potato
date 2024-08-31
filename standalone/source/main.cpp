#include <fmt/format.h>
#include <schwab/db.h>
#include <schwab/greeter.h>
#include <schwab/http_connection.h>
#include <schwab/version.h>

#include <array>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/version.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cxxopts.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace boost::beast::detail::base64;
using namespace std;
namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

// "Loop" forever accepting new connections.
void http_server(tcp::acceptor& acceptor, tcp::socket& socket) {
  acceptor.async_accept(socket, [&](beast::error_code ec) {
    if (!ec) std::make_shared<http_connection>(std::move(socket))->start();
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
  clog << "refreshToken: " << refreshToken << endl;

  string baseUrl = "https://api.schwabapi.com/v1";
  string appKey = std::getenv("SCHWAB_APP_KEY");
  string fxKey = std::getenv("FX_KEY");
  string appSecret = std::getenv("SCHWAB_APP_SECRET");
  string callbackUrl = std::getenv("SCHWAB_CALLBACK_URL");
  string oauth = "oauth/authorize";
  // if (accessToken != "" && refreshToken != "") {
  schwab_personal_trading::Potato potato("potato");
  accessToken = potato.getAccessToken("refresh_token", refreshToken);

  // string marketData = "https://api.schwabapi.com/marketdata/v1";
  string quotesUrl
      = "https://fcsapi.com/api-v3/forex/"
        "latest?symbol=GBP/USD,USD/HKD,USD/CNH,USD/JPY,EUR/USD,USD/CAD,AUD/USD,USD/SGD,USD/"
        "THB&access_key="
        + fxKey;
  // string quotesUrl
  //     = "https://api.schwabapi.com/marketdata/v1/quotes?symbols=";
  // potato.setBearerAuth(accessToken);
  string resp = potato.getAfterAuth(quotesUrl);
  cout << resp << endl;
  auto quotes = json::parse(resp);
  auto resposes = quotes["response"];
  float usd = 1.0;
  string gbp = resposes[7]["o"];
  gbp = usd / stof(gbp);
  string hkd = resposes[4]["o"];
  string cnh = resposes[5]["o"];
  string jpy = resposes[3]["o"];
  string eur = resposes[0]["o"];
  eur = usd / stof(eur);
  string cad = resposes[2]["o"];
  string aud = resposes[1]["o"];
  aud = usd / stof(aud);
  string sgd = resposes[6]["o"];
  string thb = resposes[8]["o"];

  // create table fx_rate (hkd real, usd real, cny real, jpy real, gbp real, eur real, cad real,
  // aud real, sgd real, thb real, time timestamp, date string);
  string sql = fmt::format(
      "INSERT INTO fx_rate (hkd, usd, cny, jpy, gbp, eur, cad, aud, sgd, thb, time, date) values "
      "({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, date())",
      hkd, usd, cnh, jpy, gbp, eur, cad, aud, sgd, thb, timer);
  clog << sql << endl;
  schwab_personal_trading::SchwabDB fxRateDb(
      "/home/wynston/dev/flask-project/instance/home.sqlite");

  fxRateDb.query(sql);

  return EXIT_SUCCESS;
  // } else {
  //   string url = fmt::format("{}/oauth/authorize?client_id={}&redirect_uri={}", baseUrl, appKey,
  //                            callbackUrl);
  //   cout << url << endl;
  //   // create a http server to receive requests
  //   try {
  //     auto const address = net::ip::make_address("0.0.0.0");
  //     unsigned short port = static_cast<unsigned short>(std::atoi("8081"));

  //     net::io_context ioc{1};

  //     tcp::acceptor acceptor{ioc, {address, port}};
  //     tcp::socket socket{ioc};
  //     http_server(acceptor, socket);

  //     ioc.run();
  //   } catch (std::exception const& e) {
  //     std::cerr << "Error: " << e.what() << std::endl;
  //     return EXIT_FAILURE;
  //   }
  // }

  // return EXIT_SUCCESS;
}
