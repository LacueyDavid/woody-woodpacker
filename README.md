# woody-woodpacker

Implementation of ChaCha20 (RFC 8439 profile) in C.

## What is implemented

- ChaCha20 stream cipher core (20 rounds)
- Key setup for 128-bit and 256-bit keys
- Nonce/IV setup with RFC 8439 layout
- Byte-oriented encrypt/decrypt helpers
- Raw keystream generation helper

## Important profile choices

- IV/nonce size is fixed to 96 bits (12 bytes)
- Block counter size is 32 bits
- Max data per key+nonce pair: 2^32 blocks = 256 GB

Never reuse the same key+nonce pair across different messages.

## Build and run

Build binary:

```bash
make
```

Run tests/demo:

```bash
make test
```

Clean build artifacts:

```bash
make clean
```

## Public API

The API is declared in `include/ecrypt_sync.h`.

### ECRYPT_keysetup

```c
void ECRYPT_keysetup(ECRYPT_ctx *context, const u8 *key, u32 key_bits);
```

- `context`: cipher context to initialize
- `key`: key bytes
- `key_bits`: 128 or 256

Notes:

- 256-bit is recommended.
- In 128-bit mode, the internal layout follows the classic ChaCha convention.

### ECRYPT_ivsetup

```c
void ECRYPT_ivsetup(ECRYPT_ctx *context, const u8 *iv);
```

- `iv`: exactly 12 bytes (96 bits)

Behavior:

- Resets block counter to 0
- Loads nonce in state words 13..15

### ECRYPT_encrypt_bytes

```c
void ECRYPT_encrypt_bytes(ECRYPT_ctx *context, const u8 *message, u8 *cipher,
              u32 bytes);
```

- Encrypts `bytes` from `message` into `cipher`
- Supports in-place operation (`message == cipher`)

### ECRYPT_decrypt_bytes

```c
void ECRYPT_decrypt_bytes(ECRYPT_ctx *context, const u8 *cipher, u8 *message,
              u32 bytes);
```

- Same operation shape as encryption (XOR stream cipher)

### ECRYPT_keystream_bytes

```c
void ECRYPT_keystream_bytes(ECRYPT_ctx *context, u8 *stream, u32 bytes);
```

- Writes raw keystream bytes to `stream`

## Typical usage

```c
#include "ecrypt_sync.h"

void encrypt_then_decrypt_example(void) {
  ECRYPT_ctx ctx;

  u8 key[32] = {
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
      0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
      0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
      0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

  u8 nonce[12] = {
      0x00, 0x00, 0x00, 0x00,
      0x4a, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00};

  u8 msg[] = "Hello ChaCha20";
  u8 out[sizeof(msg) - 1];

  // 1) Initialize key and nonce
  ECRYPT_keysetup(&ctx, key, 256);
  ECRYPT_ivsetup(&ctx, nonce);

  // 2) Encrypt
  ECRYPT_encrypt_bytes(&ctx, msg, out, (u32)(sizeof(msg) - 1));

  // 3) Reset nonce+counter before decrypting the same message
  ECRYPT_ivsetup(&ctx, nonce);
  ECRYPT_decrypt_bytes(&ctx, out, msg, (u32)(sizeof(msg) - 1));
}
```

## Security notes

- ChaCha20 provides confidentiality, not authenticity.
- For authenticated encryption, pair it with a MAC or use ChaCha20-Poly1305.
- Always use a unique 96-bit nonce per message for a given key.
