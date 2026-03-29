#include "ecrypt_sync.h"
#include <stdint.h>

static const u8 sigma[16] = {'e', 'x', 'p', 'a', 'n', 'd', ' ', '3',
                             '2', '-', 'b', 'y', 't', 'e', ' ', 'k'};
static const u8 tau[16] = {'e', 'x', 'p', 'a', 'n', 'd', ' ', '1',
                           '6', '-', 'b', 'y', 't', 'e', ' ', 'k'};
void chacha20_keysetup(chacha20_ctx *context, const u8 *key, u32 key_bits){
  const u8 *selected_constants = sigma;
  const u8 *second_half = key + 16;

  // Load the first 16 key bytes into state words 4..7.
  context->state[4] = U8TO32_LITTLE(key + 0);
  context->state[5] = U8TO32_LITTLE(key + 4);
  context->state[6] = U8TO32_LITTLE(key + 8);
  context->state[7] = U8TO32_LITTLE(key + 12);

  // Accept 128-bit and 256-bit keys. Any other value defaults to 256-bit mode.
  if (key_bits == 256) {
    selected_constants = sigma;
  } else if (key_bits == 128) {
    selected_constants = tau;
    second_half = key;
  }

  // Load key bytes into state words 8..11.
  context->state[8] = U8TO32_LITTLE(second_half + 0);
  context->state[9] = U8TO32_LITTLE(second_half + 4);
  context->state[10] = U8TO32_LITTLE(second_half + 8);
  context->state[11] = U8TO32_LITTLE(second_half + 12);

  // Load constants into state words 0..3.
  context->state[0] = U8TO32_LITTLE(selected_constants + 0);
  context->state[1] = U8TO32_LITTLE(selected_constants + 4);
  context->state[2] = U8TO32_LITTLE(selected_constants + 8);
  context->state[3] = U8TO32_LITTLE(selected_constants + 12);
}

void chacha20_ivsetup(chacha20_ctx *context, const u8 *iv) {
  // RFC 8439 layout:
  // state[12] = 32-bit block counter
  // state[13..15] = 96-bit nonce
  context->state[12] = 0;
  context->state[13] = U8TO32_LITTLE(iv + 0);
  context->state[14] = U8TO32_LITTLE(iv + 4);
  context->state[15] = U8TO32_LITTLE(iv + 8);
}

static void chacha20_block(u8 output[64], const u32 state[16]) {
  u32 x[16];
  int i;

  for (i = 0; i < 16; ++i)
    x[i] = state[i];

  // 20 rounds: 10 column-round + diagonal-round pairs.
  for (i = 20; i > 0; i -= 2) {
    QUARTERROUND(0, 4, 8, 12)
    QUARTERROUND(1, 5, 9, 13)
    QUARTERROUND(2, 6, 10, 14)
    QUARTERROUND(3, 7, 11, 15)

    QUARTERROUND(0, 5, 10, 15)
    QUARTERROUND(1, 6, 11, 12)
    QUARTERROUND(2, 7, 8, 13)
    QUARTERROUND(3, 4, 9, 14)
  }

  for (i = 0; i < 16; ++i)
    x[i] = PLUS(x[i], state[i]);

  for (i = 0; i < 16; ++i)
    U32TO8_LITTLE(output + 4 * i, x[i]);
}

void chacha20_encrypt_bytes(chacha20_ctx *context, const u8 *message, u8 *cipher,
                          u32 bytes) {
  u8 keystream[64];
  u32 i;

  if (!bytes)
    return;

  for (;;) {
    chacha20_block(keystream, context->state);

    // RFC 8439 uses a 32-bit block counter at state[12].
    // Callers must not exceed 2^32 blocks (256 GB) for a single key+nonce.
    context->state[12] = PLUSONE(context->state[12]);

    if (bytes <= 64) {
      for (i = 0; i < bytes; ++i) {
        cipher[i] = message[i] ^ keystream[i];
      }
      return;
    }

    for (i = 0; i < 64; ++i) {
      cipher[i] = message[i] ^ keystream[i];
    }

    bytes -= 64;
    cipher += 64;
    message += 64;
  }
}

void chacha20_decrypt_bytes(chacha20_ctx *context, const u8 *cipher, u8 *message, u32 bytes)
{
  // ChaCha20 decryption is identical to encryption (XOR stream cipher).
  chacha20_encrypt_bytes(context, cipher, message, bytes);
}

void chacha20_keystream_bytes(chacha20_ctx *context, u8 *stream, u32 bytes)
{
  u32 i;

  for (i = 0; i < bytes; ++i) {
    stream[i] = 0;
  }

  chacha20_encrypt_bytes(context, stream, stream, bytes);
}
