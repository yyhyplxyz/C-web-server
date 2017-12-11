#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <limits.h>

// 配置文件数据结构
typedef struct {
    // 端口号
    short port;
    // Web文件目录
    char doc_root[PATH_MAX];
} config;

// 初始化配置
config* config_init();

// 释放配置
void config_free(config *conf);

// 加载配置
void config_load(config *conf, const char *fn);

#endif
