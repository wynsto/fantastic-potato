#include <potato/greeter.h>
#include <potato/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <array>
#include <ctime>

using boost::asio::ip::tcp;


std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

auto main(int argc, char** argv) -> int {
  const std::unordered_map<std::string, fantastic_potato::LanguageCode> languages{
      {"en", fantastic_potato::LanguageCode::EN},
      {"de", fantastic_potato::LanguageCode::DE},
      {"es", fantastic_potato::LanguageCode::ES},
      {"fr", fantastic_potato::LanguageCode::FR},
  };

  cxxopts::Options options(*argv, "A program to welcome the world!");

  std::string language;
  std::string name;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("n,name", "Name to greet", cxxopts::value(name)->default_value("World"))
    ("l,lang", "Language code to use", cxxopts::value(language)->default_value("en"))
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "Potato, version " << POTATO_VERSION << std::endl;
    return 0;
  }

  auto langIt = languages.find(language);
  if (langIt == languages.end()) {
    std::cerr << "unknown language code: " << language << std::endl;
    return 1;
  }

  fantastic_potato::Potato potato(name);
  std::cout << potato.greet(langIt->second) << std::endl;

  std::string url = "https://www.hongkongairport.com/flightinfo-rest/rest/flights/past?date=2024-05-02&lang=en&cargo=false&arrival=false";
  std::string baseUrl = "https://api.schwabapi.com/trader/v1";
  std::string resp = potato.get(url);

  std::cout << resp << std::endl;

  try
  {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    for (;;)
    {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      std::string message = make_daytime_string();

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}
