#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "http.h"

#define BUFFER_SIZE 4096


int http_send_request(int sock, Url *url, int head_only)
{
    char request[2048];

    const char *method = head_only ? "HEAD" : "GET";

    int len = snprintf(
        request,
        sizeof(request),
        "%s %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: mycurl/0.1\r\n"
        "Connection: close\r\n"
        "\r\n",
        method,
        url->path,
        url->host
    );

    if (send(sock, request, len, 0) < 0)
    {
        perror("send");
        return -1;
    }

    return 0;
}


int http_read_response(int sock, const char *outfile, int include_headers, int head_only)
{
    char buffer[BUFFER_SIZE];
    int bytes;
    int fd = STDOUT_FILENO;

    if (outfile)
    {
        fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("open");
            return -1;
        }
    }

    int headers_done = 0;

    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0)
    {
        if (include_headers || head_only)
        {
            write(fd, buffer, bytes);
        }
        else
        {
            if (!headers_done)
            {
                char *body = strstr(buffer, "\r\n\r\n");
                if (body)
                {
                    body += 4;
                    headers_done = 1;
                    int body_len = bytes - (body - buffer);
                    write(fd, body, body_len);
                }
            }
            else
            {
                write(fd, buffer, bytes);
            }
        }
    }

    if (outfile)
        close(fd);

    return 0;
}