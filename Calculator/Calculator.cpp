#include <stdio.h>
#include <assert.h>


#include "Calculator.h"
#include "..\StringLibrary\StringLibrary.h"

bool CalculatorConstructor(Calculator* calc, const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (!file)
    {
        printf("Ошибка открытия файла \"%s\"", fileName);
        return false;
    }

    if (!ReadFile(&calc->text, file))
        return false;


}