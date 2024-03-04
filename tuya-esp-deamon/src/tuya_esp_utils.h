#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <syslog.h>
#include <signal.h>

#include "mqtt_client_interface.h"
#include "cJSON.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"

#include "argparse.h"
#include "cloud_connect.h"

/*Function that starts the tuya esp controler program*/
int start_tuya_controler(int argc, char **argv);

/*Function used to handle signals*/
void signal_handler(int signal);

/*Function used to start program as a deamon*/
int start_deamon(bool value);

/*Function used to ignore incoming signals*/
void ignore_signals();

/*Function used to handle incoming signals*/
void handle_signals();

#endif