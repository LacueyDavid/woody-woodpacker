#ifndef ECRYPT_SYNC_H
#define ECRYPT_SYNC_H

#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

// --------------------- Macro utils ------------------- //

#define U32V(v) ((u32)(v) & 0xFFFFFFFFU)
// Keep arithmetic in uint32_t space.
#define PLUS(v,w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v),1))
// Rotate left on 32-bit words.
#define ROTL32(v,n) (((v) << (n)) | (v >> (32 - (n))))

// Read 4 bytes as a little-endian uint32_t.
#define U8TO32_LITTLE(p) \
    (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) | \
     ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))

// Write uint32_t into 4 bytes in little-endian order.
#define U32TO8_LITTLE(p, v) \
    do { \
        (p)[0] = (uint8_t)((v) & 0xff); \
        (p)[1] = (uint8_t)(((v) >> 8) & 0xff); \
        (p)[2] = (uint8_t)(((v) >> 16) & 0xff); \
        (p)[3] = (uint8_t)(((v) >> 24) & 0xff); \
    } while (0)

// Core ChaCha quarter-round.
#define QUARTERROUND(a, b, c, d) \
    x[a] = PLUS(x[a], x[b]); x[d] ^= x[a]; x[d] = ROTL32(x[d], 16); \
    x[c] = PLUS(x[c], x[d]); x[b] ^= x[c]; x[b] = ROTL32(x[b], 12); \
    x[a] = PLUS(x[a], x[b]); x[d] ^= x[a]; x[d] = ROTL32(x[d], 8);  \
    x[c] = PLUS(x[c], x[d]); x[b] ^= x[c]; x[b] = ROTL32(x[b], 7);


// ------------------- Profile -----------------------//

#define ECRYPT_NAME "ChaCha20"
#define ECRYPT_PROFILE "Software"

#define ECRYPT_MAXKEYSIZE 256
#define ECRYPT_KEYSIZE(i) (128 + (i) * 128)

//IV stand for initialisation vector
#define ECRYPT_MAXIVSIZE 96
// RFC 8439 profile: only 96-bit IV is supported.
// The macro shape keeps compatibility with common ECRYPT-style loops.
#define ECRYPT_IVSIZE(i) (96 + (i) * 96)

// Structure of the ECRYPT context
typedef struct {
  u32 state[16];
} ECRYPT_ctx;

// ------------------- FUNCTIONS -----------------------//

void ECRYPT_ivsetup(ECRYPT_ctx *context, const u8 *iv);
void ECRYPT_keysetup(ECRYPT_ctx *context, const u8 *key, u32 key_bits);
void ECRYPT_keystream_bytes(ECRYPT_ctx *context, u8 *stream, u32 bytes);
void ECRYPT_decrypt_bytes(ECRYPT_ctx *context, const u8 *cipher, u8 *message, u32 bytes);
void ECRYPT_encrypt_bytes(ECRYPT_ctx *context, const u8 *message, u8 *cipher,
                          u32 bytes);

#endif
