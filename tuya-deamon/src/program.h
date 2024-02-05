#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdbool.h>
#include "ubus.h"

/*
 * @brief Function that executes the program 
 * @param args an argument structure that stores program arguments
 * @return 0 - succsess, error code
 */
int exe_program(struct arguments args);
/*
 * @brief Function that is used to handle program termination
 */
void signal_handler(int signal);
/*
 * @brief Function that start the program as a daemon process
 * @param value a bool that indicates if program should be started as daemon
 * @return 0 - succses 1 - failure
 */
int start_deamon(bool value);
/*
 * @brief Function that is used to ignore certain signals
 */
void ignore_signals();
/*
 * @brief Function that is used to handle signals
 */
void handle_signals();
/*
 * @brief Function that is used to free resources at termination
 */
void clean_up();
/*
 * @brief Function that is used to turn memory information from MemData struct 
 * into JSON string
 * @param memory a struct that stores memory information
 * @return JSON string of memory information
 */
char *generate_report_json(struct MemData memory);

#endif