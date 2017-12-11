#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "server.h"

// 接受客户端连接
connection* connection_accept(server *serv);

// 关闭连接
void connection_close(connection *con);

// 处理客户端连接
int connection_handler(server *serv, connection *con);

#endif

