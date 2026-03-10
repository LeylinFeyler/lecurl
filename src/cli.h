#ifndef CLI_H
#define CLI_H

typedef struct {
    char *url;
    char *output_file;
    int head_only;
    int include_headers;
} CliOptions;

int cli_parse(int argc, char **argv, CliOptions *opts);
void cli_help(const char *prog);

#endif