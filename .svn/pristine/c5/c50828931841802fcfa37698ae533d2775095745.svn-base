/*
 * main.c
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */

#include "lib_OEM_Wifi.h"
#include <stdio.h>

int main ( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	char temp[__WIFI_MAX_BUFFER];
	char *in = (char *) malloc(__WIFI_MAX_BUFFER*2);
	if (Wifi_init_connection("ttyUSB0", 0) == -1) {
		perror("Init failed. Reason: ");
		return -1;
	}
	Wifi_change_dhcp_mode(STATIC_MODE, "192.168.0.121");
	if (Wifi_setup_AP("STINT", "st123450") == -1) {
		perror("Init failed. Reason: ");
		return -1;
	}
	Wifi_get_IP(temp);
	printf("IP: %s\n", temp);
	Wifi_get_mac(temp);
	printf("Mac: %s\n", temp);
	//Wifi_change_dhcp_mode(DHCP_MODE, NULL);
	Wifi_connect_TCP("54.175.103.105", 30000);
	for (int i = 509; i < 1000; i++) {
		int len = 0;
		for (int j = 0; j < i; j++) {
			*(in + j) = (char)(33+i-509);
			len = j;
		}
		*(in+len+1) = '\0';
		printf("Data size: %d\n", (int) strlen(in));
		printf("Data: %s\n", in);
		Wifi_send_data(in, strlen(in));
	}
	//Wifi_connect_TCP("135.668.000.123", 7861);
	Wifi_startTCP_Listen(80);
	Wifi_get_IP(temp);
	printf("IP: %s\n", temp);
	printf("listening\n");
	sleep(25);
	strcpy(temp, "");
	Wifi_read_data(temp, __WIFI_MAX_BUFFER);
	printf("Received data: %s",temp);
	Wifi_reset();
	Wifi_stop_connection();
	free(in);
}
