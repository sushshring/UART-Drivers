/*
 * file: uart_com.h
 *
 *  Created on: May 22, 2017
 *      Author: Sushrut Shringarputale
 */
#ifndef UART_COM_INCLUDED
#define UART_COM_INCLUDED

#include "rs232.h"

/*********************************
 * FUNCTION: 	uart_init
 * Description: This function creates a connection
 * from port1 to port2. Whenever data is sent to port1
 * it will automatically be written out to port2
 *
 * INPUTS: port1: source port number matching the port in below map
 * 		   port2: destination port number matching the port in below map
 * OUTPUT: Returns 1 if unsuccessful
 * APINote: This only supports data transmission in 8N1 mode.
 */
int uart_init(char* port1Name, char* port2Name, int bdrate);

/**
portLinux 	windows
0 	ttyS0 	COM1
1 	ttyS1 	COM2
2 	ttyS2 	COM3
3 	ttyS3 	COM4
4 	ttyS4 	COM5
5 	ttyS5 	COM6
6 	ttyS6 	COM7
7 	ttyS7 	COM8
8 	ttyS8 	COM9
9 	ttyS9 	COM10
10 	ttyS10 	COM11
11 	ttyS11 	COM12
12 	ttyS12 	COM13
13 	ttyS13 	COM14
14 	ttyS14 	COM15
15 	ttyS15 	COM16
16 	ttyUSB0 	n.a.
17 	ttyUSB1 	n.a.
18 	ttyUSB2 	n.a.
19 	ttyUSB3 	n.a.
20 	ttyUSB4 	n.a.
21 	ttyUSB5 	n.a.
22 	ttyAMA0 	n.a.
23 	ttyAMA1 	n.a.
24 	ttyACM0 	n.a.
25 	ttyACM1 	n.a.
26 	rfcomm0 	n.a.
27 	rfcomm1 	n.a.
28 	ircomm0 	n.a.
29 	ircomm1 	n.a.
30 	cuau0 	n.a.
31 	cuau1 	n.a.
32 	cuau2 	n.a.
33 	cuau3 	n.a.
34 	cuaU0 	n.a.
35 	cuaU1 	n.a.
36 	cuaU2 	n.a.
37 	cuaU3 	n.a.
 */

#endif

