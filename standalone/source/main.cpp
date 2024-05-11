#include <potato/greeter.h>
#include <potato/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>


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
  std::string resp = potato.get(url);
  std::cout << resp << std::endl;


  return 0;
}
