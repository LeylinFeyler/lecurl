#ifndef HTTP_H
#define HTTP_H

#include "cli.h"
#include "url.h"

int http_send_request(int sock, Url *url, CliOptions *opts);

int http_read_response(int sock, const char *outfile, int include_headers, int head_only);

#endif