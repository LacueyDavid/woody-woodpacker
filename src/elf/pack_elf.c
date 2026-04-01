#include "packer.h"
#include <stdio.h>
#include <elf.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int handle_elf64(s_packer_ctx *ctx);
int write_woody(s_packer_ctx *ctx);

int pack_elf(s_packer_ctx *ctx)
{
	(void)ctx;
	fprintf(stderr, "Ready to pack an elf!\n");
	handle_elf64(ctx);
	fprintf(stderr, "Ready to pack execute!\n");
    write_woody(ctx);
	return (0);
}

char payload[] =
    "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x12\x00\x00\x00\x0f\x05"
    "\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0\x3c\x0f\x05\x2e\x2e"
    "\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

#define PAYLOAD_SIZE sizeof(payload)

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int write_woody(s_packer_ctx *ctx)
{
    int fd = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd < 0)
        return (-1);

    write(fd, ctx->filemap, ctx->filesize);
    close(fd);

    return (0);
}

int handle_elf64(s_packer_ctx *ctx)
{
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)ctx->filemap;
    Elf64_Phdr *phdr = (Elf64_Phdr *)(ctx->filemap + elf_header->e_phoff);

    int i;

    uint64_t original_entry = elf_header->e_entry;
    printf("original entry = %ld\n", original_entry);
    /* 1. trouver segment executable */
    for (i = 0; i < elf_header->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD && (phdr[i].p_flags & PF_X))
            break;
    }
    printf("size = %ld\n", phdr[i].p_filesz);

    /* 2. calcul offset de la cave */
    unsigned int size = 0x1000;
    unsigned int offset = phdr[i].p_offset + phdr[i].p_filesz;

    /* 3. alloc nouveau fichier */
    void *new_file = malloc(ctx->filesize + size);
    if (!new_file)
        return (-1);

    /* 4. copier avant cave */
    memcpy(new_file, ctx->filemap, offset);

    /* 5. zero cave */
    memset((char *)new_file + offset, 0, size);

    /* 6. copier reste */
    memcpy((char *)new_file + offset + size,
           ctx->filemap + offset,
           ctx->filesize - offset);

    /* ⚠️ RELOAD HEADERS SUR NEW FILE */
    elf_header = (Elf64_Ehdr *)new_file;
    phdr = (Elf64_Phdr *)((char *)new_file + elf_header->e_phoff);
    Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)new_file + elf_header->e_shoff);

    /* 7. FIX OFFSETS */

    for (int j = 0; j < elf_header->e_phnum; j++)
        if (phdr[j].p_offset > offset)
            phdr[j].p_offset += size;

    for (int j = 0; j < elf_header->e_shnum; j++)
        if (shdr[j].sh_offset > offset)
            shdr[j].sh_offset += size;

    if (elf_header->e_shoff > offset)
        elf_header->e_shoff += size;

    /* 🔥 8. INJECTION PAYLOAD */

    Elf64_Off payload_offset = phdr[i].p_offset + phdr[i].p_filesz;

    memcpy((char *)new_file + payload_offset, payload, PAYLOAD_SIZE);

    /* 🔥 9. REDIRECTION ENTRYPOINT */

    elf_header->e_entry = phdr[i].p_vaddr + phdr[i].p_filesz;

    /* 🔥 10. AGRANDIR SEGMENT */

    phdr[i].p_filesz += PAYLOAD_SIZE;
    phdr[i].p_memsz += PAYLOAD_SIZE;

    /* 11. update ctx */
    ctx->filemap = new_file;
    ctx->filesize += size;
    printf("size = %ld\n", phdr[i].p_filesz);

    return (0);
}
