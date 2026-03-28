#include <stdio.h>
#include <stdint.h>
#include "ecrypt_sync.h"

// Be carefull, v has to be a unsigned int32! (uint32) only!
#define ROTL32(v,n) (((v) << (n)) | (v >> (32 - (n))))

void chacha20(){
  uint32_t a = 0b11110000;
  printf("%b\n",a);
  a = ROTL32(a,16);
  printf("%b\n",a);
}
