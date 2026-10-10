#include "../include/Token.h"
#include <stdlib.h>

void initTokenArray(TokenArray* array)
{
    array->data = 0;
    array->size = 0;
    array->capacity = 0;
}

int pushToken(TokenArray* array, Token token)
{
    if (array->size >= array->capacity)
    {
        uint32_t newCapacity;

        if (array->capacity == 0)
        {
            newCapacity = 64;
        }
        else
        {
            newCapacity = array->capacity * 2;
        }

        Token* newData =
            (Token*)malloc(newCapacity * sizeof(Token));

        if (newData == 0)
        {
            return 0;
        }

        for (uint32_t i = 0; i < array->size; i++)
        {
            newData[i] = array->data[i];
        }

        free(array->data);

        array->data = newData;
        array->capacity = newCapacity;
    }

    array->data[array->size] = token;
    array->size++;

    return 1;
}

void freeTokenArray(TokenArray* array)
{
    free(array->data);

    array->data = 0;
    array->size = 0;
    array->capacity = 0;
}
// can we make symbols in this tokens loop 