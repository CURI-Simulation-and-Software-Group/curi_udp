#include "curi_udp.h"

void udp_pack(udp_node* p, char send_data[])
{
	strcpy(p->send_buffer, send_data);
}

void udp_unpack(udp_node* p, char recieve_data[])
{
	strcpy(recieve_data, p->recieve_buffer);
}

int main()
{
	udp_node comunication1;
	if (!udp_init(&comunication1, "127.0.0.1", 13331, 13332)) {
		printf("comunication1 create.\n");
	}
	else {
		printf("comunication1 create failure!\n");
	}

	udp_node comunication2; 
	if (!udp_init(&comunication2, "127.0.0.1", 13332, 13331)) {
		printf("comunication1 create.\n");
	}
	else {
		printf("comunication1 create failure!\n");
	}

	udp_pack(&comunication1, "123");
	udp_send(&comunication1);

	udp_receive(&comunication2);
	udp_print(&comunication2);

	udp_close(&comunication1);
	udp_close(&comunication2);

	return 0;
}