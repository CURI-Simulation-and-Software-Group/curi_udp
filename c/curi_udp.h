/********** ********** ********** ********** ********** ********** **********
 *                                 ( 0-0 )                                  *
 *                            (" \/       \/ ")                             *
 ********** ********** ********** ********** ********** ********** **********
 * Copyright (C) 2018 - 2022 CURI & HKCLR                                   *
 * File name   : curi_udp.h                                                 *
 * Author      : CHEN Wei                                                   *
 * Version     : 1.0.0                                                      *
 * Date        : 2022-04-29                                                 *
 * Description : Udp communication.                                         *
 * Others      : None                                                       *
 * History     : 2022-04-29 1st version.                                    *
 ********** ********** ********** ********** ********** ********** **********
 *                              (            )                              *
 *                               \ __ /\ __ /                               *
 ********** ********** ********** ********** ********** ********** **********/

#ifndef CURI_UDP_H
#define CURI_UDP_H

#ifdef WIN32
	#include <Winsock2.h>
	#pragma comment(lib, "WS2_32.lib")
#else
	#include <arpa/inet.h> 
	#include <netinet/in.h> 
	#include <sys/socket.h> 
	#include <sys/types.h> 
	#include <stdint.h>
	#include <sys/select.h>
#endif
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>

typedef struct _udp_node
{
	int CURI_SEND_PORT;
	int CURI_RECIVE_PORT;
	char* receive_buffer;
	char* send_buffer;
	int receive_size;
	int send_size;
	int receive_fd;
	int send_fd;
	struct sockaddr_in receive_addr;
	struct sockaddr_in send_addr;
	fd_set rset;
    uint8_t lock;
}udp_node;

#ifdef __cplusplus
extern "C" 
{
#endif

int udp_init(udp_node* p, char receive_ip[], int receive_port, char send_ip[], int send_port, int buffer_size);
int udp_init1(udp_node* p, char receive_ip[], int receive_port, char send_ip[], int send_port, int buffer_size, int receive_fd);
int udp_init_receive_fd(udp_node* p, char receive_ip[], int receive_port, int buffer_size);
int udp_init_receive_fd1(udp_node* p, char receive_ip[], int receive_port, int buffer_size, int receive_fd);
int udp_init_send_fd(udp_node* p, char send_ip[], int send_port, int buffer_size);


/*
 * function: udp_select
 *     udp communication selection to test if there are some data at the port
 * input:
 *     p[udp_node *]: the udp_node structure
 *     usec[int]: wait data time in us
 * output:
 *     [int]: the data received
 */
int udp_select(udp_node* p, int usec, int buffer_size);

/*
 * function: udp_send
 *     udp communication send data to the remote port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_send(udp_node* p, int buffer_size);

/*
 * function: udp_receive
 *     udp communication receive data from the local port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_receive(udp_node* p, int buffer_size);

/*
 * function: udp_print
 *     udp communication print the receive data from the local port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_print(udp_node* p);

/*
 * function: udp_close
 *     udp communication close the port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_close(udp_node* p);

#ifdef __cplusplus
}
#endif
#endif
