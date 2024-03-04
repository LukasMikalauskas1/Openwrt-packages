#ifndef TUYA_UBUS_UTILS_H_
#define TUYA_UBUS_UTILS_H_

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <syslog.h>

struct Devices
{
        char port[50];
        char vendor_id[20];
        char product_id[20];
};

enum
{
        ARRAY_DATA,
        ARRAY_LENGHT
};

enum
{
        DEVICE_PORT,
        DEVICE_VID,
        DEVICE_PID,
        DEVICE_LENGHT

};

enum
{
        RESPONSE_CODE,
        RESPONSE_MSG,
        RESPONSE_LENGTH
};

/*Function that returns array of connceted devices*/
int get_devices_array(struct ubus_context **ctx, struct Devices *devices);

/*Function that send an action to esp controler*/
int esp_send_action(struct ubus_context **ctx, char *action, char *response, char *port, char *pin);

/*Function that constructs a message of ubus arguments*/
struct blob_attr *construct_blob_msg(char *port, char *pin);

#endif