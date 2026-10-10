#include "../include/LZ77.h"
#include "../include/HashChain.h"
#include "../include/Token.h"

#include <stdint.h>

static uint32_t hashThreeBytes(
    const char* input,
    int position
)
{
    uint32_t a =
        (uint32_t)(unsigned char)input[position];

    uint32_t b =
        (uint32_t)(unsigned char)input[position + 1];

    uint32_t c =
        (uint32_t)(unsigned char)input[position + 2];

    uint32_t hash =
        a * 257u +
        b * 263u +
        c * 269u;

    return hash & (LZ77_HASH_SIZE - 1);
}

static void insertPosition(
    HashChain* chain,
    const char* input,
    int position,
    int inputSize
)
{
    if (position + 2 >= inputSize)
    {
        return;
    }

    uint32_t hash =
        hashThreeBytes(input, position);

    insertHashPosition(
        chain,
        (int)hash,
        position
    );
}

static int smaller(
    int a,
    int b
)
{
    if (a < b)
    {
        return a;
    }

    return b;
}

int compressLZ77(
    const char* input,
    int inputSize,
    TokenArray* output
)
{
    if (input == 0 || output == 0)
    {
        return 0;
    }

    if (inputSize <= 0)
    {
        return 1;
    }

    HashChain chain;

    if (!initHashChain(
            &chain,
            inputSize,
            LZ77_HASH_SIZE))
    {
        return 0;
    }

    int current = 0;

    while (current < inputSize)
    {
        int bestLength = 0;
        int bestPosition = -1;

        /*
         * We need at least 3 bytes to create
         * a hash-chain entry.
         */
        if (current + 2 < inputSize)
        {
            uint32_t hash =
                hashThreeBytes(
                    input,
                    current
                );

            int candidate =
                getHashHead(
                    &chain,
                    (int)hash
                );

            int windowStart =
                current - LZ77_WINDOW_SIZE;

            if (windowStart < 0)
            {
                windowStart = 0;
            }

            int depth = 0;

            /*
             * Search through previous positions
             * having the same 3-byte hash.
             */
            while (
                candidate >= windowStart &&
                candidate >= 0 &&
                depth < LZ77_MAX_HASH_DEPTH
            )
            {
                int remaining =
                    inputSize - current;

                /*
                 * Leave one character for nextChar.
                 * This means we never need a special
                 * EOF character inside Token.
                 */
                int maximumLength =
                    remaining - 1;

                maximumLength =
                    smaller(
                        maximumLength,
                        LZ77_LOOKAHEAD_SIZE
                    );

                int length = 0;

                /*
                 * Compare the candidate sequence
                 * with the current sequence.
                 *
                 * candidate < current, so overlapping
                 * LZ77 matches are naturally supported.
                 */
                while (
                    length < maximumLength &&
                    input[candidate + length]
                        == input[current + length]
                )
                {
                    length++;
                }

                if (length > bestLength)
                {
                    bestLength = length;
                    bestPosition = candidate;

                    if (length == maximumLength)
                    {
                        break;
                    }
                }

                candidate =
                    getPreviousPosition(
                        &chain,
                        candidate
                    );

                depth++;
            }
        }

        /*
         * CASE 1:
         * A match was found.
         */
        if (bestLength > 0)
        {
            int nextIndex =
                current + bestLength;

            Token token;

            token.offset =
                (uint32_t)(
                    current - bestPosition
                );

            token.length =
                (uint32_t)bestLength;

            token.nextChar =
                input[nextIndex];

            if (!pushToken(output, token))
            {
                freeHashChain(&chain);
                return 0;
            }

            /*
             * Every consumed position is inserted
             * into the hash chain.
             */
            int advance =
                bestLength + 1;

            for (
                int position = current;
                position < current + advance &&
                position < inputSize;
                position++
            )
            {
                insertPosition(
                    &chain,
                    input,
                    position,
                    inputSize
                );
            }

            current += advance;
        }

        /*
         * CASE 2:
         * No match was found.
         *
         * Emit:
         *
         * (0, 0, literal)
         */
        else
        {
            Token token;

            token.offset = 0;
            token.length = 0;
            token.nextChar = input[current];

            if (!pushToken(output, token))
            {
                freeHashChain(&chain);
                return 0;
            }

            insertPosition(
                &chain,
                input,
                current,
                inputSize
            );

            current++;
        }
    }

    freeHashChain(&chain);

    return 1;
}