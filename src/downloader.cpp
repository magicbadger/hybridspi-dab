#include "downloader.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>

#include <sstream>
#include <iostream>

using namespace std;

size_t Content::Write(void *data, size_t size, size_t nmemb, void *p)
{
    return static_cast<Content*>(p)->WriteImpl((const char*)data, size, nmemb);
}

size_t Content::WriteImpl(const char* ptr, size_t size, size_t nmemb)
{
    data.insert(end(data), ptr, ptr + size * nmemb);
    return size * nmemb;
}

Downloader::Downloader() {
    curl = curl_easy_init();
}

Downloader::~Downloader() {
    curl_easy_cleanup(curl);
}

vector<unsigned char> Downloader::download(const string& url) {
    
    //cout << "downloading " << url << endl;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
    
    Content content;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Content::Write);
    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    return content.Data();
}