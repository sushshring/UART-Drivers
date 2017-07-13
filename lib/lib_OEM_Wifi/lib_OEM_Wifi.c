/*
 * lib_OEM_Wifi.c
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */

#include "lib_OEM_Wifi.h"


#ifndef __wifi_datamode_defined
#define __wifi_datamode_defined
int __wifi_datamode = 0;
#endif

#ifndef __wifi_modeset_defined
#define __wifi_modeset_defined
int __wifi_modeset = 0;
#endif

#define __wifi_puredatabuf 500
#define __wifi_atbuf 100

#ifndef __wifi_portnr_defined
#define __wifi_portnr_defined
int wifi_portnr = 0;
#endif

int Wifi_init_connection( char *port_name , int flag) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	int wifi_fd = 0;

	if (wifi_portnr) {
		perror("Already initialized.");
		return -1;
	}

	if ((wifi_portnr = RS232_GetPortnr(port_name)) == -1) {
		perror("Port not found");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}

	if ((wifi_fd = RS232_OpenComport(wifi_portnr, __wifi_baud, __wifi_mode)) == 1) {
		perror("Could not open port. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}

	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Uartcfg -b%d -w%c -p0 -s%c\r", __wifi_baud, __wifi_mode[0], __wifi_mode[2]);
#ifdef debug
	printf("Created AT command\n");
	printf("AT COMMAND: %s\n", AT_Command);
#endif
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response);
#endif
	free(AT_Command);
	free(AT_Response);
	__wifi_datamode = flag;
	return wifi_fd;
}

int Wifi_setup_AP(char *SSID, char* password) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Conn_AP -s%s -p%s\r", SSID, password);
#ifdef debug
	printf("AT Command: %s\n", AT_Command);
#endif

	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Connflag\r");
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);

	if (RS232_PollComport(wifi_portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
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


int Wifi_get_IP(char *IP) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#GetIP\r");
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
	if (RS232_PollComport(wifi_portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	memcpy(IP, AT_Response, strlen(AT_Response));
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_change_dhcp_mode(int conn_mode, char *static_ip) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	switch (conn_mode) {
	case DHCP_MODE:
		snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Smtconn -d1\r");
		break;
	case STATIC_MODE:
		snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#StaticIP -i%s -m255.255.255.0 -g192.168.0.1 -d192.168.0.1\r", static_ip);
		break;
	}

	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_get_mac(char *mac) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Mac\r");
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
	if (RS232_PollComport(wifi_portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
#ifdef debug
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	memcpy(mac, AT_Response, strlen(AT_Response));
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_connect_TCP(char *IP, int port) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Uart_Wifi -n0 -m1 -i%s -r%d\r",IP, port);
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_startTCP_Listen(int port) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Uart_Wifi -n0 -m0 -l%d\r",port);
	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_stopTCP_Listen(int port) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Tcp_Unlisten -p%d\r",port);

	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}
int Wifi_send_data(void *buf, int size) {
	//
	//
	//LOCAL VARIABLES
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *) malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}

	if (!__wifi_modeset) {
		if (__wifi_datamode) {
		snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Puredata\r"); //TODO: Figure out if buffering of data is needed
#ifdef debug
		printf("AT Command: %s\n", AT_Command);
#endif
		if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
			perror("Could not send command. Reason: ");
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
		sleep(__WIFI_WAIT_TIME);
	#ifdef debug
		if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
			perror("Reading response failed. Reason: ");
			printf("Response: %s\n", AT_Response);
			free(AT_Command);
			free(AT_Response);
			return -1;
		}
		printf("Response: %s\n", AT_Response);
	#endif
		}
	__wifi_modeset = 1;

	}
		if (__wifi_datamode) {
			if (size > __wifi_puredatabuf) {
			int i;
			for (i = 0; i < size; i += __wifi_puredatabuf) {
				if (RS232_SendBuf(wifi_portnr, buf + i, __wifi_puredatabuf) == 1) {
					perror("Data send failed. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
			}
			sleep(__WIFI_WAIT_TIME);
			#ifdef debug
				if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
					perror("Reading response failed. Reason: ");
					printf("Response: %s\n", AT_Response);
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
				printf("Response: %s\n", AT_Response);
#endif
			if (i != size) {
				if (RS232_SendBuf(wifi_portnr, buf + size - (i-size), (i-size)) == 1) {
					perror("Data send failed. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
			}
			} else {
				if (RS232_SendBuf(wifi_portnr, buf, __wifi_puredatabuf) == 1) {
					perror("Data send failed. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
			}
		} else {
			if (size > __wifi_atbuf) {
			int i = 0;
			for (i = 0; i < size; i += __wifi_atbuf) {
				snprintf(AT_Command, __wifi_atbuf+12, "AT#Send -d%s\r", buf+i);
#ifdef debug
				printf("Created AT Command: %s\nCommand Size: %d\n", AT_Command, strlen(AT_Command));
#endif
				if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
					perror("Could not send command. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
				sleep(__WIFI_WAIT_TIME);
					#ifdef debug
						if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
							perror("Reading response failed. Reason: ");
							printf("Response: %s\n", AT_Response);
							free(AT_Command);
							free(AT_Response);
							return -1;
						}
						printf("Response: %s\n", AT_Response);
					#endif
			}
#ifdef debug
			printf("Current i: %d\n", i);
			printf("Data remaining: %s\n", buf+i);
#endif
			if (i != size) {
				snprintf(AT_Command, (i-size) + 12,"AT#Send -d%s\r", buf+(i-size+12));
#ifdef debug
				printf("Created AT Command: %s\nCommand Size: %d\n", AT_Command, strlen(AT_Command));
#endif
				if (RS232_SendBuf(wifi_portnr, buf + size - (i-size), (i-size)) == 1) {
					perror("Data send failed. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
			}
			} else {
				snprintf(AT_Command, size+12, "AT#Send -d%s\r", buf);
#ifdef debug
				printf("Created AT Command: %s\nCommand Size: %d\n", AT_Command, strlen(AT_Command));
#endif
				if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
					perror("Could not send command. Reason: ");
					free(AT_Command);
					free(AT_Response);
					return -1;
				}
				sleep(__WIFI_WAIT_TIME);
			}
		}
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_read_data(void *buf, int size) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	int bytes = 0;
	if (wifi_portnr == 0) {
		perror("Port is not configured");
		return -1;
	}
	if ((bytes = RS232_PollComport(wifi_portnr, buf, size)) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", buf);
		return -1;
	}
	return bytes;
}

int Wifi_reset( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char *AT_Command = (unsigned char *) malloc(__WIFI_MAX_BUFFER);
	unsigned char *AT_Response = (unsigned char *)malloc(__WIFI_MAX_BUFFER);

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		free (AT_Command);
		free(AT_Response);
		return -1;
	}
	snprintf(AT_Command, __WIFI_MAX_BUFFER, "AT#Reboot\r");

	if (RS232_SendBuf(wifi_portnr, AT_Command, strlen(AT_Command)) == 1) {
		perror("Could not send command. Reason: ");
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	sleep(__WIFI_WAIT_TIME *1.5);
#ifdef debug
	if (RS232_PollComport(portnr, AT_Response, __WIFI_MAX_BUFFER) < 0) {
		perror("Reading response failed. Reason: ");
		printf("Response: %s\n", AT_Response);
		free(AT_Command);
		free(AT_Response);
		return -1;
	}
	printf("Response: %s\n", AT_Response); //TODO:Debug response and update return value
#endif
	free(AT_Command);
	free(AT_Response);
	return 0;
}

int Wifi_stop_connection( void ) {
	//
	//
	//LOCAL VARIABLES
	//

	if (wifi_portnr == 0) {
		perror("Port is not configured");
		return -1;
	}
	RS232_CloseComport(wifi_portnr);
	wifi_portnr = 0;
	return 0;
}















