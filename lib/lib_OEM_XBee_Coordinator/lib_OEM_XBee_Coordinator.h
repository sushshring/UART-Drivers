#ifndef LIB_OEM_XBEE_CONTROLLER_H_
#define LIB_OEM_XBEE_CONTROLLER_H_

#include "rs232.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef __XBEE_baudrate_defined
#define _XBEE_baudrate_defined
#define __XBEE_BAUDRATE 9600
#endif

#ifndef __XBEE_mode_defined
#define _XBEE_mode_defined
#define __XBEE_MODE "8N1"
#endif

#ifndef __XBEE_bufsize_defined
#define _XBEE_bufsize_defined
#define __xbee_bufsize 80
#endif

#ifndef __XBEE_waittime_defined
#define _XBEE_waittime_defined
#define __XBEE_WAIT_TIME 4
#endif

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
 * RETURNS: file descriptor for port if successful, -1 if failure
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





#endif /* LIB_OEM_XBEE_CONTROLLER_H_ */
