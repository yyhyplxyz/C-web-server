#include <assert.h>
#include <string.h>
#include "http_header.h"

#define HEADER_SIZE_INC 20

// 初始化HTTP头部
http_headers* http_headers_init() {
    http_headers *h;
    h = malloc(sizeof(*h));
    memset(h, 0, sizeof(*h));

    return h;
}

// 释放HTTP头部
void http_headers_free(http_headers *h) {
    if (!h) return;

    for (size_t i = 0; i < h->len; i++) {
        string_free(h->ptr[i].key);
        string_free(h->ptr[i].value);
    }

    free(h->ptr);
    free(h);

}

// 扩展HTTP头部空间
static void extend(http_headers *h) {
    if (h->len >= h->size) {
        h->size += HEADER_SIZE_INC;
        h->ptr = realloc(h->ptr, h->size * sizeof(keyvalue));
    }
}

// 添加新的key-value对到HTTP头部
void http_headers_add(http_headers *h, const char *key, const char *value) {
    assert(h != NULL);
    extend(h);

    h->ptr[h->len].key = string_init_str(key); 
    h->ptr[h->len].value = string_init_str(value);
    h->len++;
}

void http_headers_add_int(http_headers *h, const char *key, int value) {
    assert(h != NULL);
    extend(h);

    string *value_str = string_init();
    string_append_int(value_str, value);

    h->ptr[h->len].key = string_init_str(key); 
    h->ptr[h->len].value = value_str;
    h->len++;
}
