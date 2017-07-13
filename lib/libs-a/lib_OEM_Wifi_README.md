This is a driver for the KM22 Wifi module.
Developed on Ubuntu 16.04 with gcc.
Tested on Ubuntu 16.04 with gcc.

To use this driver, connect the Wifi module with a UART connection to one of the serial ports on the computer
Make note of the serial port or run discovery through the /dev/ path. The port name should be similar to COM0,
COM1, COM2, ... on Windows machines, and ttyS0, ttyS1, ... on UNIX machines. If a USB-Serial device is used,
the port names are generally ttyUSB0, ttyUSB1, ...

In your application, first call init_connection() with the port name as the parameter. Without calling this 
function, all other function calls will fail. 
NOTE: This driver requests a lock on the port, so any other application
will not be able to access this port concurrently. Any requests written to this port will have to be made through
your application.

INITIALIZATION
To connect the module to an access point, use the setup_AP function. Parameters are SSID and password.

STATUS
There are two getter functions implemented in this driver:
	get_IP: This returns the IP address of the module on the local network
	get_MAC: This returns the physical MAC address associated with the module

CONNECTION
To connect to a remote port, call the connect_TCP function with the remote IP and port. In order to open a
port on the device itself for data receiving, call the start_TCP_Client function.

DATA MODES:
	There are two data modes supported on this module: Command and Pure Data mode

	1) Command mode: This mode buffers data and transmits it to the remote port 100 bytes at a time.
			After each write, the library waits a certain amount of time before sending the 
			next data portion. This mode is around four times slower than Pure Data mode, but
			it supports further commands after the data send.
	2) Pure Data mode: Once the module is put into this mode, there is no way of returning to a command
			state. It will listen for data on any ports it is listen on and it will broadcast
			data to all the remote servers it is connected to. In order to leave this mode,
			the module will have to be physically restarted. Data transmits at 512 bytes in each
			buffer cycle.
			If using the Pure Data mode, make sure all ports that the device needs to listen to
			are configured and all addresses that it will be sending data to are initialized.
	To select the data mode, pass the appropriate argument as a flag during init_connection

CUSTOM COMMANDS
The driver also supports sending custom commands to the module. Refer to the KM22 datasheet for a full list
of AT commands and their structure. Note that the device will not register an AT command if it is in Pure 
Data mode.

RECEIVING DATA
The application is expected to implement an ISR routine that will be triggered on the appropriate interrupt.
Note that the get_IP and get_Mac functions have not been tested alongside an ISR and may trigger the interrupt
when the data is returned by the module. 

CLEANUP
For disconnection, make sure that the Wifi_disconnect function is called. This frees the serial port and resets
the module. To use the module again, it has to be reconfigured with the ports and AP settings.

Author: Sushrut Shringarputale
Date: June 12, 2017
For: SmartLeaven Technologies, Inc.






/**
* FUNCTION: init_connection( char *port_name, int flag )
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
* Switch DHCP mode to static IP connection.
*
* PARAMETERS: mode - defined DHCP_MODE or STATIC_MODE
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
