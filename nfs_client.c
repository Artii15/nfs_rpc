#include "client_interface.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}

	char* serverName = argv[1];
	clientInit(serverName);

	int fd = open("test.txt", O_RDWR, 0);
	if(fd < 0) {
		perror("Error");
		exit(1);
	}

	char writeBuf[] = {'a', 'b', 'c'};
	if(write(fd, writeBuf, 3) < 0) {
		perror("Error");
		close(fd);
		exit(1);
	}

	char readBuf[100] = {0};
	int bytesRead = read(fd, readBuf, sizeof(readBuf));

	printf("[%dB] %s\n", bytesRead, readBuf);

	lseek(fd, -bytesRead, SEEK_CUR);

	memset(readBuf, 0, sizeof(readBuf));
	bytesRead = read(fd, readBuf, sizeof(readBuf));

	printf("[%dB] %s\n", bytesRead, readBuf);

	if(bytesRead < 0) {
		perror("error");
	}

	close(fd);

	clientFinish();
	exit(0);
}
