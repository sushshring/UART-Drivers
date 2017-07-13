This is a driver for the SIM900A GSM module 
Developed on Ubuntu 16.04 with gcc.
Tested on Ubuntu 16.04 with gcc.

To use this driver, connect the GSM  module with a UART connection to one of the serial ports on the computer
Make note of the serial port or run discovery through the /dev/ path. The port name should be similar to COM0,
COM1, COM2, ... on Windows machines, and ttyS0, ttyS1, ... on UNIX machines. If a USB-Serial device is used,
the port names are generally ttyUSB0, ttyUSB1, ...

In your application, first call init_connection() with the port name as the parameter. Without calling this 
function, all other function calls will fail. Pass the required data mode as the flag parameter (SEE DATA MODES) 
NOTE: This driver requests a lock on the port, so any other application will not be able to access this port 
concurrently. Any requests written to this port will have to be made through
your application.

INITIALIZATION
To connect the module to an access point, use the configure() function. It takes the APN, and an optional
username and password as required by the carrier. 

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
	2) Pure Data mode: This mode will listen for data on any ports it is listening on and it will broadcast
			data to all the remote servers it is connected to. In order to leave this mode,
			the module will have to be physically restarted. Data transmits at 512 bytes in each
			buffer cycle.
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
For disconnection, make sure that the disconnect function is called. This frees the serial port and resets
the module. To use the module again, it has to be reconfigured with the ports and AP settings.

Author: Sushrut Shringarputale
Date: June 12, 2017
For: SmartLeaven Technologies, Inc.





/**
* FUNCTION: GSM_init_connection(port_name)
* This function will initialize the connection to the
* UART port and start up the GSM module.
*
* PARAMETERS: port_name - The '/dev/<path>' string to which the
* 						  serial port is connected.
* RETURNS: 0 if successful, -1 if failed.
**/
int GSM_init_connection( char *port_name );


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
* of the module in *mac
*
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
