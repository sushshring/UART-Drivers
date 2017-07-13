/*
 * lib_OEM_Wifi.h
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */

#ifndef LIB_OEM_WIFI_H_
#define LIB_OEM_WIFI_H_

#include "rs232.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef __wifi_baud_defined
#define __wifi_baud_define
#define __wifi_baud 115200
#endif

#ifndef __wifi_mode_defined
#define __wifi_mode_defined
#define __wifi_mode "8N1"
#endif

#ifndef __wifi_waittime_defined
#define __wifi_waittime_defined
#define __WIFI_WAIT_TIME 4
#endif

#ifndef __wifi_maxbuffer_defined
#define __wifi_maxbuffer_defined
#define __WIFI_MAX_BUFFER 558
#endif

#ifndef __wifi_modes_defined
#define __wifi_modes_defined
#define DHCP_MODE 0
#define STATIC_MODE 1
#endif

extern int wifi_portnr;

//This variable can be used as an extern int to get access to the physical fd
//port number for the serial port


/**
* FUNCTION: init_connection( char *port_name , int flag)
* This function will initialize the connection to the
* UART port and start up the wifi module.
*
* PARAMETERS: port_name: The name of the serial port the device is connected to.
* 						Ex. ttyUSB0, ttyS2, etc.
* 			  flag: 0 if connect in buffered command, 1 if connect in Puredata mode.
* 			NOTE: Puredata mode can send 512 bytes of data at a time, but once
* 			this mode is triggered it cannot be reset until the device is rebooted physically
* 				buffered data mode sends data slower, but it will continue to respond to
* 				any further commands that are sent by the application.
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_init_connection( char *port_name, int flag);


/**
* FUNCTION: setup_AP(SSID, password)
* This function will connect the wifi module to the
* WiFi router with SSID and password
*
* PARAMETERS: SSID - char * containing the SSID of the network
* 			  password - char * containing the password of the network
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_setup_AP(char *SSID, char* password);


/**
* FUNCTION: get_IP(ret: IP)
* This function will return the current IP of the module
* in IP
*
* PARAMETERS: IP - allocated string to return the IP address is
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_get_IP(char *IP);


/**
* FUNCTION: change_DHCP_mode(mode, static_ip)
* Switch DHCP __wifi_mode to static IP connection.
*
* PARAMETERS: __wifi_mode - defined DHCP_MODE or STATIC_MODE
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_change_dhcp_mode(int conn_mode, char * static_ip);


/**
* FUNCTION: get_mac(ret: mac)
* This function will return the physical MAC address
* of the module in *mac
*
* PARAMETERS: mac - allocated char * to return mac address in
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_get_mac(char *mac);


/**
* FUNCTION: connect_TCP(IP, port)
* This function will connect the module to an IP with TCP
*
* PARAMETERS: IP - IP Address to connect to
* 			  port - port to connect to on remote IP
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_connect_TCP(char *IP, int port);


/**
 *
 *
 */
int Wifi_startTCP_Listen(int port);

/**
* FUNCTION: disconnect_TCP_Server(port)
* This function will disconnect the module from the port it is listening on
*
* PARAMETERS: port - port number that the module was connected to
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_stopTCP_Listen(int port);



/**
* FUNCTION: Wifi_send_data(buf, size)
* This function will send the data in buf to the connected IP.
* buf must be 'size' big
*
* PARAMETERS: buf - buffer of data to send
* 			  size - size of the buffer in bytes
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_send_data(void *buf, int size);


/**
* UNDEFINED
* This function will read in data from the module into buf
* Size is the size of the buffer. Returns the number of bytes
* written
**/
int Wifi_read_data(void *buf, int size);

/**
 *
 */
int Wifi_custom_command(char *command, size_t insize, char *response, size_t outsize);


/**
* FUNCTION: Wifi_reset ( void )
* Resets the wifi module to a blank state.
*
* PARAMETERS: none
* RETURNS: 0 if successful, -1 if failure
**/
int Wifi_reset( void );

/**
 * FUNCTION: Wifi_stop_connection( void )
 * This function disconnects the wifi module and performs cleanup
 *
 * PARAMETERS: None
 * Returns 0 is successful, -1 if failure
 */
int Wifi_stop_connection( void );


#endif /* LIB_OEM_WIFI_H_ */
