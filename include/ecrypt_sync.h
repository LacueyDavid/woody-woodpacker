#ifndef CHACHA20_SYNC_H
#define CHACHA20_SYNC_H

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

#define CHACHA20_NAME "ChaCha20"
#define CHACHA20_PROFILE "Software"

#define CHACHA20_MAXKEYSIZE 256
#define CHACHA20_KEYSIZE(i) (128 + (i) * 128)

//IV stand for initialisation vector
#define CHACHA20_MAXIVSIZE 96
// RFC 8439 profile: only 96-bit IV is supported.
// The macro shape keeps compatibility with common CHACHA20-style loops.
#define CHACHA20_IVSIZE(i) (96 + (i) * 96)

// Structure of the chacha20 context
typedef struct {
  u32 state[16];
} chacha20_ctx;

// ------------------- FUNCTIONS -----------------------//

void chacha20_ivsetup(chacha20_ctx *context, const u8 *iv);
void chacha20_keysetup(chacha20_ctx *context, const u8 *key, u32 key_bits);
void chacha20_keystream_bytes(chacha20_ctx *context, u8 *stream, u32 bytes);
void chacha20_decrypt_bytes(chacha20_ctx *context, const u8 *cipher, u8 *message, u32 bytes);
void chacha20_encrypt_bytes(chacha20_ctx *context, const u8 *message, u8 *cipher,
                          u32 bytes);

#endif
