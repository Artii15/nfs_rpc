program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		int r_open(const char *, int, unsigned) = 1;
		int r_creat(const char *, unsigned) = 2;
		ssize_t r_read(int, void*, size_t) = 3;
		ssize_t r_write(int, const void*, size_t) = 4;
		off_t r_lseek(int, off_t, int) = 5;
		int r_close(int) = 6;
	} = 1;
} = 0x20000001;
