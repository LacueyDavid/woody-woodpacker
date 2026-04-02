#include "woody.h"

#include <fcntl.h>
#include <unistd.h>

int create_woody(s_packer_ctx *ctx)
{
    int fd;

    fd = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd < 0)
        return (1);

    write(fd, ctx->filemap, ctx->filesize);

    close(fd);
    return (0);
}
