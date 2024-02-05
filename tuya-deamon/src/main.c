#include "argparse.h"
#include "program.h"
#include <syslog.h>

int main(int argc, char **argv)
{
        openlog("Tuya_deamon", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "=====TUYA_DEAMON=====");
        
        struct arguments arguments = parse_arguments(argc, argv);
        return exe_program(arguments);
}