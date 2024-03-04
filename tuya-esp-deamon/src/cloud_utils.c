#include "cloud_connect.h"
#include "cloud_utils.h"

int execute_action(tuya_mqtt_context_t *context, char *json_msg, char *device_id)
{
        syslog(LOG_INFO, "Executing action...");
        int ret = 0;
        int action = -1;

        char response[50] = {0};
        struct Params *params = (struct Params *)calloc(1, sizeof(struct Params));

        syslog(LOG_INFO, "Parsing json message...");
        ret = parse_json_message(json_msg, params);

        syslog(LOG_INFO, "Parsing action code...");
        action = parse_action(params);

        switch (action)
        {
        case ACTION_DEVICES:
                ret = devices(context, device_id);
                free(params);
                break;
        case ACTION_ON:
                ret = control_esp(params, response);
                response_handler(context, device_id, ret, response);
                free(params);
                break;
        case ACTION_OFF:
                ret = control_esp(params, response);
                response_handler(context, device_id, ret, response);
                free(params);
                break;
        case ACTION_INVALID:
                response_handler(context, device_id, ACTION_INVALID, response);
                free(params);
                break;
        default:
                syslog(LOG_INFO, "Default");
                free(params);
                break;
        }

        return ret;
}

int parse_action(struct Params *params)
{
        if (strcmp(params->action_code, "devices") == 0)
        {
                syslog(LOG_INFO, "Parsed action code %d", ACTION_DEVICES);
                return ACTION_DEVICES;
        }
        else if (strcmp(params->action_code, "on") == 0)
        {
                syslog(LOG_INFO, "Parsed action code %d", ACTION_ON);
                return ACTION_ON;
        }
        else if (strcmp(params->action_code, "off") == 0)
        {
                syslog(LOG_INFO, "Parsed action code %d", ACTION_OFF);
                return ACTION_OFF;
        }
        syslog(LOG_ERR, "Parsed action invalid!");
        return ACTION_INVALID;
}

int parse_json_message(char *json_msg, struct Params *params)
{
        cJSON *json_string = cJSON_Parse(json_msg);
        cJSON *input_params = cJSON_GetObjectItem(json_string, "inputParams");

        cJSON *port_json = cJSON_GetObjectItem(input_params, "port");
        cJSON *pin_json = cJSON_GetObjectItem(input_params, "pin");
        cJSON *action_json = cJSON_GetObjectItem(json_string, "actionCode");

        char *action = cJSON_GetStringValue(action_json);
        syslog(LOG_INFO, "Action - %s", action);

        strncpy(params->action_code, action, sizeof(params->action_code));

        if (port_json != NULL && pin_json != NULL)
        {
                char *port = cJSON_GetStringValue(port_json);
                char *pin = cJSON_GetStringValue(pin_json);

                syslog(LOG_INFO, "Port - %s", port);
                syslog(LOG_INFO, "Pin - %s", pin);

                strncpy(params->port, port, sizeof(params->port));
                strncpy(params->pin, pin, sizeof(params->pin));
        }

        cJSON_Delete(json_string);
        return 0;
}

char *construct_devices_json(struct Devices *devices)
{
        cJSON *json_arr = cJSON_CreateArray();

        for (int i = 0; i < 10; i++)
        {
                if (*devices[i].port == 0)
                {
                        break;
                }

                cJSON *json_obj = cJSON_CreateObject();
                cJSON_AddStringToObject(json_obj, "port", devices[i].port);
                cJSON_AddStringToObject(json_obj, "pid", devices[i].product_id);
                cJSON_AddStringToObject(json_obj, "vid", devices[i].vendor_id);
                cJSON_AddItemToArray(json_arr, json_obj);
        }

        cJSON *array = cJSON_CreateObject();
        cJSON_AddItemToObject(array, "devices", json_arr);

        char *json_str = cJSON_Print(array);

        cJSON_Delete(array);
        return json_str;
}

int devices(tuya_mqtt_context_t *context, char *device_id)
{
        struct Devices devices[10] = {0};

        struct ubus_context *ctx;

        ctx = ubus_connect(NULL);
        if (!ctx)
        {
                syslog(LOG_ERR, "ERROR: failed to connect to ubus");
                return -1;
        }

        syslog(LOG_INFO, "Recieving devices data from ubus...");
        get_devices_array(&ctx, devices);
        syslog(LOG_INFO, "Data recieved from ubus!");

        char *data = construct_devices_json(devices);

        tuyalink_thing_property_report(context, device_id, data);

        free(data);

        ubus_free(ctx);

        return 0;
}

int control_esp(struct Params *params, char *response)
{
        struct ubus_context *ctx;

        ctx = ubus_connect(NULL);
        if (!ctx)
        {
                syslog(LOG_ERR, "ERROR: failed to connect to ubus");
                return -1;
        }

        int ret = 0;

        syslog(LOG_INFO, "Sending action to esp...");
        ret = esp_send_action(&ctx, params->action_code, response, params[0].port, params[0].pin);

        syslog(LOG_INFO, "Action sent!");
        ubus_free(ctx);

        return ret;
}

void response_handler(tuya_mqtt_context_t *context, char *device_id, int rsp_code, char *rsp_msg)
{
        char rsp[50];
        switch (rsp_code)
        {
        case ACTION_INVALID:
                sprintf(rsp, "{\"response\":%d,\"msg\":\"Invalid action!\"}", rsp_code);
                syslog(LOG_ERR, "Response: %s", rsp);
                tuyalink_thing_property_report(context, device_id, rsp);
                break;
        case UBUS_STATUS_INVALID_ARGUMENT:
                sprintf(rsp, "{\"response\":%d,\"msg\":\"Invalid arguments!\"}", rsp_code);
                syslog(LOG_ERR, "Response: %s", rsp);
                tuyalink_thing_property_report(context, device_id, rsp);
                break;
        case -1:
                sprintf(rsp, "{\"response\":%d,\"msg\":\"Can not find ubus device!\"}", rsp_code);
                syslog(LOG_ERR, "Response: %s", rsp);
                tuyalink_thing_property_report(context, device_id, rsp);
                break;
        case 0:
                strcpy(rsp, rsp_msg);
                syslog(LOG_INFO, "Response: %s", rsp);
                tuyalink_thing_property_report(context, device_id, rsp);
                break;
        default:
                break;
        }
}
