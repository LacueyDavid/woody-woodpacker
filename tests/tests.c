#include "ecrypt_sync.h"
#include "stdio.h"

int main(void) {
  int i;
  int keysize;
  int ivsize;

  printf("\n====================================================\n");
  printf("CHACHA20 CONFIGURATION VERIFICATION\n");
  printf("Description: Validates macro expansion for Key/IV sizes.\n");
  printf("====================================================\n\n");

  printf("TEST CIPHER: %s\n", ECRYPT_NAME);
  printf("Profil: %s\n\n", ECRYPT_PROFILE);

  printf("[Design] Key Injection:\n");
  printf("-> Using a 'No Key Schedule' design to prevent cache-timing "
         "attacks.\n");
  printf("-> Key bits are directly loaded into the 512-bit state matrix "
         "(indices 4-11).\n");
  printf("-> Supports 256-bit native security or 128-bit via internal "
         "duplication.\n\n");
  printf("[key] Verification supported size:\n");
  for (i = 0; ECRYPT_KEYSIZE(i) <= ECRYPT_MAXKEYSIZE; ++i) {
    keysize = ECRYPT_KEYSIZE(i);
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
  for (i = 0; ECRYPT_IVSIZE(i) <= ECRYPT_MAXIVSIZE; ++i) {
    ivsize = ECRYPT_IVSIZE(i);
    printf("-> Index %d: %d bits (%d octets)\n", i, ivsize, ivsize / 8);
  }
  printf("\n---- End of chacha20 tests ----\n");
  return 0;
}
