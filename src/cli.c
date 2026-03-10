#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cli.h"

static void init_opts(CliOptions *opts)
{
    opts->url = NULL;
    opts->output_file = NULL;
    opts->head_only = 0;
    opts->include_headers = 0;
}

int cli_parse(int argc, char **argv, CliOptions *opts)
{
    init_opts(opts);

    int opt;

    while ((opt = getopt(argc, argv, "o:Iih")) != -1)
    {
        switch (opt)
        {
            case 'o':
                opts->output_file = optarg;
                break;

            case 'I':
                opts->head_only = 1;
                break;

            case 'i':
                opts->include_headers = 1;
                break;

            case 'h':
                cli_help(argv[0]);
                exit(0);

            default:
                cli_help(argv[0]);
                return -1;
        }
    }

    if (optind >= argc)
    {
        fprintf(stderr, "URL required\n");
        return -1;
    }

    opts->url = argv[optind];

    return 0;
}

void cli_help(const char *prog)
{
    printf("Usage: %s [options] <url>\n", prog);
    printf("Options:\n");
    printf("  -o <file>   write output to file\n");
    printf("  -I          HEAD request\n");
    printf("  -i          include headers\n");
    printf("  -h          help\n");
}