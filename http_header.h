#ifndef _HTTP_HEADER_H_
#define _HTTP_HEADER_H_

#include "server.h"

// 初始化HTTP头部
http_headers* http_headers_init();

// 释放HTTP头部
void http_headers_free(http_headers *h);

// 添加新的key-value对到HTTP头部
void http_headers_add(http_headers *h, const char *key, const char *value);
void http_headers_add_int(http_headers *h, const char *key, int value);

#endif
