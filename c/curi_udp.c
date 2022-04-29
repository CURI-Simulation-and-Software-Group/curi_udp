
#include "curi_udp.h"

int udp_init(udp_node* p, char local_ip[], int local_port, char remote_ip[], int remote_port)
{
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

	// create UDP socket 
	p->server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	p->server_addr.sin_addr.s_addr = inet_addr(local_ip);
	p->server_addr.sin_port = htons(local_port);
	p->server_addr.sin_family = AF_INET;

	p->client_fd = socket(AF_INET, SOCK_DGRAM, 0);
	p->client_addr.sin_addr.s_addr = inet_addr(remote_ip);
	p->client_addr.sin_port = htons(remote_port);
	p->client_addr.sin_family = AF_INET;

	// bind server address to socket descriptor 
#ifdef WIN32
	if (bind(p->server_fd, (struct sockaddr_in *)&(p->server_addr), sizeof(p->server_addr)) == -1) {
#else
	if (bind(p->server_fd, (SOCKADDR*)&(p->server_addr)), sizeof(p->server_addr)) == -1) {
#endif
		perror("bind error.");
		return 1;
	}
#ifdef WIN32
	int nRecvBuf = MAX_REMOTER_DATA_SIZE;//设置为1K
	if (0 != setsockopt(p->server_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		return FALSE;
	}
	if (0 != setsockopt(cp->client_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		return FALSE;
	}
#endif
	FD_ZERO(&rset);

	return 0;
}

int udp_select(udp_node* p, int usec)
{
	FD_SET(p->server_fd, &(p->rset));
	// select the ready descriptor 
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = usec;
	int nready = select(p->server_fd + 1, &(p->rset), NULL, NULL, &t);
	if (FD_ISSET(p->server_fd, &(p->rset))) {
		memset(p->recieve_buffer, 0, sizeof(p->recieve_buffer));
		p->recieve_size = recv(p->server_fd, p->recieve_buffer, MAX_REMOTER_DATA_SIZE, 0);
	} else {
		p->recieve_size = 0;
	}
	return p->recieve_size;
}

void udp_send(udp_node* p)
{
	sendto(p->client_fd, p->send_buffer, strlen(p->send_buffer), 0,
		(const struct sockaddr_in*)&(p->client_addr), sizeof(p->client_addr));
}

void udp_receive(udp_node* p)
{
	memset(p->recieve_buffer, 0, sizeof(p->recieve_buffer));
	p->recieve_size = recv(p->server_fd, p->recieve_buffer, MAX_REMOTER_DATA_SIZE, 0);
}

void udp_print(udp_node* p)
{
	p->recieve_buffer[p->recieve_size] = 0;
	puts(p->recieve_buffer);
}

void udp_close(udp_node* p)
{
#ifdef WIN32
	closesocket(p->server_fd);
	closesocket(p->client_fd);
	WSACleanup();
#else
	close(p->server_fd);
	close(p->client_fd);
#endif
}