/********** ********** ********** ********** ********** ********** **********
 *                                 ( 0-0 )                                  *
 *                            (" \/       \/ ")                             *
 ********** ********** ********** ********** ********** ********** **********
 * Copyright (C) 2018 - 2022 CURI & HKCLR                                   *
 * File name   : curi_udp.c                                                 *
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

#include "curi_udp.h"
#include <sys/time.h>   // defines struct timeval


int udp_init(udp_node* p, const char receive_ip[], int receive_port, const char send_ip[], int send_port, int buffer_size){
	int ret = udp_init_receive_fd(p, receive_ip, receive_port, buffer_size);
	if (ret != 0){
		printf("Failed to initialize receive_fd, return code: %d\n", ret);
		return -1;
	}

	ret = udp_init_send_fd(p, send_ip, send_port, buffer_size);
	if (ret != 0){
		printf("Failed to initialize send_fd, return code: %d\n", ret);
		return -1;
	}
	return 0;
}

int udp_init1(udp_node* p, const char receive_ip[], int receive_port, const char send_ip[], int send_port, int buffer_size, int receive_fd){
	int ret = udp_init_receive_fd1(p, receive_ip, receive_port, buffer_size, receive_fd);
	if (ret != 0){
		printf("Failed to initialize receive_fd, return code: %d\n", ret);
		return -1;
	}

	ret = udp_init_send_fd(p, send_ip, send_port, buffer_size);
	if (ret != 0){
		printf("Failed to initialize send_fd, return code: %d\n", ret);
		return -1;
	}
	return 0;
}

int udp_init_receive_fd(udp_node* p, const char receive_ip[], int receive_port, int buffer_size){
	int receive_fd = socket(AF_INET, SOCK_DGRAM, 0);
	udp_init_receive_fd1(p, receive_ip, receive_port, buffer_size, receive_fd);
}

int udp_init_receive_fd1(udp_node* p, const char receive_ip[], int receive_port, int buffer_size, int receive_fd){
#ifdef WIN32
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		return err;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return -1;
	}
#endif 
    p->lock = 0;
	// create UDP socket 
	p->receive_fd = receive_fd;
	p->receive_addr.sin_addr.s_addr = inet_addr(receive_ip);
	p->receive_addr.sin_port = htons(receive_port);
	p->receive_addr.sin_family = AF_INET;

	// bind server address to socket descriptor 
#ifdef WIN32
	if (bind(p->receive_fd, (SOCKADDR*)&(p->receive_addr)), sizeof(p->receive_addr)) == -1) {
#else
	if (bind(p->receive_fd, (const struct sockaddr *)&(p->receive_addr), sizeof(p->receive_addr)) == -1) {
#endif
		perror("bind error.");
		return -1;
	}

	// set the buffer size
	p->receive_buffer = (char*)malloc(buffer_size);
	if (!p->receive_buffer) {
		free(p->receive_buffer); 
		return -2; 
	}

	// set the buffer size for udp socket
	if (0 != setsockopt(p->receive_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&buffer_size, sizeof(int)))
		return -3;

	FD_ZERO(&(p->rset));

	return 0;
}

int udp_init_send_fd(udp_node* p, const char send_ip[], int send_port, int buffer_size){
#ifdef WIN32
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		return err;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return -1;
	}
#endif 
    p->lock = 0;
	p->send_fd = socket(AF_INET, SOCK_DGRAM, 0);
	p->send_addr.sin_addr.s_addr = inet_addr(send_ip);
	p->send_addr.sin_port = htons(send_port);
	p->send_addr.sin_family = AF_INET;

	// set the buffer size
	p->send_buffer = (char*)malloc(buffer_size);
	if (!p->send_buffer) {
		free(p->send_buffer); 
		return -2; 
	}

	if (0 != setsockopt(p->send_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&buffer_size, sizeof(int)))
		return -3;

	FD_ZERO(&(p->rset));

	return 0;
}
/*
 * function: udp_select
 *     udp communication selection to test if there are some data at the port
 * input:
 *     p[udp_node *]: the udp_node structure
 *     usec[int]: wait data time in us
 * output:
 *     [int]: the data received
 */
int udp_select(udp_node* p, int usec, int buffer_size)
{
	FD_SET(p->receive_fd, &(p->rset));
	// select the ready descriptor 
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = usec;
	int nready = select(p->receive_fd + 1, &(p->rset), NULL, NULL, &t);
	if (FD_ISSET(p->receive_fd, &(p->rset))) {
		memset(p->receive_buffer, 0, buffer_size);
		p->receive_size = recv(p->receive_fd, p->receive_buffer, buffer_size, 0);
	} else {
		p->receive_size = 0;
	}
	return p->receive_size;
}

/*
 * function: udp_send
 *     udp communication send data to the remote port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_send(udp_node* p, int buffer_size)
{
    if (!p || p->send_fd < 0 || !p->send_buffer) {
        return;
    }
    ssize_t send_bytes = sendto(
        p->send_fd, 
        p->send_buffer, 
        buffer_size,
        0,
        (const struct sockaddr *)&(p->send_addr), 
        sizeof(p->send_addr)
    );
	// const char send_buffer[buffer_size];
	// strncpy(send_buffer, p->send_buffer, strlen(p->send_buffer));
	// sendto(p->send_fd, (struct sockaddr *)&(send_buffer), strlen(send_buffer), 0,
	// 	(const struct sockaddr *)&(p->send_addr), sizeof(p->send_addr));
}

/*
 * function: udp_receive
 *     udp communication receive data from the local port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_receive(udp_node* p, int buffer_size)
{
	p->receive_size = recv(p->receive_fd, p->receive_buffer, buffer_size, 0);
}

/*
 * function: udp_print
 *     udp communication print the receive data from the local port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_print(udp_node* p)
{
	p->receive_buffer[p->receive_size] = 0;
	puts(p->receive_buffer);
}

/*
 * function: udp_close
 *     udp communication close the port
 * input:
 *     p[udp_node *]: the udp_node structure
 * output:
 *     [void]
 */
void udp_close(udp_node* p)
{
	free(p->receive_buffer);
	free(p->send_buffer);
	p->receive_buffer = NULL;
	p->send_buffer = NULL;
#ifdef WIN32
	closesocket(p->receive_fd);
	closesocket(p->send_fd);
	WSACleanup();
#else
	close(p->receive_fd);
	close(p->send_fd);
#endif
}
