#include "descriptors.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_FILENAME_LEN 255
#define DESCRIPTORS_POOL_SIZE_INCREMENT_VALUE 100

struct FileDescriptor {
	char fileName[MAX_FILENAME_LEN];
	int flags;
	mode_t mode;
	int inUse;
};

struct DescriptorIdx {
	int idx;
	struct DescriptorIdx* next;
};

static struct FileDescriptor* descriptorsPool = 0;
static int currentPoolSize = 0;
static int nextDescriptorIdx = 0;
static struct DescriptorIdx* freedDescriptorsIds = 0;

int useNextDescriptor();
int allocateNewDescriptor();
int reuseOldDescriptor();
void setDescriptorToZero(int descriptorIdx);
void useDescriptor(int descriptorIdx);
int isThereAnyDescriptorToReuse();
int isDescriptorsPoolCreated();
void createDescriptorsPool();
void resizeDescriptorsPool();
void clean();

int reserveDescriptor(const char *pathname, int flags, mode_t mode) {
	int fd = useNextDescriptor();

	struct FileDescriptor* descriptor = &descriptorsPool[fd];
	strncpy(descriptor->fileName, pathname, MAX_FILENAME_LEN - 1);
	descriptor->flags = flags;
	descriptor->mode = mode;

	return fd;
}

int releaseDescriptor(int fd) {
	if(fd < 0 || fd >= currentPoolSize || descriptorsPool[fd].inUse == 0) {
		errno = EBADF;
		return -1;
	}

	struct DescriptorIdx* freedDescriptorIdx = calloc(1, sizeof(struct DescriptorIdx));
	freedDescriptorIdx->idx = fd;
	freedDescriptorIdx->next = freedDescriptorsIds;
	freedDescriptorsIds = freedDescriptorIdx;

	setDescriptorToZero(fd);

	return 0;
}

int useNextDescriptor() {
	if(isThereAnyDescriptorToReuse()) {
		return reuseOldDescriptor();
	}
	else {
		return allocateNewDescriptor();
	}
}

int isThereAnyDescriptorToReuse() {
	return freedDescriptorsIds != 0;
}

int allocateNewDescriptor() {
	if(!isDescriptorsPoolCreated()) {
		createDescriptorsPool();
		atexit(clean);
	}
	else if(nextDescriptorIdx >= currentPoolSize) {
		resizeDescriptorsPool();
	}

	useDescriptor(nextDescriptorIdx);

	return nextDescriptorIdx++;
}

int isDescriptorsPoolCreated() {
	return descriptorsPool != 0;
}

void createDescriptorsPool() {
	currentPoolSize = DESCRIPTORS_POOL_SIZE_INCREMENT_VALUE;
	descriptorsPool = calloc(currentPoolSize, sizeof(struct FileDescriptor));
}

void resizeDescriptorsPool() {
	currentPoolSize += DESCRIPTORS_POOL_SIZE_INCREMENT_VALUE;
	descriptorsPool = realloc(descriptorsPool, currentPoolSize*DESCRIPTORS_POOL_SIZE_INCREMENT_VALUE);
}

int reuseOldDescriptor() {
	int descriptorToUseIdx = freedDescriptorsIds->idx;
	useDescriptor(descriptorToUseIdx);

	struct DescriptorIdx* freedDescriptorsListElementToRemove = freedDescriptorsIds;
	freedDescriptorsIds = freedDescriptorsIds->next;
	free(freedDescriptorsListElementToRemove);

	return descriptorToUseIdx;
}

void useDescriptor(int descriptorIdx) {
	setDescriptorToZero(descriptorIdx);
	descriptorsPool[descriptorIdx].inUse = 1;
}

void setDescriptorToZero(int descriptorIdx) {
	memset(&descriptorsPool[descriptorIdx], 0, sizeof(struct FileDescriptor));
}

void clean() {
	if(isDescriptorsPoolCreated()) {
		free(descriptorsPool);
	}

	while(isThereAnyDescriptorToReuse()) {
		struct DescriptorIdx* nextDescriptor = freedDescriptorsIds->next;
		free(freedDescriptorsIds);
		freedDescriptorsIds = nextDescriptor;
	}
}
