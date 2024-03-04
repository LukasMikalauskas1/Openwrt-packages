#ifndef CLOUD_CONNECT_H_
#define CLOUD_CONNECT_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "mqtt_client_interface.h"
#include "cJSON.h"

#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"
#include "argparse.h"

#include "tuya_ubus_utils.h"
#include <syslog.h>

/*Function used to connect to tuya cloud*/
int cloud_connect(tuya_mqtt_context_t *context, struct arguments args);

/*Function used to disconnect from tuya cloud and free resources*/
void cloud_disconect(tuya_mqtt_context_t *context);

/*Function used to log connection*/
void on_connected(tuya_mqtt_context_t *context, void *user_data);

/*Function used to log disconnection*/
void on_disconnect(tuya_mqtt_context_t *context, void *user_data);

/*Function used to handle incoming messages from tuya cloud*/
void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg);

#endif