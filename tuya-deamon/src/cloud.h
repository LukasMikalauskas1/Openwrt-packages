#ifndef CLOUD_H_
#define CLOUD_H_

#include "tuyalink_core.h"
#include "argparse.h"

/*
 * @brief Function that connects to tuya Iot cloud
 * @param context a pointer to tuya mqtt context
 * @param args a struct of program arguments
 * @return error code
 */
int cloud_connect(tuya_mqtt_context_t *context, struct arguments args);
/*
 * @brief Function that disconnects from tuya Iot cloud
 * @param context a pointer to tuya mqtt context
 */
void cloud_disconect(tuya_mqtt_context_t *context);
/*
 * @brief Function that executes upon conncetion to cloud (used for loging)
 */
void on_connected(tuya_mqtt_context_t *context, void *user_data);
/*
 * @brief Function that executes upon disconnect from cloud (used for loging)
 */
void on_disconnect(tuya_mqtt_context_t *context, void *user_data);
/*
 * @brief Function that executes upon message (used for loging)
 */
void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg);
/*
 * @brief Function that is used to execute an action sent from cloud
 * @param msg a pointer to the message
 */
void exe_action(const tuyalink_message_t *msg);
/*
 * @brief Function that parses message from cloud
 * @param string a message string to parse 
 * @return a string of parsed data 
 */
char *parse_data(char *string);
/*
 * @brief Function that sends a message to tuya iot cloud
 * @param context tuya mqtt context
 * @param args a struct of program arguments
 * @param report a string that will be sent to cloud
 * @return a string of parsed data 
 */
void send_report(tuya_mqtt_context_t *context, struct arguments args, char *report);
/*
 * @brief Function that gets a path to a file where daemon will write messages recieved from cloud
 * @param file_name a string that stores desired file name
 * @return a full path to the file
 */
char *get_file_path(char *file_name);

#endif