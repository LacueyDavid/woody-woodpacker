#include <stdint.h>

typedef uint32_t u32;

// Structure of the ECRYPT context
typedef struct {
  u32 input[16];
} ECRYPT_ctx;

void chacha20();
