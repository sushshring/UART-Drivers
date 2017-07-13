/*
 * ipc_controller.c
 *
 *  Created on: May 24, 2017
 *      Author: sushshring
 */

#include "ipc_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include "math.h"



int sendData(void *buf, int size, int receiverPID) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	char proc[1024];
	key_t key;
	int length = 15+(receiverPID > 0 ? (int) log10 ((double) receiverPID) + 1 : 1);
	int semid, shmid;
	void *shmaddr = NULL;
	union semun {
	   int              val;    /* Value for SETVAL */
	   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	   unsigned short  *array;  /* Array for GETALL, SETALL */
	   struct seminfo  *__buf;  /* Buffer for IPC_INFO
								   (Linux-specific) */
	} semun_t;

	struct sembuf sb[1];
//	struct shmid_ds mb;

	if (receiverPID <= 0) {
		//Illegal PID
		printf("Illegal PID");
		return -1;
	}
	if (size > 4096) {
		printf("Too large buffer.\n");
		return -1;
	}
	//Get semaphore key
	snprintf(proc, length, "/proc/%d/cmdline", receiverPID);
	printf("Created string: %s\n", proc);
	key = ftok(proc, id);
	printf("Got key: %d\n", (int)key);
	if (key == (key_t) -1) {
		//failed
		return -1;
	}

	semid = semget(key, 1, 0666 | IPC_CREAT);
	printf("Got semaphore id: %d\n", semid);
	if (semid < 0) {
		printf("Semaphore failed\n");
		return -1;
	}
	unsigned short values[1];
	values[0] = 1;
	semun_t.array = values;
	if (semctl(semid, 1, SETALL, semun_t) == -1) {
		perror("semctl failed. Reason: ");
		semctl(semid, 1, IPC_RMID);
		return -1;
	}
	sb[0].sem_num = 0;
	sb[0].sem_op = -1;
	if (semop(semid, sb, 1) == -1) {
		semctl(semid, 1, IPC_RMID);
		printf("semop failed\n");
	}
	printf("Semaphore obtained and set. Writing data now.\n");

	shmid = shmget(key, SHMSZ, IPC_CREAT | IPC_EXCL | 0666);
	if (shmid == -1) {
		perror("Memory failed. Reason: ");
		semctl(semid, 1, IPC_RMID);
		return -1;
	}
//	mb.shm_perm.mode = 0666;
//	if (shmctl(shmid, IPC_SET, mb) == -1) {
//		printf("shmctl failed\n");
//		semctl(semid, 1, IPC_RMID);
//		shmctl(shmid, IPC_RMID, NULL);
//		return -1;
//	}
	if ((shmaddr = shmat(shmid, shmaddr, 0)) == (char *) -1) {
		printf("shm attach failed\n");
		semctl(semid, 1, IPC_RMID);
		shmctl(shmid, IPC_RMID, NULL);
		return -1;
	}
	printf("memory attached to: %d\n", (unsigned int) shmaddr);
	memcpy(shmaddr, buf, size);
	if (shmdt(shmaddr) == -1) {
		printf("shm detach failed\n");
		semctl(semid, 1, IPC_RMID);
		shmctl(shmid, IPC_RMID, NULL);
		return -1;
	}
	printf("Copied data\n");
	printf("Ready!\n");
	//Send signal
	printf("Sending signal\n");
	sb[0].sem_op = 1;
	if (semop(semid, sb, 1) == -1) {
		semctl(semid, 1, IPC_RMID);
		printf("semop failed\n");
	}
	printf("Semaphore freed. Read data now.\n");
	if (kill(receiverPID, 10) == -1) {
		printf("Kill failed\n");
		return -1;
	}
	return size;
}

int getData(void *buf, int receiverPID) {
	//
	//
	//LOCAL VARIABLES
	//
	//
	char proc[1024];
	key_t key;
	int length = 15+(receiverPID > 0 ? (int) log10 ((double) receiverPID) + 1 : 1);
	int semid, shmid;
	void *shmaddr = NULL;
//	union semun {
//		int val;
//		struct sem_id * buf;
//		unsigned short * array;
//	} semun_t;

	struct sembuf sb[1];
	//struct shmid_ds mb;
	if (receiverPID <= 0) {
		//Illegal PID
		printf("Illegal PID");
		return -1;
	}
	//Get semaphore key
	snprintf(proc, length, "/proc/%d/cmdline", receiverPID);
	printf("Created string: %s\n", proc);
	key = ftok(proc, id);
	printf("Got key: %d\n", (int)key);
	if (key == (key_t) -1) {
		//failed
		return -1;
	}
	semid = semget(key, 1, 0666 | SHM_RDONLY);
	printf("Got semaphore id: %d\n", semid);
	if (semid < 0) {
		printf("Semaphore failed\n");
		return -1;
	}
	sb[0].sem_num = 0;
	sb[0].sem_op = -1;
	if (semop(semid, sb, 1) == -1) {
		semctl(semid, 1, IPC_RMID);
		printf("semop failed\n");
	}
	printf("Semaphore obtained and set. Reading data now.\n");
	shmid = shmget(key, SHMSZ, 0666);
	if (shmid == -1) {
		perror("Memory failed. Reason:");
		semctl(semid, 1, IPC_RMID);
		return -1;
	}
	if ((shmaddr = shmat(shmid, shmaddr, 0)) == (char *) -1) {
		printf("shm attach failed\n");
		semctl(semid, 1, IPC_RMID);
		return -1;
	}
	memcpy(buf, shmaddr, SHMSZ);
	sb[0].sem_op = 1;
	if (semop(semid, sb, 1) == -1) {
		semctl(semid, 1, IPC_RMID);
		printf("semop failed\n");
	}
	printf("Semaphore freed.\n");
	shmdt(shmaddr);
	semctl(semid, 1, IPC_RMID);
	shmctl(shmid, IPC_RMID, NULL);
	return SHMSZ;
}

