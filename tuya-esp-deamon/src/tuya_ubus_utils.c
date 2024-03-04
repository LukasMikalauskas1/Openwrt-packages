#include "tuya_ubus_utils.h"

static const struct blobmsg_policy array_policy[ARRAY_LENGHT] = {
    [ARRAY_DATA] = {.name = "devices", .type = BLOBMSG_TYPE_ARRAY},
};

static const struct blobmsg_policy device_policy[DEVICE_LENGHT] = {
    [DEVICE_PORT] = {.name = "port", .type = BLOBMSG_TYPE_STRING},
    [DEVICE_VID] = {.name = "vid", .type = BLOBMSG_TYPE_STRING},
    [DEVICE_PID] = {.name = "pid", .type = BLOBMSG_TYPE_STRING},
};

static const struct blobmsg_policy response_policy[3] = {
    [RESPONSE_CODE] = {.name = "response", .type = BLOBMSG_TYPE_INT32},
    [RESPONSE_MSG] = {.name = "msg", .type = BLOBMSG_TYPE_STRING},
};

static void devices_callback(struct ubus_request *request, int type, struct blob_attr *msg)
{
        struct Devices *devices = (struct Devices *)request->priv;

        struct blob_attr *array[ARRAY_LENGHT];
        struct blob_attr *table[DEVICE_LENGHT];

        blobmsg_parse(array_policy, ARRAY_LENGHT, array, blob_data(msg), blob_len(msg));

        if (array[ARRAY_DATA] == NULL)
        {
                syslog(LOG_ERR, "ERROR: no data recieved\n");
                return;
        }

        int rem;
        int counter = 0;
        struct blob_attr *current;

        blobmsg_for_each_attr(current, array[ARRAY_DATA], rem)
        {
                blobmsg_parse(device_policy, DEVICE_LENGHT, table, blobmsg_data(current), blobmsg_data_len(current));

                char *device_port = blobmsg_get_string(table[DEVICE_PORT]);
                char *device_vid = blobmsg_get_string(table[DEVICE_VID]);
                char *device_pid = blobmsg_get_string(table[DEVICE_PID]);

                strncpy(devices[counter].port, device_port, sizeof(devices->port));
                strncpy(devices[counter].vendor_id, device_vid, sizeof(devices->vendor_id) - 1);
                strncpy(devices[counter].product_id, device_pid, sizeof(devices->product_id) - 1);

                counter++;
        }
}

static void action_callback(struct ubus_request *request, int type, struct blob_attr *msg)
{
        char *response = (char *)request->priv;

        struct blob_attr *table[RESPONSE_LENGTH];

        blobmsg_parse(response_policy, RESPONSE_LENGTH, table, blob_data(msg), blob_len(msg));

        if (table[RESPONSE_CODE] == NULL)
        {
                syslog(LOG_ERR, "ERROR: no data recieved\n");
                return;
        }

        int code = blobmsg_get_u32(table[RESPONSE_CODE]);
        char *message = blobmsg_get_string(table[RESPONSE_MSG]);

        sprintf(response, "{\"response\":%d,\"msg\":\"%s\"}", code, message);
        syslog(LOG_INFO, "Ubus response - %s", response);
}

int get_devices_array(struct ubus_context **ctx, struct Devices *devices)
{
        uint32_t id;

        if (ubus_lookup_id(*ctx, "esp_controler", &id) != UBUS_STATUS_OK)
        {
                syslog(LOG_ERR, "ERROR: ubus failed to look up id");
                ubus_free(*ctx);
                return -1;
        }

        if (ubus_invoke(*ctx, id, "devices", NULL, devices_callback, devices, 3000) != UBUS_STATUS_OK)
        {
                syslog(LOG_ERR, "ERROR: failed to invoke method");
                return -1;
        }
        return 0;
}

int esp_send_action(struct ubus_context **ctx, char *action, char *response, char *port, char *pin)
{
        uint32_t id;
        int ret = 0;

        struct blob_buf buffer = {};

        blob_buf_init(&buffer, 0);

        ret = blobmsg_add_string(&buffer, "port", port);
        ret = blobmsg_add_u32(&buffer, "pin", atoi(pin));

        if (ubus_lookup_id(*ctx, "esp_controler", &id) != UBUS_STATUS_OK)
        {
                syslog(LOG_ERR, "ERROR: ubus failed to look up id");
                ubus_free(*ctx);
                return -1;
        }

        ret = ubus_invoke(*ctx, id, action, buffer.head, action_callback, response, 3000);

        blob_buf_free(&buffer);

        return ret;
}

struct blob_attr *construct_blob_msg(char *port, char *pin)
{
        struct blob_buf buffer = {};

        blob_buf_init(&buffer, 0);

        blobmsg_add_string(&buffer, "port", port);
        blobmsg_add_u32(&buffer, "pin", atoi(pin));

        return buffer.head;
}
