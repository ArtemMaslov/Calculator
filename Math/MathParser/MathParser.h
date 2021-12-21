#ifndef MATH_PARSER_H_
#define MATH_PARSER_H_


#include "..\MathExpression\MathExpression.h"
#include "..\MathTree\MathTree.h"
#include "..\..\StringLibrary\StringLibrary.h"
#include "..\..\MParser\MParser.h"


struct Token
{
    Token* next;
    Token* prev;
    MathExpression value;
};

struct MathParser
{
    Token* tokens;
    size_t tokenCount;
};

const size_t minTokenSize = 100;
const double tokenScale = 2;

bool ParseMathTree(Text* text, MathTree* problem);

bool ReadTreeFromFile(MathTree* problem, Text* text, FILE* file);

void WriteTreeToFile(MathTree* problem, FILE* file);


#endif