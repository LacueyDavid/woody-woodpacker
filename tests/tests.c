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

  printf("[key] Verification supported size:\n");
  for (i = 0; ECRYPT_KEYSIZE(i) <= ECRYPT_MAXKEYSIZE; ++i) {
    keysize = ECRYPT_KEYSIZE(i);
    printf("-> Index %d: %d bits (%d octets)\n", i, keysize, keysize / 8);
  }
  printf("\n");
  printf("[IV] Verification supported size:\n");
  for (i = 0; ECRYPT_IVSIZE(i) <= ECRYPT_MAXIVSIZE; ++i) {
    ivsize = ECRYPT_IVSIZE(i);
    printf("-> Index %d: %d bits (%d octets)\n", i, ivsize, ivsize / 8);
  }
  printf("\n---- End of chacha20 tests ----\n");
  return 0;
}
