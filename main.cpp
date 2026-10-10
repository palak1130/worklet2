#include <stdio.h>
#include <stdlib.h>

#include "include/LZ77.h"
#include "include/Token.h"

static char* readFile(
    const char* filename,
    int* fileSize
)
{
    FILE* file =
        fopen(filename, "rb");

    if (file == 0)
    {
        return 0;
    }

    fseek(file, 0, SEEK_END);

    long size =
        ftell(file);

    fseek(file, 0, SEEK_SET);

    if (size < 0)
    {
        fclose(file);
        return 0;
    }

    char* buffer =
        (char*)malloc(
            (size_t)size
        );

    if (buffer == 0)
    {
        fclose(file);
        return 0;
    }

    size_t bytesRead =
        fread(
            buffer,
            1,
            (size_t)size,
            file
        );

    fclose(file);

    if (bytesRead != (size_t)size)
    {
        free(buffer);
        return 0;
    }

    *fileSize = (int)size;

    return buffer;
}

static void printToken(
    Token token,
    int index
)
{
    printf(
        "Token %d : (%u, %u, '%c')\n",
        index,
        token.offset,
        token.length,
        token.nextChar
    );
}

int main(int argc, char* argv[])
{
    char* input = 0;
    int inputSize = 0;

    /*
     * If a filename is provided,
     * read the file.
     */
    if (argc >= 2)
    {
        input =
            readFile(
                argv[1],
                &inputSize
            );

        if (input == 0)
        {
            printf(
                "Error: Could not read input file.\n"
            );

            return 1;
        }
    }

    /*
     * Otherwise use a small test string.
     */
    else
    {
        const char* test =
            "ABABABABABABABCD";

        inputSize = 16;

        input =
            (char*)malloc(
                inputSize
            );

        if (input == 0)
        {
            return 1;
        }

        for (int i = 0; i < inputSize; i++)
        {
            input[i] = test[i];
        }
    }

    TokenArray tokens;

    initTokenArray(&tokens);

    if (!compressLZ77(
            input,
            inputSize,
            &tokens))
    {
        printf(
            "Error: LZ77 compression failed.\n"
        );

        free(input);

        return 1;
    }

    printf(
        "\n========== LZ77 OUTPUT ==========\n\n"
    );

    printf(
        "Input size  : %d bytes\n",
        inputSize
    );

    printf(
        "Token count : %u\n\n",
        tokens.size
    );

    for (
        uint32_t i = 0;
        i < tokens.size;
        i++
    )
    {
        printToken(
            tokens.data[i],
            (int)i
        );
    }

    printf(
        "\n=================================\n"
    );

    freeTokenArray(&tokens);

    free(input);

    return 0;
}