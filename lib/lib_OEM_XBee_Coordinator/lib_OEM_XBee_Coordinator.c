/*
 * lib_OEM_XBee_Coordinator.c
 *
 *  Created on: Jun 5, 2017
 *      Author: sushshring
 */
#include "lib_OEM_XBee_Coordinator.h"

#ifndef __xbee_portnr_defined
#define __xbee_portnr_defined
int xbee_portnr = 0;
#endif

int switch_to_command_mode() {
	//
	//
	//LOCAL VARIABLES
	//
	char *AT_Command = (char *) malloc(__xbee_bufsize);
	char *AT_Response = (char *) malloc(__xbee_bufsize);

	if (xbee_portnr == 0) {
		perror("Module not connected");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __xbee_bufsize, "+++");
	if (RS232_SendBuf(xbee_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not set to command mode");
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	return 0;
}

int switch_to_idle_mode() {
	//
		//
		//LOCAL VARIABLES
		//
		char *AT_Command = (char *) malloc(__xbee_bufsize);
		char *AT_Response = (char *) malloc(__xbee_bufsize);

		if (xbee_portnr == 0) {
			perror("Module not connected");
			free (AT_Command);
			free(AT_Response);
			return -1;
		}
		snprintf(AT_Command, __xbee_bufsize, "ATCN\r");
		if (RS232_SendBuf(xbee_portnr, AT_Command, strlen(AT_Command)) == 1) {
			perror("Could not set to command mode");
			return -1;
		}
		sleep(__XBEE_WAIT_TIME);
		if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
			perror("Reading response failed. Reason: ");
			printf("Response: %s\n", AT_Response);
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
		if (strncmp(AT_Response, "ERROR", 6) == 0) {
			perror("Could not set to command mode\n");
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
		return 0;
}

int XBEE_init_connection(char *port, char *node_identifier, char *pan_id) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	char *command = (char *)malloc(__xbee_bufsize);
	char *AT_Response = (char *) malloc(__xbee_bufsize);
	int xbee_fd = 0;
	xbee_portnr = RS232_GetPortnr(port);

	if (xbee_portnr == -1) {
		perror("Port not found");
		free(command);
		free(AT_Response);
		return -1;
	}
	if ((xbee_fd = RS232_OpenComport(xbee_portnr, __XBEE_BAUDRATE, __XBEE_MODE)) == 1) {
		perror("Could not open port. Reason: ");
		free(command);
		free(AT_Response);
		return -1;
	}
	//Configure the device to run in command mode
	if (switch_to_command_mode() == -1) {
		free(command);
		free(AT_Response);
		return -1;
	}
	snprintf(command, __xbee_bufsize, "ATNI%s,SD0,NJ0xFF,WR\r", node_identifier);
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(command);
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		free(command);
		printf("Response: %s\n", AT_Response);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(command);
		free(AT_Response);
		return -1;
	}
	snprintf(command, __xbee_bufsize, "ATSC\r");
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(command);
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		free(command);
		printf("Response: %s\n", AT_Response);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(command);
		free(AT_Response);
		return -1;
	}
	char sc[3];
	snprintf(sc, 3, AT_Response);
	snprintf(command, __xbee_bufsize, "ATSC%s,WR\r",sc );
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(command);
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		free(command);
		printf("Response: %s\n", AT_Response);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(command);
		free(AT_Response);
		return -1;
	}
	snprintf(command, __xbee_bufsize, "ATID\r");
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(command);
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(command);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(command);
		free(AT_Response);
		return -1;
	}
	memcpy(pan_id, AT_Response, strlen(AT_Response));
	free(command);
	free(AT_Response);
	if (switch_to_idle_mode() == -1) {
		perror("Module not in data mode");
		return -1;
	}
	return xbee_fd;
}

int XBEE_send_data(char *destination_id, void *buf, size_t length) {
	//
	//
	//ERROR CHECKING
	//
	//
	if (xbee_portnr == 0) {
		perror("Port not configured\n");
		return -1;
	}
	if (strlen(destination_id) > 20) {
		perror("Destination id too long");
		return -1;
	}
	//
	//LOCAL VARIABLES
	//
	//
	char *AT_Command = (char *) malloc(__xbee_bufsize);
	char *AT_Response = (char *) malloc(__xbee_bufsize);

	if (switch_to_command_mode() == -1) {
		return -1;
	}

	snprintf(AT_Command, __xbee_bufsize, "ATDN%s\r", destination_id);
	if (RS232_SendBuf(xbee_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(AT_Response);
		return -1;
	}
	if (RS232_SendBuf(xbee_portnr, buf, length) == 1) {
		perror("Could not send data");
		free(AT_Response);
		return -1;
	}
	return 0;
}

/**
 *
 */
int XBEE_read_data(void *buf, size_t length) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Response = (unsigned char *) malloc(__xbee_bufsize);

	if (xbee_portnr == 0) {
		perror("Port is not configured");
		free(AT_Response);
		return -1;
	}
	if (RS232_PollComport(xbee_portnr, buf, length) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", (unsigned char *) buf);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", (unsigned char *) buf);
	memcpy(buf, AT_Response, strlen(AT_Response));
	free(AT_Response);
	return 0;
}


int XBEE_close_connection() {
	//
	//
	//LOCAL VARIABLES
	//
	char *command = "ATNR0\r";
	char *AT_Response = (char *) malloc(__xbee_bufsize);

	if (switch_to_command_mode() == -1) {
		return -1;
	}
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not initialize in command mode. Reason: ");
		free(AT_Response);
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, AT_Response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Response);
		return -1;
	}
	if (strncmp(AT_Response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		free(AT_Response);
		return -1;
	}
	RS232_CloseComport(xbee_portnr);
	xbee_portnr = 0;
	free(AT_Response);
	return 0;
}

/**
 *
 */
int XBEE_custom_command(char *command, char *response) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	if (xbee_portnr == 0) {
		perror("Port not configured");
		return -1;
	}
	if (switch_to_command_mode() == -1) {
		return -1;
	}
	if (RS232_SendBuf(xbee_portnr, command, strlen(command)) == 1) {
		perror("Could not send command");
		return -1;
	}
	sleep(__XBEE_WAIT_TIME);
	if (RS232_PollComport(xbee_portnr, response, __xbee_bufsize) < 0) {
		perror("Reading response failed. Reason: ");
		return -1;
	}
	if (strncmp(response, "ERROR", 6) == 0) {
		perror("Could not set to command mode\n");
		return -1;
	}
	if (switch_to_idle_mode() == -1){
		return -1;
	}
	return 0;
}




















