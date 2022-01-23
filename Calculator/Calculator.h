#ifndef CALCULATOR_H_
#define CALCULATOR_H_


#include "..\StringLibrary\StringLibrary.h"
#include "..\Math\MathTree\MathTree.h"


struct Calculator
{
    Text text;
    MathTree tree;
};


bool CalculatorConstructor(Calculator* calc, const char* fileName);

void CalculatorDestructor(Calculator* calc);

#endif 