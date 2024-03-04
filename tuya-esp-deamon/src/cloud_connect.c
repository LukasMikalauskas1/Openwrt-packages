#include "tuya_cacert.h"
#include "cloud_connect.h"
#include "cloud_utils.h"

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
        syslog(LOG_INFO, "Connected to cloud!");
        TY_LOGI("Connected to cloud!");
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
        syslog(LOG_INFO, "Disconnected from cloud!");
        closelog();
        TY_LOGI("Disconnected from cloud!");
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
        TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
        syslog(LOG_INFO, "on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);

        switch (msg->type)
        {
        case THING_TYPE_ACTION_EXECUTE:
                syslog(LOG_INFO, "Recieved data - %s", msg->data_string);
                execute_action(context, msg->data_string, msg->device_id);
        default:
                TY_LOGI("Default message: %s", msg->data_string);
                break;
        }
        printf("\r\n");
}

void cloud_disconect(tuya_mqtt_context_t *context)
{
        syslog(LOG_INFO, "Disconecting from cloud...");
        tuya_mqtt_disconnect(context);
        tuya_mqtt_deinit(context);
}

int cloud_connect(tuya_mqtt_context_t *context, struct arguments args)
{
        syslog(LOG_INFO, "Conectinng to cloud...");
        int ret = OPRT_OK;
        ret = tuya_mqtt_init(context, &(const tuya_mqtt_config_t){
                                          .host = "m1.tuyacn.com",
                                          .port = 8883,
                                          .cacert = (const uint8_t *)tuya_cacert_pem,
                                          .cacert_len = sizeof(tuya_cacert_pem),
                                          .device_id = args.device_id,
                                          .device_secret = args.device_secret,
                                          .keepalive = 100,
                                          .timeout_ms = 2000,
                                          .on_connected = on_connected,
                                          .on_disconnect = on_disconnect,
                                          .on_messages = on_messages});

        if (ret != OPRT_OK)
        {
                syslog(LOG_INFO, "mqtt connect failed");
                exit(EXIT_FAILURE);
        }

        ret = tuya_mqtt_connect(context); // memleak

        if (ret != OPRT_OK)
        {
                syslog(LOG_INFO, "mqtt conncet failed");
                exit(EXIT_FAILURE);
        }
        return ret;
}