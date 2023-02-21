#include "utils.h"

#include <iostream>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string *response = (std::string *) userdata;
    size_t num_bytes = size * nmemb;
    response->append(ptr, num_bytes);
    return num_bytes;
}
std::string do_post(const std::string &httpaddr, const Json::Value &json_message, const std::map<std::string, std::string> &headers) {

    // Create a new curl handle
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        throw std::runtime_error("Failed to initialize curl");
    }

    // Set the URL to POST to
    curl_easy_setopt(curl_handle, CURLOPT_URL, httpaddr.c_str());

    // Set the request method to POST
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);

    // Set the request headers
    struct curl_slist *header_list = nullptr;
    for (auto const &header : headers) {
        std::string header_string = header.first + ": " + header.second;
        header_list = curl_slist_append(header_list, header_string.c_str());
    }
    header_list = curl_slist_append(header_list, "Content-Type: application/json");
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, header_list);

    // Set the request body to the JSON message
    std::string json_string = json_message.toStyledString();
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json_string.c_str());

    const char *http_proxy = std::getenv("http_proxy");
    if (!http_proxy) {
        http_proxy = std::getenv("HTTP_PROXY");
    }
    if (http_proxy) {
        curl_easy_setopt(curl_handle, CURLOPT_PROXY, http_proxy);
    }


    // Set the callback function for receiving data from the HTTP response
    std::string response;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

    // Perform the HTTP request
    CURLcode curl_result = curl_easy_perform(curl_handle);
    if (curl_result != CURLE_OK || response.length()<=1) {
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl_handle);
        std::cerr<<"Connect failed. "<<curl_easy_strerror(curl_result)<<std::endl;
        throw "curl failed.";
    }

    // Clean up resources and return the response
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl_handle);
    return response;
}

std::string do_get(const std::string &httpaddr, const std::map<std::string, std::string> &headers) {

    // Create a new curl handle
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        throw std::runtime_error("Failed to initialize curl");
    }

    // Set the URL to POST to
    curl_easy_setopt(curl_handle, CURLOPT_URL, httpaddr.c_str());

    // Set the request method to POST
    curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);

    // Set the request headers
    struct curl_slist *header_list = nullptr;
    for (auto const &header : headers) {
        std::string header_string = header.first + ": " + header.second;
        header_list = curl_slist_append(header_list, header_string.c_str());
    }
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, header_list);

    const char *http_proxy = std::getenv("http_proxy");
    if (!http_proxy) {
        http_proxy = std::getenv("HTTP_PROXY");
    }
    if (http_proxy) {
        curl_easy_setopt(curl_handle, CURLOPT_PROXY, http_proxy);
    }

    // Set the callback function for receiving data from the HTTP response
    std::string response;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

    // Perform the HTTP request
    CURLcode curl_result = curl_easy_perform(curl_handle);
    if (curl_result != CURLE_OK || response.length()<=1) {
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl_handle);
        std::cerr<<"Connect failed. "<<curl_easy_strerror(curl_result)<<std::endl;
        throw "curl failed.";
    }

    // Clean up resources and return the response
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl_handle);
    return response;
}
