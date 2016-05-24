struct OpenRequest {
	string fileName<>;
	int flags;
	unsigned mode;
};

struct FileAccessRequest {
	int offset; 
	unsigned count;
	struct OpenRequest fileAttributes;
};

struct WriteRequest {
	opaque content<>;
	struct FileAccessRequest requestAttributes;
};

struct OperationStatus {
	int returnValue;
	int error;
};

struct ReadResponse {
	opaque content<>;
	struct OperationStatus status;
};

struct LseekRequest {
	struct OpenRequest fileAttributes;
	int oldOffset;
	int newOffset;
	int whence;
};

program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		struct OperationStatus rOpen(struct OpenRequest) = 1;
		struct ReadResponse rRead(struct FileAccessRequest) = 2;
		struct OperationStatus rWrite(struct WriteRequest) = 3;
		struct OperationStatus rLseek(struct LseekRequest) = 4;
	} = 1;
} = 0x20000001;
