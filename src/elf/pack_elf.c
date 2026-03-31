#include "packer.h"
#include <stdio.h>
#include <elf.h>

int pack_elf(s_packer_ctx *ctx)
{
	(void)ctx;
	fprintf(stderr, "Ready to pack an elf!\n");
	return (0);
}

int handle_elf64(s_packer_ctx *ctx)
{
	Elf64_Ehdr *elf_header;
	Elf64_Shdr *section_header;
	char       *shstrtab;

	elf_header = (Elf64_Ehdr *)ctx->filemap;

	sections = (Elf64_shdr *)(ctx->filemap + elf_header->e_shoff);
	shstrtab
}
