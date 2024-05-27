#include <fmt/format.h>
#include <potato/greeter.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <iostream>


using namespace fantastic_potato;
using namespace std;
using namespace boost::beast::detail::base64;

using json = nlohmann::json;

std::string baseUrl="";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

Potato::Potato(std::string _name) : name(std::move(_name)) {
  string appKey = getenv("SCHWAB_APP_KEY");
  string appSecret = getenv("SCHWAB_APP_SECRET");
  
  string base64Code;
  string src = appKey + ":" + appSecret;
  base64Code.resize(encoded_size(src.size()));

  encode(&base64Code[0], src.c_str(), src.size());
  auth = "Authorization: Basic " + base64Code;
}

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

std::string Potato::post(std::string url, std::string data) const {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();

  struct curl_slist *headers= NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, auth.c_str());
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "error";
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
  return "error";
}

std::string Potato::getAfterAuth(std::string url) const {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();

  struct curl_slist *headers= NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, bearerAuth.c_str());
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "error";
}

void Potato::setBearerAuth(std::string accessToken) {
  bearerAuth = "Authorization: Bearer " + accessToken;
}


