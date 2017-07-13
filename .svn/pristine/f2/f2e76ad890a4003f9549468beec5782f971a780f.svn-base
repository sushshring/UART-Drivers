/*
 * ipc_controller.h
 *
 *  Created on: May 24, 2017
 *      Author: sushshring
 */

#ifndef IPC_CONTROLLER_INCLUDED
#define IPC_CONTROLLER_INCLUDED

#include <sys/types.h>
#include <unistd.h>

#define id 833498
#define SHMSZ 4096


/*******************************
 * Function: sendData
 * This function takes data from one process (PID: senderPID)
 * and sends it to the other process (PID: receiverPID).
 * NOTE: Receiver PID must implement a handler for signal SIGUSR1,
 * which will be called when the data is completely sent
 * Parameters:
 * senderPID: Process ID of the sending process.
 * buf: buffer of data. must not exceed 4096 bytes
 * size: size of buf
 * receiverPID: PID of the receiving process
 * RETURNS:
 * number of bytes written. -1 if failed.
 *********************************/
int sendData(void *buf, int size, pid_t receiverPID);

/**
 *
 *
 */
int getData(void *buf, pid_t receiverPID);

#endif
