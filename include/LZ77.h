#ifndef LZ77_H
#define LZ77_H

#include <stdint.h>
#include "Token.h"

#define LZ77_WINDOW_SIZE 32768 // 32KB
#define LZ77_LOOKAHEAD_SIZE 258 // 258 bytes

#define LZ77_HASH_SIZE 65536 //this needs to be figured out
#define LZ77_MAX_HASH_DEPTH 64 

int compressLZ77( // is this related to container??
    const char* input, 
    int inputSize,
    TokenArray* output
);

#endif