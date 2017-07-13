/*
 * main.c
 *
 *  Created on: May 25, 2017
 *      Author: sushshring
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "ipc_controller.h"
void getSignal(int);

int main (int argc, char *argv[] ) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	char *message = (char *) malloc(4096);
	int receiverPID = strtol(argv[2], NULL, 0);
	size_t size = 512;
	printf("starting\n");

	if (strtol(argv[1], NULL, 0) == 0) {
		printf("wut\n");
		//In send process
		getline(&message, &size, stdin);
		printf("sending data. Response: %d\n", sendData(message, 4096, receiverPID));
		free(message);
		exit(0);
	} else {
		signal(SIGUSR1, getSignal);
		while(1)
			sleep(1);
	}
}

void getSignal(int signal) {
	printf("Handling!\n");
	if (signal == SIGUSR1) {
		char *message = (char *) malloc(4096);
		printf("Got data! size: %d\n", getData(message, getpid()));
		printf(message);
		free(message);
		exit(0);
	}
}
