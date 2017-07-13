/*
 * lib_OEM_MODBUS.h
 * This library is for use with a Modbus ethernet connectin
 * The standard modbus library must be installed with
 * apt-get install libmodbus-dev
 *
 *  Created on: Jun 20, 2017
 *      Author: sushshring
 */

#ifndef LIB_OEM_MODBUS_H_
#define LIB_OEM_MODBUS_H_

#include <modbus/modbus-tcp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>

typedef struct _modbus modbus_t;

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






#endif /* LIB_OEM_MODBUS_H_ */
