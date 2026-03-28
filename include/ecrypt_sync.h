#include <stdint.h>

typedef uint32_t u32;

// ---------------------------------------------------------//

// Cipher parameters

#define ECRYPT_NAME "ChaCha20"
#define ECRYPT_PROFILE "Software"

#define ECRYPT_MAXKEYSIZE 256
#define ECRYPT_KEYSIZE(i) (128 + (i) * 128)

//IV stand for initialisation vector
#define ECRYPT_MAXIVSIZE 96
#define ECRYPT_IVSIZE(i) (32 + (i) * 32)

// Structure of the ECRYPT context
typedef struct {
  u32 input[16];
} ECRYPT_ctx;

void chacha20();
