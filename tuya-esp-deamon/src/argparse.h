#ifndef ARGPARSE_
#define ARGPARSE_

#include <stdbool.h>
#include <argp.h>

struct arguments
{
        char *device_id;
        char *product_id;
        char *device_secret;
        bool start_deamon;
};

/*Function used to parse program arguments*/
struct arguments parse_arguments(int argc, char **argv);

#endif