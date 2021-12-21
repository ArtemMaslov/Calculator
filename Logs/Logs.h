#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>

extern int TextOffset;
extern size_t CreateGraphCallCount;

enum LogLevel
{
    LOG_DEBUG   = 0,
    LOG_WARNING = 1,
    LOG_ERROR   = 2
};

const char LogLevelMessages[][8] = 
{
    "DEBUG",
    "WARNING",
    "ERROR"
};

const size_t LogFileMaxSigSize = 3;
const char* LogSigGeneral = "GEN";

/**
 * @brief             Конструктор файла логов.
 * @param logFileName Имя выходного файла с логами.
 * @param caption     Заголовок файла логов.
*/
bool LogConstructor(const char* logFileName, const char* caption);

/**
 * @brief      Закрывает файл логов.
*/
void LogDestructor();

void CreateHtmlGraphicLog(const char* imagesName);

/**
 * @brief Внутренняя функция. Используйте LogLine взамен.
*/
void $LogLine__(const char* message, LogLevel logLevel, const char* sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine);

#define LogLine(message, logLevel, sig, dublicateToConsole)                          \
    $LogLine__(message, logLevel, sig, dublicateToConsole, __FUNCSIG__, __FILE__, __LINE__);


#endif // !LOGS_H_
