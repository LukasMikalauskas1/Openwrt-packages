#include "tuya_esp_utils.h"

tuya_mqtt_context_t client_instance;

int start_tuya_controler(int argc, char **argv)
{
        openlog("Tuya_deamon", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "==========TUYA_DEAMON==========");

        int ret = OPRT_OK;
        bool connected = false;
        struct arguments args = parse_arguments(argc, argv);

        ret = start_deamon(args.start_deamon);
        if (ret == EXIT_FAILURE)
        {
                return EXIT_FAILURE;
        }

        tuya_mqtt_context_t *client = &client_instance;

        ignore_signals();
        while (!connected) // Waits till the program conncets to the cloud to prevent memory leak
        {
                ret = cloud_connect(client, args);
                if (ret != OPRT_OK)
                {
                        syslog(LOG_ERR, "Error: conection to toya Iot cloud failed");
                        cloud_disconect(client);
                        return EXIT_FAILURE;
                }
                connected = true;
                ret = OPRT_OK;
        }
        handle_signals();

        while (true)
        {
                tuya_mqtt_loop(client);
        }

        return ret;
}

void signal_handler(int signal)
{
        syslog(LOG_INFO, "Teminating program...");
        cloud_disconect(&client_instance);
        closelog();
        syslog(LOG_INFO, "Program terminated!");
        exit(EXIT_SUCCESS);
}

int start_deamon(bool is_deamon)
{
        if (is_deamon)
        {
                if (daemon(0, 0))
                {
                        syslog(LOG_ERR, "Error: failed to start program as a deamon");
                        return EXIT_FAILURE;
                }
                syslog(LOG_INFO, "Started program as a deamon!");
        }
        return EXIT_SUCCESS;
}

void ignore_signals()
{
        syslog(LOG_INFO, "Ignoring signals...");
        signal(SIGTERM, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGINT, SIG_IGN);
}

void handle_signals()
{
        syslog(LOG_INFO, "Handling signals...");
        signal(SIGTERM, signal_handler);
        signal(SIGTSTP, signal_handler);
        signal(SIGINT, signal_handler);
}