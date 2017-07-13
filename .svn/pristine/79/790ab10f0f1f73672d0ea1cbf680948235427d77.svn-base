/*
 * lib_OEM_MODBUS.c
 *
 *  Created on: Jun 20, 2017
 *      Author: sushshring
 */


#include "lib_OEM_MODBUS.h"


int Modbus_connect_TCP(modbus_t *ctx, char *IP, int port) {
	ctx = modbus_new_tcp(IP, port);
	if (ctx == NULL) {
		perror("Could not allocate MODBUS context\n");
		return -1;
	}
	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		return -1;
	}
	return 0;
}

int Modbus_listen_TCP(modbus_t *ctx, char *IP, int port) {
	ctx = modbus_new_tcp(IP, port);
	if (ctx == NULL) {
		perror("Could not allocate MODBUS context\n");
		return -1;
	}
	return modbus_tcp_listen(ctx, 10);
}

int Modbus_send_data(modbus_t *ctx, void *buf, unsigned short size) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	int socket_fd = 0;
	int written_bytes = 0;


	socket_fd = modbus_get_socket(ctx);
	if (socket_fd == 0) {
		perror("Could not get socket from context\n");
		return -1;
	}

	while (written_bytes < size) {
		written_bytes = write(socket_fd, buf+written_bytes, size);
	}
	return written_bytes;
}

int Modbus_receive_data(modbus_t *ctx, void *buf, unsigned short size) {
	//
		//
		//LOCAL VARIABLES
		//
		//
		int socket_fd = 0;
		int read_bytes = 0;


		socket_fd = modbus_get_socket(ctx);
		if (socket_fd == 0) {
			perror("Could not get socket from context\n");
			return -1;
		}

		while (read_bytes < size) {
			read_bytes = read(socket_fd, buf+read_bytes, size);
		}
		return read_bytes;
}

void Modbus_disconnect_TCP(modbus_t *ctx) {
	modbus_close(ctx);
	modbus_free(ctx);
}
