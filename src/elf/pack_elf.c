#include "woody.h"
#include <stdio.h>
#include <elf.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int handle_elf64(s_packer_ctx *ctx);

int pack_elf(s_packer_ctx *ctx)
{
	handle_elf64(ctx);
    create_woody(ctx);
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

Elf64_Phdr *find_entry_segment(Elf64_Phdr *program_header, size_t count);

int handle_elf64(s_packer_ctx *ctx)
{
    Elf64_Ehdr *elf_header;
    size_t     elf_original_entry;
    Elf64_Phdr *program_header;
    Elf64_Phdr *entry_segment_ptr;

    elf_header = (Elf64_Ehdr *)ctx->filemap;
    program_header = (Elf64_Phdr *)(ctx->filemap + elf_header->e_phoff);

    elf_original_entry = elf_header->e_entry;
    entry_segment_ptr = find_entry_segment(program_header, elf_header->e_phnum, elf_original_entry);
    if (!entry_segment_ptr) {
        return (1);
    }

    elf_original_entry = elf_header->e_entry;

    //unsigned int size = 0x1000;
    //unsigned int offset = phdr[i].p_offset + phdr[i].p_filesz;

    ///* 3. alloc nouveau fichier */
    //void *new_file = malloc(ctx->filesize + size);
    //if (!new_file)
    //    return (-1);

    ///* 4. copier avant cave */
    //memcpy(new_file, ctx->filemap, offset);

    ///* 5. zero cave */
    //memset((char *)new_file + offset, 0, size);

    ///* 6. copier reste */
    //memcpy((char *)new_file + offset + size,
    //       ctx->filemap + offset,
    //       ctx->filesize - offset);

    ///* ⚠️ RELOAD HEADERS SUR NEW FILE */
    //elf_header = (Elf64_Ehdr *)new_file;
    //phdr = (Elf64_Phdr *)((char *)new_file + elf_header->e_phoff);
    //Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)new_file + elf_header->e_shoff);

    ///* 7. FIX OFFSETS */

    //for (int j = 0; j < elf_header->e_phnum; j++)
    //    if (phdr[j].p_offset > offset)
    //        phdr[j].p_offset += size;

    //for (int j = 0; j < elf_header->e_shnum; j++)
    //    if (shdr[j].sh_offset > offset)
    //        shdr[j].sh_offset += size;

    //if (elf_header->e_shoff > offset)
    //    elf_header->e_shoff += size;

    ///* 🔥 8. INJECTION PAYLOAD */

    //Elf64_Off payload_offset = phdr[i].p_offset + phdr[i].p_filesz;

    //memcpy((char *)new_file + payload_offset, payload, PAYLOAD_SIZE);

    ///* 🔥 9. REDIRECTION ENTRYPOINT */

    //elf_header->e_entry = phdr[i].p_vaddr + phdr[i].p_filesz;

    ///* 🔥 10. AGRANDIR SEGMENT */

    //phdr[i].p_filesz += PAYLOAD_SIZE;
    //phdr[i].p_memsz += PAYLOAD_SIZE;

    ///* 11. update ctx */
    //ctx->filemap = new_file;
    //ctx->filesize += size;
    //printf("size = %ld\n", phdr[i].p_filesz);

    return (0);
}

Elf64_Phdr *find_entry_segment(Elf64_Phdr *program_header, size_t count, size_t entry)
{
    for (size_t i = 0; i < count; i++) {
        if (phdr[i].p_type != PT_LOAD)
            continue;

        if (entry >= phdr[i].p_vaddr && entry < phdr[i].p_vaddr + phdr[i].p_memsz) {
            return &phdr[i];
        }
    }

    return (NULL);
}
