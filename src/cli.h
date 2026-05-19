#ifndef CLI_H
#define CLI_H

#define MAX_HEADERS 32

typedef struct
{
    char *url;
    char *output_file;

    char *method;
    char *body;

    char *auth;

    int head_only;
    int include_headers;

    int timeout;
    int follow_redirects;
    int retry_count;

    char *headers[MAX_HEADERS];
    int header_count;

} CliOptions;

int cli_parse(int argc, char **argv, CliOptions *opts);
void cli_help(const char *prog);

#endif