/*
 * lib_OEM_GSM.h
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */

#ifndef lib_OEM_GSM_INCLUDED
#define lib_OEM_GSM_INCLUDED

//#defines to quickly find the physical /dev location the port is connected
#include "rs232.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#ifndef __gsm_baud_defined
#define __gsm_baud_defined
#define __gsm_baud 115200
#endif

#ifndef __gsm_mode_defined
#define __gsm_mode_defined
#define __gsm_mode "8N1"
#endif

#ifndef __gsm_maxbuffer_defined
#define __gsm_maxbuffer_defined
#define __GSM_MAX_BUFFER 558
#endif

#ifndef __gsm_waittime_defined
#define __gsm_waittime_defined
#define __GSM_WAIT_TIME 4
#endif

extern int gsm_portnr;

/**
* FUNCTION: GSM_init_connection(port_name)
* This function will initialize the connection to the
* UART port and start up the GSM module.
*
* PARAMETERS: port_name - The '/dev/<path>' string to which the
* 						  serial port is connected. Ex: ttyS0, ttyUSB0, COM2
* RETURNS: File descriptor of port is successful, -1 if failed.
**/
int GSM_init_connection( char *port_name);


/**
 * FUNCTION: GSM_configure(APN, username, password)
 * This function will configure the GSM connection with
 * the APN, username, and password as required by the
 * carrier
 *
 * PARAMETERS: APN - A char string containing the APN
 * 			   username - A char string containing the username
 * 			   password - A char string containing the password
 * RETURNS: 0 is successful, -1 if failed.
 */
int GSM_configure(char *APN, char *username, char *password);


/**
 * FUNCTION: GSM_get_IP(ret: IP)
* This function will return the current IP of the module
* in IP
*
*
* PARAMETERS: allocated string IP to return IP value in
* RETURN: 0 if successful, -1 if failed.
**/
int GSM_get_IP(char *IP);


/**
* FUNCTION: GSM_get_mac(ret: mac)
* This function will return the physical MAC address
* of the module in mac
* PARAMETERS: allocated string mac to returnmac address value
* RETURNS: 0 if successful, -1 if failed
**/
//int GSM_get_mac(char *mac);


/**
* FUNCTION: GSM_connect_TCP(IP, port)
* This function will connect the module to an IP with TCP
*
* PARAMETERS: IP - char string which contains the IP address
* 					of destination
* 			  port - port number to connect to on remote IP
**/
int GSM_connect_TCP(char *IP, int port);


/**
* FUNCTION: GSM_disconnect_TCP( void )
* This function will disconnect the module from the port it is connected to
*
* PARAMETERS: None
* RETURNS: 0 is successful, -1 if failure
**/
int GSM_disconnect_TCP( void );


/**
* FUNCTION: GSM_send_data(buf, size)
* This function will send the data in buf to the connected IP.
* buf must be 'size' big
*
* PARAMETERS: buf - buffer containing data
* 			  size - size of buffer
* RETURNS: -1 if failure, number of bytes written if succcess
**/
int GSM_send_data(void *buf, int size);


/**
* FUNCTION: GSM_read_data(ret: buf, size)
* NOTE: size cannot exceed 1460 bytes
*
* This function will read in data from the module into buf
*
* PARAMETERS: buf - buffer to write data into
* 			  size - size of data to read. MAX: 1460
* RETURNS: -1 if failure. number of bytes written is success
**/
int GSM_read_data(void *buf, int size);


/**
* FUNCTION: GSM_reset (void)
* Resets the wifi module to a blank state.
*
* PARAMETERS: None
* RETURNS: -1 if failure. 0 if success
**/
int GSM_reset( void );


/**
 * FUNCTION: GSM_disconnect( void );
 * Disconnects from the module and cleans up
 *
 * PARAMETERS: None
 * RETURNS: -1 if failure. 0 if success
 */
int GSM_disconnect( void );

/**
 *
 */
int GSM_custom_command(char *in, size_t insize, char *response, size_t outsize);

#endif
