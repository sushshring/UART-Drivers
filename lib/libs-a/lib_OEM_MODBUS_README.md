This is a driver for the OEM MODBUS module 
Developed on Ubuntu 16.04 with gcc.
Tested on Ubuntu 16.04 with gcc.

To use this driver, make sure that the libmodbus_dev library has been installed with apt 

To start, connect to an IP with the Modbus_connect_TCP function. The library uses sockets to communicate
with remote ports, so once a connection has been initialized, it can be read and written to easily
All data related to the connection is stored in a context structure of type modbus_t

CONNECTION
To connect to a remote port, call the connect_TCP function with the remote IP and port. In order to open a
port on the device itself for data receiving, call the list_TCP function.



/**
 * FUNCTION: Modbus_connect_TCP(context[ret], IP, port)
 * This function will create a new TCP connection on the specified IP address
 * and port number, and return the connection parameters in the allocated
 * context object
 * PARAMETERS: ctx - An allocated context object to return connection details in
 * 	       IP - the IP address to connect to
 * 	       port - the port number to connect to on the specified IP
 * RETURNS: 0 if successful, -1 if failed
 */
int Modbus_connect_TCP(modbus_t *ctx, char *IP, int port);

/**
 * FUNCTION: Modbus_listen_TCP(context, IP, port)
 * This function will create a new TCP connection on the specified IP address
 * and port number, and return the connection parameters in the allocated
 * context object
 * PARAMETERS: ctx - An allocated context object to return connection details in
 * 	       IP - the IP address to connect to
 * 	       port - the port number to connect to on the specified IP
 * RETURNS: 0 if successful, -1 if failed
 */
int Modbus_listen_TCP(modbus_t *ctx, char *IP, int port);

/**
 * FUNCTION: Modbus_send_data(context, buffer, size)
 * This function sends data to the connected IP as listed in context
 * PARAMETERS: ctx - A modbus_t structure that has already been initialized
 * 			with Modbus_connect_TCP
 * 	       buf - A buffer of data to send
 * 	       size - Size of the buffer
 * RETURNS: 0 if successful, -1 if failed
 */
int Modbus_send_data(modbus_t *ctx, void *buf, unsigned short size);

/**
 * FUNCTION: Modbus_receive_data(ctx, buf, size)
 * This function polls for data on the port and IP listed by ctx
 * PARAMETERS: ctx - A modbus_t structure that has already been initialized
 * 			with Modbus_listen_TCP
 * 	       buf - An allocated empty buffer to write data to
 * 	       size - the maximum size of the buffer
 * RETURNS: 0 if successful, -1 if failed
 */
int Modbus_receive_data(modbus_t *ctx, void *buf, unsigned short size);

/**
 * FUNCTION: Modbus_disconnect_TCP(ctx)
 * This function disconnects from the connected TCP context
 * PARAMETERS: ctx - A modbus_t structire that has already been initialized
 * 			with Modbus_connect_TCP of Modbus_listen_TCP
 * RETURNS: 0 is successful, -1 if failure
 */
void Modbus_disconnect_TCP(modbus_t *ctx);
