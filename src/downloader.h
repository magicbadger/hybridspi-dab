#ifndef DOWNLOADER_H_
#define DOWNLOADER_H_

#include <string>
#include <vector>

using namespace std;

class Content
{
    public:
        
        static size_t Write(void *ptr, size_t size, size_t nmemb, void *p);
        
        size_t WriteImpl(const char *ptr, size_t size, size_t nmemb);
        
        vector<unsigned char> Data() { return data; };
        
    private:
    
        vector<unsigned char> data;
};

class Downloader
{    
    public:

        Downloader();
        
        ~Downloader();
        
        /**
        * Download a file using HTTP GET and store in in a std::string
        * @param url The URL to download
        * @return The download result
        */
        vector<unsigned char> download(const string& url);
        
    private:

        void* curl;
};

#endif