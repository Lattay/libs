/* Taken from https://stackoverflow.com/a/21001712/6324751
 * Rewritten to fit the format of other algorithms in this directory.
 *
 * This is the basic CRC-32 calculation with some optimization but no
 * table lookup. The byte reversal is avoided by shifting the crc reg
 * right instead of left and by using a reversed 32-bit word to represent
 * the polynomial.
 * When compiled to Cyclops with GCC, this function executes in 8 + 72n
 * instructions, where n is the number of bytes in the input message. It
 * should be doable in 4 + 61n instructions.
 * If the inner loop is strung out (approx. 5*8 = 40 instructions),
 * it would take about 6 + 46n instructions. */

#include <stdlib.h>
#include <stdint.h>
// expect a null terminated string
uint32_t hash(const char* content);

// use a length parameter to determine the length of the buffer
uint32_t hashn(const uint8_t* content, size_t length);

#ifdef CRC32_IMPLEMENTATION

uint32_t hash(const char* content){
  size_t length = 0;
  while(*(content + length) != 0) length++;
  return hashn(content, length);
}

uint32_t hashn(const uint8_t* content, size_t length){
   int j;
   unsigned int crc, mask;

   crc = 0xFFFFFFFF;
   for(int i = 0; i < length; i++){
      uint8_t byte = content[i];   // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
   }
   return ~crc;
}

#endif
