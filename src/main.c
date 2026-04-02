#include "woody.h"

#include <stdio.h> // fprintf(), stderr

int main(int argc, char **argv)
{
	s_packer_ctx ctx;

	if (argc < 2) {
		fprintf(stderr, "%s: Error: Invalid number of arguments\n",
			argv[0]);
		return (1);
	}

	packer_ctx_init(&ctx);

	if (parse_args(&ctx, argc, argv) != 0)
		return (2);

	if (packer_ctx_setup(&ctx) != 0)
		return (3);

	if (packer(&ctx) != 0)
		return (4);

	packer_ctx_cleanup(&ctx);
	return (0);
}
