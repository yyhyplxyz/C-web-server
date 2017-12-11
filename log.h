#ifndef _LOG_H_
#define _LOG_H_

#include "server.h"

// 打开日志文件
void log_open(server *serv, const char *logfile);

// 关闭日志文件
void log_close(server *serv);

// 记录HTTP请求
void log_request(server *serv, connection *con);

// 记录出错信息
void log_error(server *serv, const char *format, ...);

// 记录日志信息
void log_info(server *serv, const char *format, ...);

#endif
