#include "packer.h"
#include <stdio.h>
#include <elf.h>

int handle_elf64(s_packer_ctx *ctx);

int pack_elf(s_packer_ctx *ctx)
{
	(void)ctx;
	fprintf(stderr, "Ready to pack an elf!\n");
	handle_elf64(ctx);
	return (0);
}

#include <stdint.h>

static void print_padding(void *filemap, uint64_t start, uint64_t end)
{
    unsigned char *data = (unsigned char *)filemap;

    printf("\n=== PADDING DUMP ===\n");
    printf("start: 0x%lx\n", start);
    printf("end:   0x%lx\n", end);
    printf("size:  %ld bytes\n\n", end - start);

    for (uint64_t i = start; i < end; i++)
    {
        printf("%02x ", data[i]);

        if ((i - start + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n====================\n");
}

int handle_elf64(s_packer_ctx *ctx)
{
    Elf64_Ehdr *elf_header;
    Elf64_Phdr *ph;

    elf_header = (Elf64_Ehdr *)ctx->filemap;
    ph = (Elf64_Phdr *)(ctx->filemap + elf_header->e_phoff);

    for (int i = 0; i < elf_header->e_phnum; i++)
    {
        if (ph[i].p_type != PT_LOAD)
            continue;

        uint64_t cur_offset = ph[i].p_offset;
        uint64_t cur_end = cur_offset + ph[i].p_filesz;

        uint64_t next_offset = UINT64_MAX;

        for (int j = 0; j < elf_header->e_phnum; j++)
        {
            if (ph[j].p_type != PT_LOAD)
                continue;

            if (ph[j].p_offset > cur_offset &&
                ph[j].p_offset < next_offset)
            {
                next_offset = ph[j].p_offset;
                printf("i = %d | j = %d\n", i, j);
            }
        }

        if (next_offset != UINT64_MAX)
        {
            uint64_t start = cur_end;
            uint64_t end = next_offset;

            long gap = (long)(end - start);

            if (gap > 0)
            {
                printf("%ld: padding between segments\n", gap);
                print_padding(ctx->filemap, start, end);
            }
        }
    }

    return (0);
}
