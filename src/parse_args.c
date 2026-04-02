#include "woody.h"

#include <string.h> // strcmp()
#include <stddef.h> // NULL
#include <stdio.h>  // fprintf(), stderr
#include <stdlib.h> // atoi()

#define INVALID_OPTION_FORMAT "%s Error: invalid option '%s'\n"
#define EXPECTING_ARGUMENT_FORMAT "%s: Error: expecting argument after '%s'\n"
#define MULTIPLE_FILES_FORMAT "%s: Error: Multiple files\n"

int parse_args(s_packer_ctx *ctx, int argc, char **argv)
{
	int i;

	ctx->progname = argv[0];

	i = 1;
	while (i < argc) {
		if (argv[i][0] == '-') {
			if (strcmp("-k", argv[i]) == 0) {
				if (i == argc - 1) {
					fprintf(stderr,
						EXPECTING_ARGUMENT_FORMAT,
						ctx->progname,
						argv[i]
					);
					return (1);
				}
				i++;
				ctx->key = atoi(argv[i]);
			} else {
				fprintf(stderr,
					INVALID_OPTION_FORMAT,
					ctx->progname,
					argv[i]
				);
				return (1);
			}
		} else if (ctx->filename == NULL) {
			ctx->filename = argv[i];
		} else {
			fprintf(stderr, MULTIPLE_FILES_FORMAT, ctx->progname);
			return (1);
		}

		i++;
	}

	return (0);
}
