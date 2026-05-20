#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "url.h"

int url_parse(const char *url_str, Url *url) {
    memset(url, 0, sizeof(Url));

    const char *url_ptr;

    url_ptr = strstr(url_str, "://");
    if (!url_ptr) {
        fprintf(stderr, "Invalid URL\n");
        return -1;
    }

    int scheme_len = url_ptr - url_str;
    strncpy(url->scheme, url_str, scheme_len);

    url_ptr += 3;

    const char *host_start = url_ptr;

    while (*url_ptr && *url_ptr != ':' && *url_ptr != '/') {
        url_ptr++;
    }

    int host_len = url_ptr - host_start;
    strncpy(url->host, host_start, host_len);

    if (*url_ptr == ':') {
        url_ptr++;

        url->port = atoi(url_ptr);

        while (*url_ptr && *url_ptr != '/') {
            url_ptr++;
        }
    } else {
        url->port = 80;
    }

    if (*url_ptr == '/') {
        strcpy(url->path, url_ptr);
    } else {
        strcpy(url->path, "/");
    }

    return 0;
}