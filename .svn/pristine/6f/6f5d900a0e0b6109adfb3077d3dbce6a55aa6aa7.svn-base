/*
 * uart_com.c
 *
 *  Created on: May 22, 2017
 *      Author: Sushrut Shringarputale
 */

#include <stdio.h>
#include "uart_com.h"
#include "rs232.h"
#include <sys/signal.h>

volatile sig_atomic_t listen = 1;

void sig_handler(int signal) {
	printf("Got signal: %d", signal);
	listen = 0;
}
/**
 * Creates a two way listening mechanism between
 * port1Name and port2Name
 */
int uart_init(char* port1Name, char* port2Name, int bdrate) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	unsigned char buf[4096], tBuf[4096];
	char mode[] = {'8', 'N', '1', 0};
	int n = 0, m = 0,r = 0, s = 0;

	struct sigaction new_handler, old_handler;
	new_handler.sa_handler = sig_handler;
	new_handler.sa_flags = 0;
	sigaction(SIGINT, &new_handler, &old_handler);
	sigaction(SIGHUP, &new_handler, &old_handler);
	sigaction(SIGTERM, &new_handler, &old_handler);


	int port1 = RS232_GetPortnr(port1Name);
	int port2 = RS232_GetPortnr(port2Name);

	printf("Trying to open port: %d\n", port1 );
	printf("Trying to open port: %d\n", port2 );
	if (RS232_OpenComport(port1, bdrate, mode)) {
		printf("Port1 could not be opened\n");
		return 1;
	} else {
		printf("port1 opened\n");
	}
	if (RS232_OpenComport(port2, bdrate, mode)) {
		printf("Port2 could not be opened\n");
		return 1;
	} else {
		printf("port2 opened\n");
	}
	char data[] = "this is data\n";
	printf("Connected ports: %d and writing to port: %d\n", port1, port2);
//	if (RS232_SendBuf(port2, data, strlen(data))) {
//		printf("write failed\n");
//		return 1;
//	} else {
//		printf("wrote\n");
//	}
	while (1 && listen) {
		n = RS232_PollComport(port1, buf, 4095);
		if (n>0) {
			if (buf[n] != '\0')
				buf[n] = '\0';
			for (int i = 0; i < n; i++) {
				if (buf[i]<32) {
					buf[i] = '.';
				}
			}
			printf("Received data on port: %d\n. Data: %s\n", port1, buf);
			r = RS232_SendBuf(port2, buf, n);
			if (r == -1) {
				return 1;
			}
			n = 0;
			r = 0;
		}
		m = RS232_PollComport(port2, tBuf, 4095);
		if (m>0) {
			if (tBuf[m] != '\0')
				tBuf[m] = '\0';
			for (int i = 0; i < m; i++) {
				if (tBuf[i]<32) {
					tBuf[i] = '.';
				}
			}
			printf("Received data on port: %d\n. Data: %s\n", port2, tBuf);
			s = RS232_SendBuf(port1, tBuf, m);
			if (r == -1) {
				return 1;
			}
			m = 0;
			s = 0;
		}
	}

	RS232_CloseComport(port1);
	RS232_CloseComport(port2);
	return 0;
}




