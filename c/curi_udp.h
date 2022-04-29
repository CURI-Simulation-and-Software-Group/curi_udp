#ifndef CURI_UDP_H

#ifdef WIN32
	#include <arpa/inet.h> 
	#include <netinet/in.h> 
	#include <sys/socket.h> 
	#include <sys/types.h> 
	#include <stdint.h>
#else
	#include <Winsock2.h>
	#pragma comment(lib, "WS2_32.lib")
#endif
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAX_REMOTER_DATA_SIZE 256

typedef struct _udp_node
{
	int CURI_SEND_PORT;
	int CURI_RECIVE_PORT;
	char recieve_buffer[MAX_REMOTER_DATA_SIZE];
	char send_buffer[MAX_REMOTER_DATA_SIZE];
	int recieve_size;
	int send_size;
	int server_fd;
	int client_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	fd_set rset;
}udp_node;

int udp_init(udp_node* p, char local_ip[], int local_port, char remote_ip[], int remote_port);

int udp_select(udp_node* p, int usec);

void udp_send(udp_node* p);

void udp_receive(udp_node* p);

void udp_print(udp_node* p);

void udp_close(udp_node* p);

#endif