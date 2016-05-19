struct OpenRequest {
	char* fileName;
	int flags;
	unsigned mode;
};

struct CreatRequest {
	char* fileName;
	unsigned mode;
};

struct FileAccessRequest {
	char* fileName; 
	off_t offset; 
	size_t count;
};

struct ReadResponse {
	int bytesRead;
	opaque content<100>;
};

program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		int rOpen(struct OpenRequest) = 1;
		int rCreat(struct CreatRequest) = 2;
		struct ReadResponse rRead(struct FileAccessRequest) = 3;
		ssize_t rWrite(struct FileAccessRequest) = 4;
	} = 1;
} = 0x20000001;
