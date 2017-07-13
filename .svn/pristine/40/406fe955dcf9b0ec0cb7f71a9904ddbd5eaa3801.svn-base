/*
 * main.c
 *
 *  Created on: May 22, 2017
 *      Author: sushshring
 */


#include "uart_com.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
	int status;
	char* port1, *port2;

	if (argc != 3) {
		exit(-1);
	}
	printf("Received ports: %s - %s\n", argv[1], argv[2]);
	port1 = argv[1];
	port2 = argv[2];


	printf("Starting to listen\n");
	//childpid = fork();
	if (fork() == 0) {
		exit(uart_init(port1, port2, 9600));
	} else {
		wait(&status);
	}
		printf("exit with status: %d\n", status);
	return 0;

}
