/*
 * main.c
 *
 *  Created on: Jun 13, 2017
 *      Author: sushshring
 */

#include <lib_OEM_Wifi.h>
#include <lib_OEM_GSM.h>
#include <lib_OEM_XBee_Coordinator.h>
#include <rs232.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
//#include <linux/interrupt.h>
#include <signal.h>
#include <sys/types.h>
#define MAX_BUF_REC 1024
#define MAX_BUF_SEND 500
#define debug

char *buf_wifi_rec;
char *buf_GSM_rec;
char *buf_XBee_rec;
char *buf_wifi_send;
char *buf_XBee_send;
char *buf_GSM_send;

void cleanup() {
	free(buf_wifi_rec);
	free(buf_GSM_rec);
	free(buf_XBee_rec);
	free(buf_wifi_send);
	free(buf_XBee_send);
	free(buf_GSM_send);
}

volatile int loop = 1;

//static irqreturn_t intr_handler(int irq, void *dev) {
//	printf("Interrupt receivewd\n");
//}

void sighandler(int signum, siginfo_t *info, void *ptr) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	printf("SIGNAL RAISED: %d\n",signum);
	buf_wifi_rec = (char *) malloc(MAX_BUF_REC);
	buf_GSM_rec = (char *) malloc(MAX_BUF_REC);
	buf_XBee_rec = (char *) malloc(MAX_BUF_REC);
	buf_wifi_send = (char *) malloc(MAX_BUF_SEND);
	buf_GSM_send = (char *) malloc(MAX_BUF_SEND);
	buf_XBee_send = (char *) malloc(MAX_BUF_SEND);


	printf("Signal received: %d\n", signum);
	printf("Signal sent by IO: %d\n", info->si_fd);
	if (signum == SIGIO) {
		if (info->si_fd == wifi_portnr) {
			Wifi_read_data(buf_wifi_rec, MAX_BUF_REC);
			printf("Received buffer: %s\n", buf_wifi_rec);
			//GSM_send_data(buf_wifi_rec, MAX_BUF_REC);
		} else if (info->si_fd == gsm_portnr) {
			GSM_read_data(buf_GSM_rec, MAX_BUF_REC);
			printf("Received buffer: %s\n", buf_GSM_rec);
		} else if (info->si_fd == xbee_portnr) {
			XBEE_read_data(buf_XBee_rec, (size_t) MAX_BUF_REC);
			printf("Received buffer: %s\n", buf_XBee_rec);
			//GSM_send_data(buf_XBee_rec, MAX_BUF_REC);
		} else {
			perror("Operation not supported\n");
		}
	}
}

void sigkill (int no) {
	if (no == SIGTERM || no == SIGKILL) {
		loop = 0;
		printf("Killing\n");
		cleanup();
		Wifi_stop_connection();
		GSM_disconnect();
		XBEE_close_connection();
	}
}

int main ()
{
	//
	//
	//LOCAL VARIABLES
	//
	//
	char wifi_port[] = "ttyUSB3";
	char gsm_port[] = "ttyUSB0";
	char xbee_port[] = "ttyUSB2";
	char pan_id[50];
	char ip[50] = "54.175.103.105";
	int gsm_fd = 0, wifi_fd = 0,xbee_fd = 0;

	struct sigaction new_action = {0}, old_action = {0};

	if ((wifi_fd = Wifi_init_connection(wifi_port, 0)) == -1) {
		printf("Wifi could not be initialized\n");
	}
//	else {
//		printf("Wifi initialized\n");
//		printf("Port: %d\n", wifi_portnr);
//	}
//	if (Wifi_setup_AP("STINT", "st123450") == -1) {
//			printf("AP connection failed\n");
//		} else {
//			printf("AP coonection sucess\n");
//			Wifi_get_IP(ip);
//			printf("IP Address: %s\n", ip);
//		}

	if ((gsm_fd = GSM_init_connection(gsm_port)) == -1) {
		printf("GSM could not be initialized\n");
		//RS232_PollComport(wifi_portnr, NULL, 80);
	} else {
		printf("Gsm inittialized. GSM Port number: %d\n", gsm_portnr);
	}
	//XBEE_init_connection(xbee_port, "coordinator", pan_id);
	int wififlags = 0;
	if ((wififlags = fcntl(wifi_fd, F_GETFD)) == -1) {
		perror("Could not read flags. Reason:");
	}
	if (fcntl(wifi_fd, F_SETFD, wififlags|FASYNC) == -1) {
		perror("Could not initiate async RW mode for wifi. Reason: ");
	}
	int gsmflags = fcntl(gsm_fd, F_GETFD);
		if (fcntl(gsm_fd, F_SETFD, gsmflags|FASYNC) == -1) {
			perror("Could not initiate async RW mode for wifi. Reason: ");
		}
		int xbeeflags = fcntl(xbee_fd, F_GETFD);
			if (fcntl(xbee_fd, F_SETFD, xbeeflags|FASYNC) == -1) {
				perror("Could not initiate async RW mode for wifi. Reason: ");
			}

	new_action.sa_sigaction = sighandler;
		sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = SA_SIGINFO;
		sigaction(SIGIO, NULL, &old_action);
		if (old_action.sa_handler != SIG_IGN) {
		sigaction(SIGIO, &new_action, NULL);
		}
		if (fcntl(wifi_fd, F_SETOWN, getpid()) == -1) {
			perror("Could not register IO handler for wifi port. Reason: ");
		}
		if (fcntl(gsm_fd, F_SETOWN, getpid()) == -1) {
			perror("Could not register IO handler for GSM port. Reason: ");
		}
		if (fcntl(xbee_fd, F_SETOWN, getpid()) == -1) {
			perror("Could not register IO handler for XBee port. Reason: ");
		}



//	if (Wifi_connect_TCP("192.168.0.59", 7681) == -1) {
//		printf("Could not establish TCP connection\n");
//	} else {
//		printf("Connected via TCP to 192.168.0.59 on port 7681\n");
//	}
//
//	if (Wifi_startTCP_Listen(8080) == -1) {
//		printf("Failed to open port 8080\n");
//	} else {
//		printf("Listening for TCP connections on port 8080\n");
//	}
//	if (Wifi_startTCP_Listen(80) == -1) {
//		printf("Failed to open port 80\n");
//	} else {
//		printf("Listening on port 80\n");
//	}
	Wifi_send_data("asjfdlkjasldkfjaskldfjlaksdfj", strlen("asjfdlkjasldkfjaskldfjlaksdfj"));

	//GSM_configure("uninor", " ", " ");
	GSM_connect_TCP(ip, 30000);


//	request_irq(
//			0,
//			intr_handler,
//			IRQF_SHARED,
//			"wifi_module",
//			"wifi_module"
//			);


	GSM_send_data("afjlaskf", strlen("afjlaskf"));

	printf("Initialization complete. Waiting for data\n");


	while(loop) {
		//listening for data
		sleep(1);
		//printf("Woken up\n");
	}
	return 0;
}











