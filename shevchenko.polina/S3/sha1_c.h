#ifndef SHA1_C_H
#define SHA1_C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SHA1Context {
  uint32_t Intermediate_Hash[5];
  uint32_t Length_Low;
  uint32_t Length_High;
  int_least16_t Message_Block_Index;
  uint8_t Message_Block[64];
  int Computed;
  int Corrupted;
} SHA1Context;

int SHA1Reset(SHA1Context *context);
int SHA1Input(SHA1Context *context, const uint8_t *message_array, unsigned int length);
int SHA1Result(SHA1Context *context, uint8_t Message_Digest[20]);

#ifdef __cplusplus
}
#endif

#endif
