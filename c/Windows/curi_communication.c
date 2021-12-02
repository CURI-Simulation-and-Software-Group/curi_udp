#include "pch.h"
#include "curi_communication.h"
#include <Winsock2.h>

#pragma comment(lib, "WS2_32.lib")    // Winsocket是动态链接库，需要加载它的库和包含对应的头文件

#define CURI_SEND_PORT 11230
#define CURI_RECIVE_PORT 11235
#define MAX_REMOTER_DATA_SIZE 256
char communication_get_data[MAX_REMOTER_DATA_SIZE];
char communication_set_data[MAX_REMOTER_DATA_SIZE];
int communication_get_data_size = 0;

SOCKET communication_server = 0;
SOCKET communication_client = 0;
SOCKADDR_IN communication_server_addr;
SOCKADDR_IN communication_client_addr;
fd_set communication_rset;

typedef double hduVector3Dd[3];

int communication_init(const char local_ip[], int local_port, const char remote_ip[], int remote_port)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		return err;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return -1;
	}

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
	if (bind(communication_server, (SOCKADDR*)&communication_server_addr, sizeof(communication_server_addr)) == -1) {
		perror("bind error.");
		return 1;
	}
	FD_ZERO(&communication_rset);
	int nRecvBuf = MAX_REMOTER_DATA_SIZE;//设置为1K
	if (0 != setsockopt(communication_server, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		return FALSE;
	}
	if (0 != setsockopt(communication_client, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		return FALSE;
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
	return communication_get_data_size > 0;
}

void communication_set()
{
	sendto(communication_client, communication_set_data, strlen(communication_set_data), 0,
		(SOCKADDR*)&communication_client_addr, sizeof(communication_client_addr));
}

char* communication_print()
{
	communication_get_data[communication_get_data_size] = 0;
	puts(communication_get_data);
	return communication_get_data;
}

void communication_close()
{
	closesocket(communication_server);
	closesocket(communication_client);
	WSACleanup();
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
