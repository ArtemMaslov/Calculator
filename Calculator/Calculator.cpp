#include <stdio.h>
#include <assert.h>


#include "Calculator.h"
#include "..\Math\MathParser\MathParser.h"
#include "..\StringLibrary\StringLibrary.h"


bool CalculatorConstructor(Calculator* calc, const char* fileName)
{
    assert(calc);

    FILE* file = fopen(fileName, "r");

    if (!file)
    {
        printf("Ошибка открытия файла \"%s\"", fileName);
        return false;
    }

    MathTree tree = {};

    if (!ReadTreeFromFile(&tree, file))
    {
        TreeDestructor(&tree);
        return false;
    }

    return true;
}

void CalculatorDestructor(Calculator* calc)
{
    assert(calc);

    TextDestructor(&calc->text);
    TreeDestructor(&calc->tree);
}