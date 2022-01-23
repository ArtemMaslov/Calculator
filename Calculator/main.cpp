#include <stdio.h>
#include <stdlib.h>


#include "Calculator.h"
#include "..\Logs\Logs.h"


int main(int argc, char* argv[])
{
    if (LogsConstructor() != LOG_NO_ERRORS)
    {
        fputs("Ошибка создания файла логов", stderr);
        return false;
    }

    Calculator calc = {};

    CalculatorConstructor(&calc, "test6.txt");

    CalculatorDestructor(&calc);

    LogsDestructor();
}
