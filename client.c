/**
* 文件同步客户端
* @Author felix.liu
* @Date 2015-03-09
*/
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h"
#include "client.h"


CURL *curl;
CURLcode res;

/**
* 文件MD5校验
*/
int file_md5_check(char *path, char *md5_str)
{
    char *file_md5 = MD5_file(path);
    printf("下载文件MD5值:%s \n比对MD5值:%s\n", file_md5, md5_str);
    if(strcmp(md5_str, file_md5) == 0){
        free(file_md5);
        return 0;
    }else{
        free(file_md5);
        return -1;
    }
}

/**
* 获取文件MD5值
*/
char *MD5_file (char *path)
{
    FILE *fp = fopen("m.sql", "rb");
    char *file_md5;
    unsigned char buf[128];
    int n;
    unsigned char digest[16];
    MD5_CTX   context;

    if(fp == NULL){
        fprintf(stderr, "fopen %s failed\n", path);
        return NULL;
    }

    MD5Init(&context);

    while ((n = fread(buf, 1, 128, fp)) > 0) {
        MD5Update(&context, buf, n);
    }

    MD5Final(digest, &context);
    
    file_md5 = (char *)malloc(33 * sizeof(char));
    if(file_md5 == NULL)
    {
        fprintf(stderr, "malloc failed.\n");
        fclose(fp);
        return NULL;
    }
    memset(file_md5, 0, 33);
    
    make_digest(file_md5, digest);
    
    fclose(fp);
    return file_md5;
}


/**
* 下载文件
*/
int download_file(char *url, char *filename)
{
    FILE *fp;
    fp = fopen(filename, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    fclose(fp);

    if(CURLE_OK != res){
        return -1;
    }else{
        return 0;
    }
}

/**
* 心跳包发送
* times为心跳包时间间隔,单位毫秒
*/
int heartBeat(int times)
{
    return 0;
}

/**
* 获取当前时间
*/
int get_datetime(char *now)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(now, "%4d-%02d-%02d %02d:%02d:%02d"
                   , 1900 + timeinfo->tm_year
                   , 1+timeinfo->tm_mon
                   , timeinfo->tm_mday
                   , timeinfo->tm_hour
                   , timeinfo->tm_min
                   , timeinfo->tm_sec);
    return 0;
}

/**
* 同步文件
* check_md5 文件原始MD5值
*/
int sync(char *url, char *check_md5)
{
    char datetime[32];
    char *filename = "m.sql";

    //获取当前时间
    get_datetime(datetime);
    curl = curl_easy_init();

    int st = download_file(url, filename);
    curl_easy_cleanup(curl);
    printf("\n-----------------%s--------------------\n", datetime);
    printf("下载地址:%s\n", url);
    if(st == -1){
        printf("%s下载文件失败!\n", filename);
        return -1;
    }else{
        int sn = file_md5_check(filename, check_md5);
        if(sn == -1){
            printf("%s文件校验失败!\n", filename);
            return -1;
        }else{
            printf("%s文件校验成功!\n", filename);
            return 0;
        }
    }
}

int main()
{
    char url[1024] = "http://xxxxxxxxxx:8027/m.sql";
    char md5_check[33] = "29286b0259435f58d9fdcf455fbf417e";
    int status = sync(url, md5_check);
    
    return 0;
}
