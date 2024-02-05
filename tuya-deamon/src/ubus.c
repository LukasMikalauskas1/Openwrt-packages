#include "ubus.h"
#include <syslog.h>

static const struct blobmsg_policy memory_policy[MEMORY_LENGHT] = {
    [TOTAL_MEMORY] = {.name = "total", .type = BLOBMSG_TYPE_INT64},
    [FREE_MEMORY] = {.name = "free", .type = BLOBMSG_TYPE_INT64},
    [SHARED_MEMORY] = {.name = "shared", .type = BLOBMSG_TYPE_INT64},
    [BUFFERED_MEMORY] = {.name = "buffered", .type = BLOBMSG_TYPE_INT64},
    [AVAILABLE_MEMORY] = {.name = "available", .type = BLOBMSG_TYPE_INT64},
    [CACHED_MEMORY] = {.name = "cached", .type = BLOBMSG_TYPE_INT64},
};

static const struct blobmsg_policy info_policy[INFO_LENGHT] = {
    [MEMORY_DATA] = {.name = "memory", .type = BLOBMSG_TYPE_TABLE},
};

static void board_cb(struct ubus_request *request, int type, struct blob_attr *msg)
{
        struct MemData *memoryData = (struct MemData *)request->priv;
        struct blob_attr *table[INFO_LENGHT];
        struct blob_attr *memory[MEMORY_LENGHT];

        blobmsg_parse(info_policy, INFO_LENGHT, table, blob_data(msg), blob_len(msg));

        if (table[MEMORY_DATA] == NULL)
        {
                syslog(LOG_ERR, "ERROR: no memory data received");
                return;
        }

        blobmsg_parse(memory_policy, MEMORY_LENGHT, memory, blobmsg_data(table[MEMORY_DATA]), blobmsg_data_len(table[MEMORY_DATA]));

        memoryData->total = blobmsg_get_u64(memory[TOTAL_MEMORY]);
        memoryData->free = blobmsg_get_u64(memory[FREE_MEMORY]);
        memoryData->shared = blobmsg_get_u64(memory[SHARED_MEMORY]);
        memoryData->buffered = blobmsg_get_u64(memory[BUFFERED_MEMORY]);
        memoryData->available = blobmsg_get_u64(memory[AVAILABLE_MEMORY]);
        memoryData->cached = blobmsg_get_u64(memory[CACHED_MEMORY]);
}

void ubus_disconnect(struct ubus_context *ctx)
{
        syslog(LOG_INFO, "Disconecting from ubus...");
        ubus_free(ctx);
        syslog(LOG_INFO, "Disconected from ubus!");
}

int ubus_get_mem_info(struct MemData *memory, struct ubus_context *ctx)
{
        uint32_t id;

        if(ubus_lookup_id(ctx, "system", &id))
        {
                syslog(LOG_ERR, "ERROR: cannot request memory info from procd");
                return -1;
        }

        if(ubus_invoke(ctx, id, "info", NULL, board_cb, memory, 3000))
        {
                syslog(LOG_ERR, "ERROR: cannot request memory info from procd");
                return -1;
        }

        return 0;
}