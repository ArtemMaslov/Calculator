#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <assert.h>


#include "Logs.h"


#define LOGS_FOLDER "programm logs\\"

struct LogFile
{
    FILE* file;
    const char* sig;
    const char* caption;
    const char* fileName;
};


const int LogFilesSize = 10;
LogFile   LogFiles[LogFilesSize];
size_t    LogFilesIndex = 0;
bool      generalLogCreated = false;

int     TextOffset = 0;

void CreateGeneralLog()
{
    generalLogCreated = true;
    assert(LogConstructor("log_general.txt", "Общий лог программы.", LogSigGeneral));
}

bool LogConstructor(const char* logFileName, const char* caption, const char* signature)
{
    assert(logFileName);
    assert(caption);
    assert(generalLogCreated);

    if (LogFilesIndex >= LogFilesSize)
    {
        LogLine()
        return false;
    }
    LogFilesIndex++;
    logFile = fopen(logFileName, "w");

    if (!logFile)
    {
        printf("Ошибка открытия файла с логами. FileName = \"%s\"", logFileName);
        return false;
    }

    system("rd " LOGS_FOLDER "/s /q");
    system("md " LOGS_FOLDER);

    atexit(LogDestructor);
    
    const size_t bufferSize = 50;
    char buffer[50] = "";
    time_t rawTime = time(nullptr);
    tm* curTime = localtime(&rawTime);

    strftime(buffer, bufferSize, "%H:%M:%S %d.%m.%Y", curTime);

    fprintf(logFile, "<html>\n"
                     "<head><title>%s</title><style>font {line-height: 0.8;} body {background-color: #414A4C;} head {background-color: #414A4C;}</style></head>\n"
                     "<body>\n"
                     "<h1><font color=\"99B333\">%s.</font></h1>\n", caption, buffer);

    TextOffset = ftell(logFile);

    fputs("</body>\n"
          "</html>\n", logFile);
    TextOffset -= ftell(logFile);

    return true;
}

/**
 * @brief      Закрывает файл логов.
*/
void LogDestructor()
{
    assert(generalLogCreated);

    for (size_t st = 0; st < LogFilesIndex; st++)
    {
        LogFile logFile = LogFiles[st];

        assert (logFile.file);
        fclose(logFile.file);
    }
}

/**
 * @brief                    Добавляет строку в файл логов.
 * @param file               Указатель на поток вывода.
 * @param message            Строка, которую необходимо добавить.
 * @param dublicateToConsole Если true, то дублирует сообщения в консоль. По умолчанию false
*/
void $LogLine__(const char* message, LogLevel logLevel, const char* sig, bool dublicateToConsole,
                const char* funcName, const char* fileName, int logLine)
{
    assert(message);
    assert(funcName);
    assert(fileName);
    assert(generalLogCreated);


    static unsigned long long callCounter = 0;
    callCounter++;
    const size_t bufferSize = 1000;
    char  buffer[bufferSize] = "";
    const char* logColor = nullptr;
    
    size_t logFileIndex = 0;
    while (logFileIndex < LogFilesIndex)
    {
        if (strncmp(sig, LogFiles[logFileIndex].sig, LogFileMaxSigSize))
            break;
        logFileIndex++;
    }

    if (logFileIndex >= LogFilesIndex)
    {

        LogLine("Неопознанная сигнатура файла логов", LOG_ERROR, LogSigGeneral, dublicateToConsole);
    }

    switch (logLevel)
    {
        case LOG_DEBUG:
            logColor = "FFA000";
            break;
        case LOG_WARNING:
            logColor = "FF4D00";
            break;
        case LOG_ERROR:
            logColor = "D53032";
            break;
        default:
            logColor = "EAE6CA";
            break;
    }

    time_t rawTime = time(nullptr);
    tm* curTime = localtime(&rawTime);

    size_t timeSize = 10;
    strftime(buffer, bufferSize, "%H:%M:%S ", curTime);
    
    snprintf(buffer + timeSize, bufferSize - timeSize, "<pre>"
            "%0ulld"
            "<font color=\"%s\">[%-.8s] > </font>"
            "<font color = \"BDDA57\">%150s</font>"
            "<font color = \"B0B7C6\"> in %50s</font>"
            "<font color = \"FFD700\"> at %d line</font>\n"
            "<font color = \"ADDFAD\">%s</font>"
            "</pre>\n",
            logColor, LogLevelMessages[(int)logLevel],
            funcName,
            fileName,
            logLine,
            message);

    fseek(logFile, TextOffset, SEEK_END);

    fputs(buffer, logFile);

    if (dublicateToConsole)
        puts(buffer);

    fputs("</body>\n"
          "</html>\n", logFile);

    fflush(logFile);
}

//void CreateHtmlGraphicLog(const char* imagesName)
//{
//    assert(imagesName);
//
//    FILE* file = fopen(GRAPH_LOG_FOLDER "GraphicLog.html", "w");
//    if (!file)
//    {
//        puts("Ошибка открытия файла.");
//        return;
//    }
//
//    const char* pattern = "<html>\n"
//                          "<head><title>Лог программы \"Дифференциатор\".</title><style>font{line - height: 0.8; } body{background - color: #404040; } head{background - color: #404040; }</style></head>\n"
//                          "<body>\n"
//                          "<h1><font color = \"99B333\">Лог программы \"Дифференциатор\".</font></h1>\n";
//
//    const char* ending  = "</body>\n</html>";
//
//    fputs(pattern, file);
//
//    for (int st = 1; st <= CreateGraphCallCount; st++)
//        fprintf(file, "<img src = \"%s%d.png\">\n", imagesName, st);
//
//    fputs(ending, file);
//
//    fclose(file);
//}