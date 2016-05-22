struct OpenRequest {
	string fileName<>;
	int flags;
	unsigned mode;
};

struct CreatRequest {
	string fileName<>;
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

program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		struct OperationStatus rOpen(struct OpenRequest) = 1;
		struct OperationStatus rCreat(struct CreatRequest) = 2;
		struct ReadResponse rRead(struct FileAccessRequest) = 3;
		struct OperationStatus rWrite(struct WriteRequest) = 4;
	} = 1;
} = 0x20000001;
