#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdint.h>

#define CURI_SEND_PORT 11230
#define CURI_RECIVE_PORT 11235
#define MAX_REMOTER_DATA_SIZE 256
char communication_get_data[MAX_REMOTER_DATA_SIZE];
char communication_set_data[MAX_REMOTER_DATA_SIZE];
int communication_get_data_size = 0;
int communication_server = 0;
int communication_client = 0;
struct sockaddr_in communication_server_addr;
struct sockaddr_in communication_client_addr;
fd_set communication_rset;

int communication_init(char local_ip[], int local_port, char remote_ip[], int remote_port)
{
	// Create a UDP Socket 
	communication_server = socket(AF_INET, SOCK_DGRAM, 0);
	communication_server_addr.sin_addr.s_addr = inet_addr(local_ip);
	communication_server_addr.sin_port = htons(local_port);
	communication_server_addr.sin_family = AF_INET;

	communication_client = socket(AF_INET, SOCK_DGRAM, 0);
	communication_client_addr.sin_addr.s_addr = inet_addr(remote_ip);
	communication_client_addr.sin_port = htons(remote_port);
	communication_client_addr.sin_family = AF_INET;

	// bind server address to socket descriptor 
	if (bind(communication_server, (struct sockaddr_in *)&communication_server_addr, sizeof(communication_server_addr)) == -1) {
		perror("bind error.");
		exit(1);
	}
	FD_ZERO(&communication_rset);

	return 0;
}

int communication_get(int usec)
{
	FD_SET(communication_server, &communication_rset);
	// select the ready descriptor 
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = usec;
	int nready = select(communication_server + 1, &communication_rset, NULL, NULL, &t);
	if (FD_ISSET(communication_server, &communication_rset)) {
		memset(communication_get_data, 0, sizeof(communication_get_data));
		communication_get_data_size = recv(communication_server, communication_get_data, MAX_REMOTER_DATA_SIZE, 0);
	}
	return nready;
}

void communication_send()
{
	sendto(communication_client, communication_set_data, strlen(communication_set_data), 0,
		(const struct sockaddr_in*)&communication_client_addr, sizeof(communication_client_addr));
}

void communication_receive()
{
	memset(communication_get_data, 0, sizeof(communication_get_data));
	communication_get_data_size = recv(communication_server, communication_get_data, MAX_REMOTER_DATA_SIZE, 0);
}

void communication_print()
{
	communication_get_data[communication_get_data_size] = 0;
	puts(communication_get_data);
}

void communication_close()
{
	close(communication_server);
	close(communication_client);
}

void communication_pack(int state, hduVector3Dd position, hduVector3Dd force)
{
	sprintf_s(communication_set_data, MAX_REMOTER_DATA_SIZE, "#%1d,%8.3lf,%8.3lf,%8.3lf,%8.3f,%8.3lf,%8.3lf#",
		state, position[0], position[1], position[2], force[0], force[1], force[2]);
}

void communication_unpack(hduVector3Dd position)
{
	sscanf_s(communication_get_data, "%lf,%lf,%lf", &position[0], &position[1], &position[2]);
}