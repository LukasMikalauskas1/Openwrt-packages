#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <syslog.h>

#include "mqtt_client_interface.h"
#include "cJSON.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"

#include "argparse.h"
#include "cloud.h"
#include "program.h"

#include "ubus.h"

tuya_mqtt_context_t client_instance;
struct ubus_context *ctx;

int exe_program(struct arguments args)
{
        int ret = OPRT_OK;
        bool connected = false;

        ret = start_deamon(args.start_deamon);
        if (ret == EXIT_FAILURE)
                return EXIT_FAILURE;

        tuya_mqtt_context_t *client = &client_instance;

        ignore_signals();
        while (!connected) // Waits till the program conncets to the cloud to prevent memory leak
        {
                ret = cloud_connect(client, args);
                if (ret != OPRT_OK)
                {
                        TY_LOGI("Error: conection to toya Iot cloud failed");
                        syslog(LOG_ERR, "Error: conection to toya Iot cloud failed");
                        cloud_disconect(client);
                        return EXIT_FAILURE;
                }
                connected = true;
                ret = OPRT_OK;
        }
        handle_signals();

        syslog(LOG_DEBUG, "Conecting to ubus");

        ctx = ubus_connect(NULL);
        if (ctx == NULL)
        {
                syslog(LOG_ERR, "ERROR: failed to conncet to ubus");
                clean_up();
                return -1;
        }
        syslog(LOG_INFO, "Conected to ubus");

        struct MemData memory = {0};

        while (true)
        {
                tuya_mqtt_loop(client);
                ubus_get_mem_info(&memory, ctx);
                send_report(client, args, generate_report_json(memory));
        }

        return ret;
}

void signal_handler(int signal)
{
        syslog(LOG_INFO, "Teminating program...");
        clean_up();
        syslog(LOG_INFO, "Program terminated!");
        syslog(LOG_INFO, "=====================");
        closelog();
        exit(EXIT_SUCCESS);
}

int start_deamon(bool is_deamon)
{
        if (is_deamon)
        {
                if (daemon(0, 0))
                {
                        syslog(LOG_ERR, "Error: failed to start program as a deamon");
                        return 1;
                }
                syslog(LOG_INFO, "Started program as deamon");
        }
        return 0;
}

void ignore_signals()
{
        signal(SIGTERM, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGINT, SIG_IGN);
}

void handle_signals()
{
        signal(SIGTERM, signal_handler);
        signal(SIGTSTP, signal_handler);
        signal(SIGINT, signal_handler);
}

void clean_up()
{
        cloud_disconect(&client_instance);
        ubus_disconnect(ctx);
}

char *generate_report_json(struct MemData memory)
{
        cJSON *object = cJSON_CreateObject();

        cJSON_AddItemToObject(object, "Total", cJSON_CreateNumber(memory.total));
        cJSON_AddItemToObject(object, "Free", cJSON_CreateNumber(memory.free));
        cJSON_AddItemToObject(object, "Shared", cJSON_CreateNumber(memory.shared));
        cJSON_AddItemToObject(object, "Buffered", cJSON_CreateNumber(memory.buffered));
        cJSON_AddItemToObject(object, "Available", cJSON_CreateNumber(memory.available));
        cJSON_AddItemToObject(object, "Cached", cJSON_CreateNumber(memory.cached));

        char *report = cJSON_Print(object);

        cJSON_Delete(object);

        return report;
}