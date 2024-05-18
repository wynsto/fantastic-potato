#include <fmt/format.h>
#include <potato/greeter.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace fantastic_potato;

using json = nlohmann::json;

std::string baseUrl="";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

Potato::Potato(std::string _name) : name(std::move(_name)) {}

std::string Potato::greet(LanguageCode lang) const {
  
  switch (lang) {
    default:
    case LanguageCode::EN:
      return fmt::format("Hello, Potato {}!", name);
    case LanguageCode::DE:
      return fmt::format("Hallo Potato {}!", name);
    case LanguageCode::ES:
      return fmt::format("¡Hola Potato {}!", name);
    case LanguageCode::FR:
      return fmt::format("Bonjour Potato {}!", name);
  }
}

std::string Potato::post(std::string url) const {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    auto res = json::parse(readBuffer);
    if (!res.empty()) {
      return res[0]["date"];
    }
    return readBuffer;
  }
}

std::string Potato::get(std::string url) const {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    auto res = json::parse(readBuffer);
    if (!res.empty()) {
      return res[0]["date"];
    }
    return readBuffer;
  }
}

