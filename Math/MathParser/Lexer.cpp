#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#include "..\..\Logs\Logs.h"
#include "MathParser.h"


int MathParserConstructor(MathParser* mathParser, Text* text)
{
    assert(mathParser);
    assert(text);

    return AllocTokens(mathParser, minTokenSize);
}

void AddToken(Token* tokens)
{

}

static int AllocTokens(MathParser* mathParser, size_t size)
{
    assert(mathParser);

    Token* arr = (Token*)realloc(mathParser->tokens, sizeof(Token) * size);
    if (!arr)
    {
        LogLine("Не хватает памяти", LOG_ERROR, true);
        return ME_ERR_NOT_ENOUGHT_MEMORY;
    }

    mathParser->tokenCount = size;
}