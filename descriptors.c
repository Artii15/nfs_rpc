#include "descriptors.h"

#define MAX_FILENAME_LEN 255
#define DESCRIPTORS_POOL_SIZE_INCREMENT_VALUE 100

struct FileDescriptor {
	char fileName[MAX_FILENAME_LEN];
	int flags;
	mode_t mode;
};

struct DescriptorIdx {
	int idx;
	struct DescriptorIdx* next;
};

static struct FileDescriptor* descriptorsPool = 0;
static int currentPoolSize = 0;
static int nextDescriptorIdx = 0;
static struct DescriptorIdx* freedDescriptorsIds = 0;

int reserveNexDescriptorId();

int open(const char *pathname, int flags, mode_t mode) {
	return reserveNexDescriptorId();
}

int reserveNexDescriptorId() {
	return 0;
}
