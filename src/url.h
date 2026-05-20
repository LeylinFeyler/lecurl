#ifndef URL_PARSER_H
#define URL_PARSER_H

typedef struct {
    char scheme[16];
    char host[256];
    int port;
    char path[1024];
} Url;

int url_parse(const char *url_str, Url *url);

#endif