#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include "Config.h"
#include "Exceptions.h"

enum class LogLevel 
{
    INFO, WARNING, ERR, CRITICAL
};

class Logger 
{
private:
    static Logger* instance;
    static std::mutex mtx;
    std::ofstream logFile;
    Logger();

public:
    static Logger* getInstance();
    void log(LogLevel level, const std::string& message);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
