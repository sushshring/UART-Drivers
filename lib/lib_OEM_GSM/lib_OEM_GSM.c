/*
 * lib_OEM_GSM.c
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */

#include "lib_OEM_GSM.h"
#ifndef __gsm_portnr_defined
#define __gsm_portnr_defined
int gsm_portnr = 0;
#endif


int GSM_init_connection ( char *port_name) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	int gsm_fd = 0;

	if ((gsm_portnr = RS232_GetPortnr(port_name)) == -1) {
		perror("Port not found");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}

	if ((gsm_fd = RS232_OpenComport(gsm_portnr, __gsm_baud, __gsm_mode)) == -1) {
		perror("Could not open port. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}

	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT\r");
#ifdef debug
	printf("AT COMMAND: %s\n", AT_Command);
#endif
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response);
#endif
	strcpy(AT_Command, "AT+CGATT=1\r");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
			perror("Could not send command. Reason: ");
			free(AT_Command);
			free(AT_Response);
			return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Reason: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	if (strcmp(AT_Response+strlen(AT_Command)+2,"ERROR") == 13) {
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	strcpy(AT_Command, "AT+CGACT=1,1\r");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
			perror("Could not send command. Reason: ");
			free(AT_Command);
			free(AT_Response);
			return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Reason: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	if (strcmp(AT_Response+strlen(AT_Command)+2,"ERROR") == 13) {
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	free(AT_Command);
	free(AT_Response);
	return gsm_fd;
}

int GSM_configure(char *APN, char *username, char *password) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (char *)malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, 'AT+CSTT="%s","%s","%s"\r', APN, username, password);
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	snprintf(AT_Command, __GSM_MAX_BUFFER, 'AT+CGDCONT=1,"IP","%s"\r', APN);
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response);
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_get_IP(char *IP) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT+CIFSR\r");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_connect_TCP(char *IP, int port) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	strcpy(AT_Command, "AT+CGACT=1,1\r");
		if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
				perror("Could not send command. Reason: ");
				free(AT_Command);
				free(AT_Response);
				return -1;
		}
		sleep(__GSM_WAIT_TIME);
		if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
			perror("Reading response failed. Reason: ");
			printf("Reason: %s\n", AT_Response);
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
		if (strcmp(AT_Response+strlen(AT_Command)+2,"ERROR") == 13) {
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT+CIPSTART=\"TCP\",\"%s\",%d\r", IP, port);
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_disconnect_TCP( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT+CIPCLOSE=0\r");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_send_data(void *buf, int size) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT+CIPSEND=%d\r", size);
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
	if (RS232_SendBuf(gsm_portnr, buf, size) == 1) {
		perror("Data send failed. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response);
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_read_data(void *buf, int size) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, buf, size) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", (unsigned char *) buf);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", (unsigned char *) buf);
#endif
	memcpy(buf, AT_Response, strlen(AT_Response));
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int GSM_reset( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "ATZ=0");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;

}

int GSM_disconnect( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__GSM_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__GSM_MAX_BUFFER);

	if (gsm_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __GSM_MAX_BUFFER, "AT+CIPSHUT=0\r");
	if (RS232_SendBuf(gsm_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__GSM_WAIT_TIME);
	if (RS232_PollComport(gsm_portnr, AT_Response, __GSM_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);

	RS232_CloseComport(gsm_portnr);
	gsm_portnr = 0;
	return 0;
}















