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

    int success = 0;

    for (int attempt = 0; attempt <= opts.retry_count; attempt++) {
        int sock = tcp_connect(
            url.host,
            url.port,
            opts.timeout
        );

        if (sock < 0) {
            fprintf(stderr,
                "connection attempt %d failed\n",
                attempt + 1
            );

            continue;
        }

        if (http_send_request(sock, &url, &opts) == 0) {
            if (http_read_response(
                    sock,
                    opts.output_file,
                    opts.include_headers,
                    opts.head_only
                ) == 0) {
                success = 1;
                close(sock);
                break;
            }
        }

        close(sock);
    }

    if (!success) {
        fprintf(stderr, "request failed\n");
        return 1;
    }

    return 0;
}