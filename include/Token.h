#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

struct Token {
    uint32_t offset;
    uint32_t length;
    char nextChar;
}; // struct oken to be replaced by uint32

struct TokenArray {
    Token* data;
    uint32_t size;
    uint32_t capacity;
}; // replace by vector token 

void initTokenArray(TokenArray* array);

int pushToken(TokenArray* array, Token token);

void freeTokenArray(TokenArray* array);

#endif