#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "cli.h"

static struct option long_options[]=
{
    {"connect-timeout",required_argument,0,1000},
    {"retry",required_argument,0,1001},

    {0,0,0,0}
};

static void init_opts(CliOptions *opts)
{
    memset(opts, 0, sizeof(*opts));
    opts->url = NULL;
    opts->output_file = NULL;
    opts->head_only = 0;
    opts->include_headers = 0;
    opts->method="GET";
    opts->timeout=5;
    opts->retry_count=0;
}

int cli_parse(int argc, char **argv, CliOptions *opts)
{
    init_opts(opts);

    int opt;

    while((opt=getopt_long(
            argc,
            argv,
            "o:IiH:X:d:u:Lh",
            long_options,
            NULL))!=-1)
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

            case 'X':
                opts->method=optarg;
                break;

            case 'd':
                opts->body=optarg;
                break;

            case 'H':
                if(opts->header_count>=MAX_HEADERS)
                {
                    fprintf(
                        stderr,
                        "too many headers\n"
                    );

                    return -1;
                }

                opts->headers[
                    opts->header_count++
                ]=optarg;

                break;

            case 'u':
                opts->auth=optarg;
                break;

            case 'L':
                opts->follow_redirects=1;
                break;

            case 1000:
                opts->timeout=
                    atoi(optarg);
                break;

            case 1001:
                opts->retry_count=
                    atoi(optarg);
                break;

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
    printf("  -X METHOD      request method\n");
    printf("  -d DATA        request body\n");
    printf("  -H HEADER      custom header\n");
    printf("  -u USER:PASS   basic auth\n");
    printf("  -L             follow redirects\n");
    printf("  --retry N      retry count\n");
    printf("  --connect-timeout SEC\n");    
}