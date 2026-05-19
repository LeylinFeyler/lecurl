#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <ctype.h>
#include "http.h"
#include "cli.h"

#define BUFFER_SIZE 4096

static int read_line(int sock, char *buffer, int max_len)
{
    int i = 0;
    char c;
    while (i < max_len - 1)
    {
        int n = recv(sock, &c, 1, 0);  
        if (n <= 0) break;             
        buffer[i++] = c;
        if (i >= 2 && buffer[i-2] == '\r' && buffer[i-1] == '\n') break;
    }
    buffer[i] = 0;
    return i;
}

static int hex_to_int(const char *hex)
{
    int val = 0;
    while (*hex)
    {
        char c = *hex++;
        if (isdigit(c)) val = val*16 + (c-'0');
        else if (c >= 'a' && c <= 'f') val = val*16 + (c-'a'+10);
        else if (c >= 'A' && c <= 'F') val = val*16 + (c-'A'+10);
        else break; // stop at first non-hex character
    }
    return val;
}

int http_send_request(int sock, Url *url, CliOptions *opts)
{
    char request[4096];

    int len = snprintf(request, sizeof(request),
        "%s %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: lecurl/0.1\r\n"
        "Accept: */*\r\n",
        opts->method,
        url->path,
        url->host
    );

    for (int i = 0; i < opts->header_count; i++)
    {
        len += snprintf(request + len, sizeof(request) - len,
            "%s\r\n",
            opts->headers[i]
        );
    }

    if (opts->body)
    {
        len += snprintf(request + len, sizeof(request) - len,
            "Content-Length: %ld\r\n",
            strlen(opts->body)
        );
    }

    len += snprintf(request + len, sizeof(request) - len, "\r\n");

    if (opts->body)
    {
        snprintf(request + len, sizeof(request) - len,
            "%s",
            opts->body
        );
    }

    send(sock, request, strlen(request), 0);
    return 0;
}

int http_read_response(int sock, const char *outfile, int include_headers, int head_only)
{
    char buffer[BUFFER_SIZE];
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
    int chunked = 0;

    // for HEAD requests, always include headers
    if (head_only)
        include_headers = 1;

    // read headers line by line
    while (!headers_done)
    {
        int n = read_line(sock, buffer, sizeof(buffer));
        if (n <= 0) break;

        if (include_headers)
            write(fd, buffer, n);

        if (strcmp(buffer, "\r\n") == 0)
        {
            headers_done = 1; // empty line marks end of headers
        }

        // detect chunked transfer encoding
        if (strncasecmp(buffer, "Transfer-Encoding:", 18) == 0 &&
            strstr(buffer, "chunked") != NULL)
        {
            chunked = 1;
        }
    }

    // if HEAD, we are done after printing headers
    if (head_only)
    {
        if (outfile) close(fd);
        return 0;
    }

    // read body
    if (chunked)
    {
        while (1)
        {
            int n = read_line(sock, buffer, sizeof(buffer));
            if (n <= 0) break;

            int chunk_size = hex_to_int(buffer);
            if (chunk_size == 0) break; // last chunk

            int remaining = chunk_size;
            while (remaining > 0)
            {
                int to_read = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;
                int r = recv(sock, buffer, to_read, 0);
                if (r <= 0) break;
                write(fd, buffer, r);
                remaining -= r;
            }
            recv(sock, buffer, 2, 0); // skip trailing \r\n after each chunk
        }
    }
    else
    {
        // read entire body if not chunked
        int bytes;
        while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0)
        {
            write(fd, buffer, bytes);
        }
    }

    if (outfile) close(fd);
    return 0;
}