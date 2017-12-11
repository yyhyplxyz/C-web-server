#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "server.h"

// 初始化HTTP请求
http_request* http_request_init(void);

// 释放HTTP请求
void http_request_free(http_request *req);

// 根据HTTP/1.0协议验证HTTP请求是否合法
int http_request_complete(connection *con);

// 解析HTTP请求
void http_request_parse(server *serv, connection *con);

#endif
