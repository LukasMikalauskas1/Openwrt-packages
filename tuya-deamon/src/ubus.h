#ifndef UBUS_H
#define UBUS_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>

struct MemData
{
        int total;
        int free;
        int shared;
        int buffered;
        int available;
        int cached;
};

enum
{
        TOTAL_MEMORY,
        FREE_MEMORY,
        SHARED_MEMORY,
        BUFFERED_MEMORY,
        AVAILABLE_MEMORY,
        CACHED_MEMORY,
        MEMORY_LENGHT,
};

enum
{
        MEMORY_DATA,
        INFO_LENGHT,
};

/*
 * @brief Function that gets and stores memory information using ubus
 * @param memory a pointer to struct that stores memory data
 * @param ctx a pointer to ubus context whitch is used for ubus communication
 * @return 0 - succses, -1 - failed to get info from ubus
 */
int ubus_get_mem_info(struct MemData *memory, struct ubus_context *ctx);
/*
 * @brief Function that is used to dissconect from ubus and free resources
 * @param ctx a pointer to ubus context whitch is used to disconect from
 */
void ubus_disconnect(struct ubus_context *ctx);

#endif