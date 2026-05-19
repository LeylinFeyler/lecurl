#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cli.h"
#include "url.h"
#include "net.h"
#include "http.h"

int main(int argc, char **argv)
{
    CliOptions opts;
    Url url;

    if (cli_parse(argc, argv, &opts) != 0)
        return 1;

    if (url_parse(opts.url, &url) != 0)
        return 1;


    int attempts = opts.retry_count + 1;

    while (attempts--) {
        int sock = tcp_connect(url.host, url.port, opts.timeout);
        if (sock < 0)
        {
            fprintf(stderr, "Failed to connect to %s:%d\n", url.host, url.port);
            return 1;
        }

        if (http_send_request(sock, &url, &opts) != 0)
        {
            fprintf(stderr, "Failed to send HTTP request\n");
            close(sock);
            return 1;
        }

        if (http_read_response(sock, opts.output_file, opts.include_headers, opts.head_only) != 0)
        {
            fprintf(stderr, "Failed to read HTTP response\n");
            close(sock);
            return 1;
        }

        close(sock);
        break;
    }

    return 0;
}