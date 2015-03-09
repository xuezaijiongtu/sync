/**
* 文件MD5校验
*/
int file_md5_check(char *path, char *md5_str);

/**
* 获取文件MD5值
*/
char *MD5_file (char *path);

/**
* 下载文件
*/
int download_file(char *url, char *filename);

/**
* 获取当前时间
*/
int get_datetime(char *now);


/**
* 心跳包发送
* times为心跳包时间间隔,单位毫秒
*/
int heartBeat(int times);

/**
* 同步文件
*/
int sync(char *url, char *check_md5);
