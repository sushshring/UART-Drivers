/*
 * main.c
 *
 *  Created on: May 31, 2017
 *      Author: sushshring
 */



#include <stdio.h>
#include "lib_OEM_GSM.h"
#define debug

int main ( void ) {
	//
	//
	//LOCAL VARIABLES
	//
	//

	if (GSM_init_connection("ttyUSB0") == -1) {
		printf("Failed\n");
	}
	//GSM_configure("uninor", " ", " ");
	GSM_connect_TCP("54.175.103.105", 30000);
	GSM_send_data("afjlaskf", strlen("afjlaskf"));
}
