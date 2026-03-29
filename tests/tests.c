#include "ecrypt_sync.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

void profile_chacha_20_test() {
  int i;
  int keysize;

  printf("\n====================================================\n");
  printf("CHACHA20 CONFIGURATION VERIFICATION\n");
  printf("Description: Validates macro expansion for Key/IV sizes.\n");
  printf("====================================================\n\n");

  printf("TEST CIPHER: %s\n", CHACHA20_NAME);
  printf("Profil: %s\n\n", CHACHA20_PROFILE);

  printf("[Design] Key Injection:\n");
  printf("-> Using a 'No Key Schedule' design to prevent cache-timing "
         "attacks.\n");
  printf("-> Key bits are directly loaded into the 512-bit state matrix "
         "(indices 4-11).\n");
  printf("-> Supports 256-bit native security or 128-bit via internal "
         "duplication.\n\n");
  printf("[key] Verification supported size:\n");
  for (i = 0; CHACHA20_KEYSIZE(i) <= CHACHA20_MAXKEYSIZE; ++i) {
    keysize = CHACHA20_KEYSIZE(i);
    printf("-> Index %d: %d bits (%d octets)\n", i, keysize, keysize / 8);
  }
  printf("\n");

  printf("[Design] 96-bit Nonce (RFC 8439):\n");
  printf("-> Provides 2^96 unique IV combinations per key.\n");
  printf("-> Aligned with modern industry standards like TLS 1.3 and "
         "OpenSSH.\n");
  printf("-> Leaves 32 bits for the block counter, supporting files up to "
         "256 GB.\n\n");
  printf("[IV] Verification supported size:\n");
  printf("-> 96 bits (12 octets)\n");
}

void chacha20_test() {
    chacha20_ctx ctx;
    u8 key[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                  0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                  0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    u8 iv[12] = {
        0xca, 0xfe, 0xba, 0xbe, 
        0xde, 0xad, 0xbe, 0xef,
        0x01, 0x02, 0x03, 0x04
    };
    char *original_text = "Hello Woody! Encrypting this ELF binary.";
    u32 len = (u32)strlen(original_text);
    
    u8 *plaintext = malloc(len + 1);
    u8 *ciphertext = malloc(len);
    u8 *decrypted = malloc(len + 1);

    if (!plaintext || !ciphertext || !decrypted) return;

    // Init
    memcpy(plaintext, original_text, len);
    
    printf("--- ChaCha20 RFC 8439 Test ---\n");
    printf("Original:  %s\n", original_text);

    // --- ENCRYPTION ---
    chacha20_keysetup(&ctx, key, 256);
    chacha20_ivsetup(&ctx, iv); // Reset counter + Set IV
    chacha20_encrypt_bytes(&ctx, (u8*)original_text, ciphertext, len);

    printf("Ciphered: ");
    for (u32 i = 0; i < len; i++) printf("%02x ", ciphertext[i]);
    printf("\n");

    // --- Dchacha20ION ---
    // VERY IMPORTANT: reset context (counter to 0) to decrypt
    chacha20_ivsetup(&ctx, iv); 
    chacha20_decrypt_bytes(&ctx, ciphertext, decrypted, len);
    decrypted[len] = '\0';

    printf("Decrypted: %s\n", decrypted);

    // --- VERIFICATION ---
    if (memcmp(original_text, decrypted, len) == 0) {
        printf("\nSUCCESS: Decrypted text matches original\n");
    } else {
        printf("\nFAILURE: Decrypted text is different\n");
    }

    free(plaintext);
    free(ciphertext);
    free(decrypted);
    printf("\n-------------------------------------\n");
}

int main(void) {

  profile_chacha_20_test();
  printf("\n====================================================\n\n");
  chacha20_test();

  return 0;
}
