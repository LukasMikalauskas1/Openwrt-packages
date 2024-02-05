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

/*
 * @brief Function that parses program arguments
 * @param argc argument count
 * @param argv arguments
 * @return a struct of arguments
 */
struct arguments parse_arguments(int argc, char **argv);
/*
 * @brief Function that parses argument options
 */
error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif