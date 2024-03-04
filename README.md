Third task. Implementing ESP devices control over Tuya platform

In this task you will need to combine your first and second task so that it will be possible to control ESP devices from Tuya platform by sending different actions.

If your Tuya clouds program is stored in GIT, create another branch which will represent a different version of your program.

In this task, you need to change Tuya program in that way, that it will only have actions to control ESP devices. Your previous data sending, and actions aren’t needed anymore. Your program should use UBUS to communicate between programs.

Tuya platform must have two or three actions, depends how you will implement the functionality. One is to get the list of active ESP devices and the other to send command to turn on or off the pin of the ESP device. When controlling the pins over the action, don’t forget to send the port of the ESP device which you want to control.

Success or failure/error messages must be returned back to Tuya platform.

Task result:

    Modified Tuya program which can communicate with ESP devices control program
    Two separate programs. One for Tuya and one for ESP devices
