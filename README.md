First task. Connecting the router to the Tuya IoT cloud

This task is intended to help consolidate your acquired theoretical knowledge in practice. It will also help you better to understand the structure of the OpenWRT system, create new packages for it and apply new software.

For this task you will need to change your Tuya IoT daemon program so that it would work on the RutOS system. 

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

Task result:

    A daemon type program has been prepared that will send data to the Tuya IoT cloud.
    Founded library or SDK that will be used to communicate with the Tuya IoT cloud. The library/SDK is prepared as a separate package.
    A web interface is prepared through which the program will be managed and only the most necessary input fields are provided to configure the program.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

A total of three different packages must be prepared:

    A daemon type program is responsible for communicating with Tuya IoT cloud
    A library/SDK which will be used to communicate and send data to Tuya IoT cloud
    WEB interface
