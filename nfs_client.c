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

	int fd = open("/home/artur/test.txt", O_RDWR, 0);

	char writeBuf[] = {'a', 'b', 'c'};
	if(write(fd, writeBuf, 3) < 0) {
		perror("Error");
		close(fd);
		exit(1);	
	}

	printf("%d\n", fd);

	char readBuf[100] = {0};
	int bytesRead = read(fd, readBuf, 99);

	printf("%d %s\n", bytesRead, readBuf);

	lseek(fd, -bytesRead, SEEK_CUR);

	char readBuf2[100] = {0};
	int bytesRead2 = read(fd, readBuf2, 99);

	printf("%d %s\n", bytesRead2, readBuf2);

	if(bytesRead < 0) {
		perror("error");
	}

	close(fd);

	clientFinish();
	exit(0);
}
