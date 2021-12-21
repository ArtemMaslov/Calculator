#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "StackLite.h"
#include "StackDiagnostic.h"

#include "..\Logs\Logs.h"

#ifdef STACK_LOGS

extern FILE* stackLogFile;

#endif // StackLogs


int ValidateStack(Stack *stack)
{
#ifdef STACK_LOGS
    LogLine(stackLogFile, "ValidateStack", DEBUG);
#endif

    int error = STACKERR_NO_ERRORS;
    if (stack == nullptr)
    {
        error |= STACKERR_PTR_IS_NULL;
#ifdef STACK_LOG_ERRORS
        LogLine(stackLogFile, "WARNING: Stack ptr is null", WARNING);
#endif
    }
    else
    {
        if (stack->data == nullptr && stack->stackCapacity > 0)
        {
            error |= STACKERR_STACK_IS_EMPTY;
#ifdef STACK_LOG_ERRORS
        LogLine(stackLogFile, "WARNING: Stack is empty", WARNING);
#endif
        }
        
        if (stack->stackSize > stack->stackCapacity && stack->stackCapacity != 0)
        {
            error |= STACKERR_SIZE_MORE_CAPACITY;
#ifdef STACK_LOG_ERRORS
        LogLine(stackLogFile, "WARNING: Stack size more capacity", WARNING);
#endif
        }
    }

    return error;
}

int IsStackEmpty(Stack *stack)
{
    assert(stack);

    if (stack->data          == nullptr &&
        stack->stackSize     == 0 &&
        stack->elementSize   == 0 &&
        stack->stackCapacity == 0)
        return STACKERR_NO_ERRORS;
    else
        return STACKERR_STACK_IS_NOT_INITED;
}

bool CheckForError(int errors, StackError errorName)
{
    if ((errors & errorName) > 0)
        return true;
    else
        return false;
}

bool IsStackBroken(int errors, Stack* stack)
{
    if (stack)
    {
        if (errors > 0)
        {
            StackDump(stack, stackLogFile);
            return true;
        }
        return false;
    }
    return true;
}

void StackDump_(Stack *stack, FILE *file,
    const char *stack_variable_name,
    const char *stack_function_name,
    const char *stack_file,
    const int   stack_line,
    const char *programm_function_name,
    const char *programm_file,
    const int   programm_line)
{
#ifdef STACK_LOGS
    LogLine(stackLogFile, "StackDump_", DEBUG);
#endif

    if (!file)
    {
        puts("StackDump called with null file ptr");
        return;
    }

    puts("StackDump called");

    char stackState[4] = "ERR";
    char buffer[140] = "";

    unsigned int stackError = ValidateStack(stack);

    if (file == stackLogFile)
    {
        fseek(file, TextOffset, SEEK_END);
        fputs("<font color=\"17F9FF\">", file);
    }

    if (stack != nullptr && stackError == STACKERR_NO_ERRORS)
        strcpy_s(stackState, "OK");

    sprintf_s(buffer, "StackDump\nStack [0x%p]: %s \"%s\" called ",
        stack,
        stackState,
        stack_variable_name);

    fputs(buffer, file);

    fprintf(file, "from % s() at |%s, %d|\n", stack_function_name, stack_file, stack_line);

    //fprintf(file, "%*sfrom %s() at |%s, %d|\n", strlen(buffer), "", programm_function_name, programm_file, programm_line);

    if (stackError > 0)
    {
        fprintf(file, "Errors:\n");

        int arrayIndex = 1;
        while (stackError > 0)
        {
            if (stackError % 2 > 0)
                fprintf(file, "    * %s\n", StackErrorStrings[arrayIndex]);
            arrayIndex++;
            stackError /= 2;
        }
    }
    fputs("Structure:\n{\n", file);

    if (stack)
    {
        const int leftOffset1 = 4 - 1;// ����� ��������, �������� ������ �����. 4 �������
        const int leftOffset2 = 8 - 1;// ����� ��������, �������� ������ �����. 8 ��������
        
        fprintf(file, "%*s elementSize   = %zd\n", leftOffset1, "", stack->elementSize);
        fprintf(file, "%*s stackSize     = %zd\n", leftOffset1, "", stack->stackSize);
        fprintf(file, "%*s stackCapacity = %zd\n", leftOffset1, "", stack->stackCapacity);
        fprintf(file, "\n    data [0x%p]:\n    {\n", stack->data);

        if (stack->data)
        {
            size_t capacity = stack->stackCapacity;
            size_t size     = stack->stackSize;
            int    *data     = (int*)stack->data;

            size_t numberLength = 0;
            while (capacity > 0)
            {
                capacity /= 10;
                numberLength++;
            }
            capacity = stack->stackCapacity;

            for (size_t st = 0; st < capacity; st++)
            {
                fprintf(file, "%*s %c[%*.1d] = %d\n", leftOffset2, "", (st <= size) ? '*' : ' ', numberLength, st, data[st]);
            }
        }

        fputs("    }\n", file);
    }

    fputs("}\n", file);

    if (file == stackLogFile)
        fputs("</font>\n</body>\n</html>\n", file);

    fflush(file);
}