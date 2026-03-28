#include <stdio.h>
#include <stdint.h>
#include "ecrypt_sync.h"

// we are going to use macro for performances
// --------------------- MACRO UTILS ------------------- //

// macro for rotate 32 bits left
// Be carefull, v has to be a unsigned int32! (uint32) only!
#define ROTL32(v,n) (((v) << (n)) | (v >> (32 - (n))))

// take 4 octets in the memory to create a u32 (for the reading)
#define U8TO32_LITTLE(p) \
    (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) | \
     ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))
// 
// take a u32 and cut it in 4 oct
#define U32TO8_LITTLE(p, v) \
    do { \
        (p)[0] = (uint8_t)((v) & 0xff); \
        (p)[1] = (uint8_t)(((v) >> 8) & 0xff); \
        (p)[2] = (uint8_t)(((v) >> 16) & 0xff); \
        (p)[3] = (uint8_t)(((v) >> 24) & 0xff); \
    } while (0)ets

// - mathematics found by Daniel J. Bernstein, which create chacha20 logic.
// - 16,12,8 and 7 are numbers optimised to maximise the diffusion.
// - here its ARX (add, rotate, XOR) but its (add, XOR, rotate) here.
#define QUARTERROUND(a, b, c, d) \
    a += b; d ^= a; d = ROTL32(d, 16); \
    c += d; b ^= c; b = ROTL32(b, 12); \
    a += b; d ^= a; d = ROTL32(d, 8);  \
    c += d; b ^= c; b = ROTL32(b, 7);

void chacha20(){
  uint32_t a = 0b11110000;
  printf("%b\n",a);
  a = ROTL32(a,16);
  printf("%b\n",a);
}
