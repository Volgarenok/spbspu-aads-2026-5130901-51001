#include "sha1_c.h"
#include <string.h>

#define SHA1CircularShift(bits, word) \
(((word) << (bits)) | ((word) >> (32 - (bits))))

int SHA1Reset(SHA1Context *context) {
  if (!context) {
    return 1;
  }
  
  context->Length_Low = 0;
  context->Length_High = 0;
  context->Message_Block_Index = 0;
  
  context->Intermediate_Hash[0] = 0x67452301;
  context->Intermediate_Hash[1] = 0xEFCDAB89;
  context->Intermediate_Hash[2] = 0x98BADCFE;
  context->Intermediate_Hash[3] = 0x10325476;
  context->Intermediate_Hash[4] = 0xC3D2E1F0;
  
  context->Computed = 0;
  context->Corrupted = 0;
  
  return 0;
}

static void SHA1ProcessMessageBlock(SHA1Context *context) {
  const uint32_t K[] = {
    0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6
  };
  int t;
  uint32_t temp;
  uint32_t W[80];
  uint32_t A, B, C, D, E;
  
  for (t = 0; t < 16; t++) {
    W[t] = ((uint32_t)context->Message_Block[t * 4]) << 24;
    W[t] |= ((uint32_t)context->Message_Block[t * 4 + 1]) << 16;
    W[t] |= ((uint32_t)context->Message_Block[t * 4 + 2]) << 8;
    W[t] |= ((uint32_t)context->Message_Block[t * 4 + 3]);
  }
  
  for (t = 16; t < 80; t++) {
    W[t] = SHA1CircularShift(1, W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
  }
  
  A = context->Intermediate_Hash[0];
  B = context->Intermediate_Hash[1];
  C = context->Intermediate_Hash[2];
  D = context->Intermediate_Hash[3];
  E = context->Intermediate_Hash[4];
  
  for (t = 0; t < 80; t++) {
    if (t < 20) {
      temp = (B & C) | ((~B) & D);
    } else if (t < 40) {
      temp = B ^ C ^ D;
    } else if (t < 60) {
      temp = (B & C) | (B & D) | (C & D);
    } else {
      temp = B ^ C ^ D;
    }
    
    uint32_t T = SHA1CircularShift(5, A) + temp + E + W[t] + K[t/20];
    E = D;
    D = C;
    C = SHA1CircularShift(30, B);
    B = A;
    A = T;
  }
  
  context->Intermediate_Hash[0] += A;
  context->Intermediate_Hash[1] += B;
  context->Intermediate_Hash[2] += C;
  context->Intermediate_Hash[3] += D;
  context->Intermediate_Hash[4] += E;
  
  context->Message_Block_Index = 0;
}

int SHA1Input(SHA1Context *context, const uint8_t *message_array, unsigned int length) {
  if (!length) {
    return 0;
  }
  
  if (!context || !message_array) {
    return 1;
  }
  
  if (context->Computed) {
    context->Corrupted = 1;
    return 1;
  }
  
  if (context->Corrupted) {
    return context->Corrupted;
  }
  
  while (length--) {
    context->Message_Block[context->Message_Block_Index++] = (*message_array & 0xFF);
    
    context->Length_Low += 8;
    if (context->Length_Low == 0) {
      context->Length_High++;
      if (context->Length_High == 0) {
        context->Corrupted = 1;
      }
    }
    
    if (context->Message_Block_Index == 64) {
      SHA1ProcessMessageBlock(context);
    }
    
    message_array++;
  }
  
  return 0;
}

static void SHA1PadMessage(SHA1Context *context) {
  if (context->Message_Block_Index > 55) {
    context->Message_Block[context->Message_Block_Index++] = 0x80;
    while (context->Message_Block_Index < 64) {
      context->Message_Block[context->Message_Block_Index++] = 0;
    }
    SHA1ProcessMessageBlock(context);
    while (context->Message_Block_Index < 56) {
      context->Message_Block[context->Message_Block_Index++] = 0;
    }
  } else {
    context->Message_Block[context->Message_Block_Index++] = 0x80;
    while (context->Message_Block_Index < 56) {
      context->Message_Block[context->Message_Block_Index++] = 0;
    }
  }
  
  context->Message_Block[56] = (uint8_t)(context->Length_High >> 24);
  context->Message_Block[57] = (uint8_t)(context->Length_High >> 16);
  context->Message_Block[58] = (uint8_t)(context->Length_High >> 8);
  context->Message_Block[59] = (uint8_t)(context->Length_High);
  context->Message_Block[60] = (uint8_t)(context->Length_Low >> 24);
  context->Message_Block[61] = (uint8_t)(context->Length_Low >> 16);
  context->Message_Block[62] = (uint8_t)(context->Length_Low >> 8);
  context->Message_Block[63] = (uint8_t)(context->Length_Low);
  
  SHA1ProcessMessageBlock(context);
}

int SHA1Result(SHA1Context *context, uint8_t Message_Digest[20]) {
  int i;
  
  if (!context || !Message_Digest) {
    return 1;
  }
  
  if (context->Corrupted) {
    return context->Corrupted;
  }
  
  if (!context->Computed) {
    SHA1PadMessage(context);
    for (i = 0; i < 64; i++) {
      context->Message_Block[i] = 0;
    }
    context->Length_Low = 0;
    context->Length_High = 0;
    context->Computed = 1;
  }
  
  for (i = 0; i < 20; i++) {
    Message_Digest[i] = (uint8_t)(context->Intermediate_Hash[i>>2] >> 8 * (3 - (i & 0x03)));
  }
  
  return 0;
}
