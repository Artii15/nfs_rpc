#include "client_interface.h"
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}

	char* serverName = argv[1];
	clientInit(serverName);

	int fd = open("/home/artur/pgadmin.log", O_RDONLY, 0);
	close(fd);

	printf("%d\n", fd);

	clientFinish();
	exit(0);
}
