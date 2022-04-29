#include "curi_udp.h"

int main()
{
	udp_node comunication1;
	if (!udp_init(&comunication1, "172.0.0.1", 13331, "172.0.0.1", 13332)) {
		printf("comunication1 create.\n");
	}
	else {
		printf("comunication1 create failure!\n");
	}

	udp_node comunication2; 
	if (!udp_init(&comunication2, "172.0.0.1", 13332, "172.0.0.1", 13331)) {
		printf("comunication1 create.\n");
	}
	else {
		printf("comunication1 create failure!\n");
	}


	udp_close(&comunication1);
	udp_close(&comunication2);

	return 0;
}