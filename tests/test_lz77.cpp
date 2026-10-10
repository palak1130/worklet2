#include <stdio.h>

#include "../include/LZ77.h"
#include "../include/Token.h"

static int testBasicCompression()
{
    const char* input =
        "ABABABABABABABCD";

    int inputSize = 16;

    TokenArray tokens;

    initTokenArray(&tokens);

    int result =
        compressLZ77(
            input,
            inputSize,
            &tokens
        );

    if (result == 0)
    {
        printf(
            "TEST FAILED: compression error\n"
        );

        freeTokenArray(&tokens);

        return 0;
    }

    if (tokens.size == 0)
    {
        printf(
            "TEST FAILED: no tokens generated\n"
        );

        freeTokenArray(&tokens);

        return 0;
    }

    printf(
        "TEST PASSED: LZ77 generated %u tokens\n",
        tokens.size
    );

    freeTokenArray(&tokens);

    return 1;
}

int main()
{
    if (testBasicCompression())
    {
        printf("All tests passed.\n");
        return 0;
    }

    printf("Tests failed.\n");

    return 1;
}