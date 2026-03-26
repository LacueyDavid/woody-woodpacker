#include "packer.h"

#include <string.h> // memcmp()
#include <stdio.h>  // fprintf(), stderr

#define INVALID_FILE_FORMAT "%s: Error: '%s' is an invalid object file\n"

int packer(s_packer_ctx *ctx)
{
	if (memcmp(ctx->filemap, "\x7f""ELF", 4) == 0)
		return (pack_elf(ctx));

	fprintf(stderr, INVALID_FILE_FORMAT, ctx->progname, ctx->filename);
	return (1);
}
