This is a driver for the XBee Zigbee module 
Developed on Ubuntu 16.04 with gcc.
Tested on Ubuntu 16.04 with gcc.

To use this driver, connect the Zigbee module with a UART connection to one of the serial ports on the computer
Make note of the serial port or run discovery through the /dev/ path. The port name should be similar to COM0,
COM1, COM2, ... on Windows machines, and ttyS0, ttyS1, ... on UNIX machines. If a USB-Serial device is used,
the port names are generally ttyUSB0, ttyUSB1, ...

In your application, first call init_connection() with the port name as the parameter. The function also requires
a second parameter, that will uniquely identify the coordinator in the PAN. All data transmission will only identify
the coordinator using this identifier. Without calling this function, all other function calls will fail.
NOTE: This driver requests a lock on the port, so any other application will not be able to access this port 
concurrently. Any requests written to this port will have to be made through
your application.

SENDING DATA
To send data to a node on the PAN, use the send_data() function. Identify the node you will be using with the identifier
used to setup the node. Pass this as a string in the destination_id parameter. The other two parameters are the buffer
of data, and the size of the buffer.

CUSTOM COMMANDS
The driver also supports sending custom commands to the module. Refer to the datasheet for a full list
of AT commands and their structure. 

RECEIVING DATA
The application is expected to implement an ISR routine that will be triggered on the appropriate interrupt.
After the interrupt is registered, use the read_data() function to read the actual data from the port.

CLEANUP
For disconnection, make sure that the disconnect function is called. This frees the serial port and resets
the module. To use the module again, it has to be reconfigured with the ports and AP settings.

In case the port number for the serial port is required, it will be stored in the xbee_portnr variable. Access it
as an extern variable after the init_connection command is called.
extern int xbee_portnr;

//Library functions for configuring and working with the XBee Coordinator


/**
 * FUNCTION: XBEE_init_connection: Configures the connection to the XBEE coordinator
 * PARAMS: port: String - name of the port the device is connected to (ttyS0, etc.)
 * 		   node_identifier: String - An identifier to associate with the module for
 * 									network discovery
 * 		   pan_id: ret String - The id of the created PAN is returned in this
 * RETURNS: 0 if successful, -1 if failure
 *
 */
int XBEE_init_connection(char *port, char *node_identifier, char *pan_id);

/**
 * FUNCTION: XBEE_send_data: Sends a buffer of data to the node with destination_id as the Node Identifier
 * PARAMS: destination_id: String - The node identifier for the destination node
 * 		   buf: void - Buffer of data to send. Buffer limit is the UART buffer limit
 * 		   length: int - length of the buffer
 * RETURNS: 0 if successful, -1 if failure
 */
int XBEE_send_data(char *destination_id, void *buf, size_t length);


/**
 * FUNCTION: XBEE_receive_data: Manually polls for any data. This should not be used instead of an ISR
 * PARAMS: buf: ret void: An allocated buffer to return data to
 * 		   lenth: int - the size of the allocated buffer
 *
 * RETURNS: 0 if successful, -1 if failure
 *
 */
int XBEE_read_data(void *buf, size_t length);


/**
 * FUNCTION: XBEE_close_connection: Closes the connection to the XBEE coordinator and reset it
 *
 * RETURNS: 0 if successful, -1 if failure
 */
int XBEE_close_connection();

/**
 * FUNCTION: XBEE_custom_command: Sends a custom AT command to the XBEE device
 * PARAMS: command: String - The AT command to send as per the datasheet
 * 		   response: ret String - String to write the repsonse from the device to
 *
 * 	RETURNS: 0 if successful, -1 if failure
 */
int XBEE_custom_command(char *command, char *response);
