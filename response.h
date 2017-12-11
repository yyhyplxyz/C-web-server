#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "server.h"

// 初始化HTTP响应
http_response* http_response_init();

// 释放HTTP响应
void http_response_free(http_response *resp);

// 发送HTTP响应
void http_response_send(server *serv, connection *con);

#endif
