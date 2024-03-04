#ifndef CLOUD_H_
#define CLOUD_H_

#include "tuyalink_core.h"
#include "argparse.h"
#include "tuya_ubus_utils.h"
#include "tuya_error_code.h"

enum
{
        ACTION_DEVICES,
        ACTION_ON,
        ACTION_OFF,
        ACTION_INVALID
};

struct Params
{
        char port[50];
        char pin[5];
        char action_code[50];
};

/*Function used to identify and execute action*/
int execute_action(tuya_mqtt_context_t *context, char *json_msg, char *device_id);

/*Function used to parse the json message*/
int parse_json_message(char *json_msg, struct Params *params);

/*Function used to convert action code form string to int*/
int parse_action(struct Params *params);

/*Function used to convert devices from struct to json string*/
char *construct_devices_json(struct Devices *devices);

/*Function used to get devices data from ubus and sending formated data to tuya cloud*/
int devices(tuya_mqtt_context_t *context, char *device_id);

/*Function used to send actions to esp controler*/
int control_esp(struct Params *params, char *response);

/*Function used to send responses to tuya cloud*/
void response_handler(tuya_mqtt_context_t *context, char *device_id, int rsp_code, char *rsp_msg);

#endif