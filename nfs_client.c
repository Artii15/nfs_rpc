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

	int fd = open("/home/artur/test.txt", O_RDONLY, 0);

	printf("%d\n", fd);

	char readBuf[100] = {0};
	int bytesRead = read(fd, readBuf, 99);

	printf("%d %s\n", bytesRead, readBuf);

	if(bytesRead < 0) {
		perror("error");
	}

	close(fd);

	clientFinish();
	exit(0);
}
