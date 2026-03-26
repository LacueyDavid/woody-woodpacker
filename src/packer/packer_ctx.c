#include "packer.h"

#include <string.h>   // memset()
#include <fcntl.h>    // open(), O_RDONLY
#include <stdio.h>    // perror()
#include <unistd.h>   // lseek(), close()
#include <sys/mman.h> // mmap(), PROT_READ, PROT_WRITE, MAP_PRIVATE, MAP_FAILED, munmap()

void packer_ctx_init(s_packer_ctx *ctx)
{
	memset(ctx, 0, sizeof(s_packer_ctx));
}

int packer_ctx_setup(s_packer_ctx *ctx)
{
	int fd;

	fd = open(ctx->filename, O_RDONLY);
	if (fd < 0) {
		perror(ctx->filename);
		return (1);
	}

	ctx->filesize = lseek(fd, 0, SEEK_END);
	if (ctx->filesize < 0) {
		perror("lseek()");
		close(fd);
		return (2);
	}

	ctx->filemap = mmap(NULL, ctx->filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (ctx->filemap == MAP_FAILED) {
		perror("mmap()");
		close(fd);
		return (3);
	}

	close(fd);
	return (0);
}

void packer_ctx_cleanup(s_packer_ctx *ctx)
{
	munmap(ctx->filemap, ctx->filesize);
}
