#include <iostream>
using namespace std;
#include <thread>
#include <stdio.h>
#include <curl/curl.h>
#include <string>

size_t callbackfunction(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}

void download_jpeg(const char* url, const char* filen)
{
    errno_t fper;
    FILE* fp;
    fper = fopen_s(&fp,filen, "wb");
    if (fper)
    {
        printf("!!! Failed to create file on the disk\n");
        return;
    }
    CURL* curlCtx = curl_easy_init();
    curl_easy_setopt(curlCtx, CURLOPT_URL, url);
    curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
    curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curlCtx, CURLOPT_NOPROGRESS, false);
    Sleep(5);

    CURLcode rc = curl_easy_perform(curlCtx);
    if (rc)
    {
        printf("!!! Failed to download: %s\n", url);
        return;
    }

    long res_code = 0;
    curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
    if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
    {
        printf("!!! Response code: %d\n", res_code);
        return;
    }

    curl_easy_cleanup(curlCtx);

    fclose(fp);
 
    return;
}

int main(int argc, char** argv)
{

    thread td1(download_jpeg, "https://images.pexels.com/videos/3163534/free-video-3163534.jpg?auto=compress&cs=tinysrgb&dpr=1&w=500", "td1.jpg");
    thread td2(download_jpeg, "https://images.pexels.com/photos/60597/dahlia-red-blossom-bloom-60597.jpeg?cs=srgb&dl=pexels-pixabay-60597.jpg", "td2.jpg");
    thread td3(download_jpeg, "https://images.pexels.com/photos/736230/pexels-photo-736230.jpeg?auto=compress&cs=tinysrgb&dpr=1&w=500", "td3.jpg");
    td1.join();
    td2.join();
    td3.join();
    return 0;
} 