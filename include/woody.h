#ifndef WOODY_H
#define WOODY_H

/* Includes */
#include <sys/types.h>

/* Macros */
#define ELF_MAG "\x7f""ELF"

/* Structures */
typedef struct {
	const char *progname;
	int        key;  // Ajuster avec le type de la clef
	char       *filename;
	off_t      filesize;
	void       *filemap;
} s_packer_ctx;

/* Functions */
/* parse_args.c */
int parse_args(s_packer_ctx *ctx, int argc, char **argv);

/* packer_ctx.c */
void packer_ctx_init(s_packer_ctx *ctx);
int packer_ctx_setup(s_packer_ctx *ctx);
void packer_ctx_cleanup(s_packer_ctx *ctx);

/* elf/pack_elf.c */
int pack_elf(s_packer_ctx *ctx);

/* woody/create_woody.c */
int create_woody(s_packer_ctx *ctx);

/* packer.c */
int packer(s_packer_ctx *ctx);

#endif
