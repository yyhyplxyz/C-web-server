#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include "stringutils.h"
#include "log.h"

// 以append模式打开日志文件
void log_open(server *serv, const char *logfile) {
    if (serv->use_logfile) {
        serv->logfp = fopen(logfile, "a");

        if (!serv->logfp) {
            perror(logfile);
            exit(1);
        }

        return;
    }

    openlog("webserver", LOG_NDELAY | LOG_PID, LOG_DAEMON);
}

// 关闭日志文件
void log_close(server *serv) {
    if (serv->logfp)
        fclose(serv->logfp);
    closelog();
}

// 生成时间戳字符串
static void date_str(string *s) {
    struct tm *ti;
    time_t rawtime;
    char local_date[100];
    char zone_str[20];
    int zone;
    char zone_sign;

    time(&rawtime);
    ti = localtime(&rawtime);
    zone = ti->tm_gmtoff / 60;

    if (ti->tm_zone < 0) {
        zone_sign = '-';
        zone = -zone;
    } else
        zone_sign = '+';
    
    zone = (zone / 60) * 100 + zone % 60;

    strftime(local_date, sizeof(local_date), "%d/%b/%Y:%X", ti);
    snprintf(zone_str, sizeof(zone_str), " %c%.4d", zone_sign, zone);

    string_append(s, local_date);
    string_append(s, zone_str);
}

// 记录HTTP请求
void log_request(server *serv, connection *con) {
    http_request *req = con->request;
    http_response *resp = con->response;
    char host_ip[INET_ADDRSTRLEN];
    char content_len[20];
    string *date = string_init();

    if (!serv || !con)
        return;

    if (resp->content_length > -1 && req->method != HTTP_METHOD_HEAD) {
        snprintf(content_len, sizeof(content_len), "%d", resp->content_length); 
    } else {
        strcpy(content_len, "-");
    }

    inet_ntop(con->addr.sin_family, &con->addr.sin_addr, host_ip, INET_ADDRSTRLEN);
    date_str(date);

    // 日志中需要记录的项目：IP，时间，访问方法，URI，版本，状态，内容长度
    if (serv->use_logfile) {
        fprintf(serv->logfp, "%s - - [%s] \"%s %s %s\" %d %s\n",
                host_ip, date->ptr, req->method_raw, req->uri,
                req->version_raw, con->status_code, content_len);
        fflush(serv->logfp);
    } else {
        syslog(LOG_ERR, "%s - - [%s] \"%s %s %s\" %d %s",
                host_ip, date->ptr, req->method_raw, req->uri,
                req->version_raw, con->status_code, content_len);
    }

    string_free(date);
}

// 写入日志函数
static void log_write(server *serv, const char *type, const char *format, va_list ap) {
    string *output = string_init();

    // 写入时间，消息类型
    if (serv->use_logfile) {
        string_append_ch(output, '[');
        date_str(output);
        string_append(output, "] ");
    }

    string_append(output, "[");
    string_append(output, type);
    string_append(output, "] ");
    
    string_append(output, format);

    if (serv->use_logfile) {
        string_append_ch(output, '\n');
        vfprintf(serv->logfp, output->ptr, ap);
        fflush(serv->logfp);
    } else {
        vsyslog(LOG_ERR, output->ptr, ap);
    }

    string_free(output);
}

// 记录出错信息
void log_error(server *serv, const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    log_write(serv, "error", format, ap);
    va_end(ap);
}

// 记录日志信息
void log_info(server *serv, const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    log_write(serv, "info", format, ap);
    va_end(ap);
}
