#include "Logger.h"

Logger* Logger::instance = nullptr;
std::mutex Logger::mtx;

Logger::Logger() 
{
    logFile.open(Config::LOG_FILE, std::ios::app);
    if (!logFile.is_open()) 
    {
        throw FileIOException("Cannot open log file: " + Config::LOG_FILE);
    }
}

Logger* Logger::getInstance() 
{
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) 
    {
        instance = new Logger();
    }
    return instance;
}

void Logger::log(LogLevel level, const std::string& message) 
{
    std::lock_guard<std::mutex> lock(mtx);

    std::string levelStr;
    int color;

    switch (level) 
    {
    case LogLevel::INFO:
        levelStr = "INFO";
        color = Config::COLOR_DEFAULT;
        break;
    case LogLevel::WARNING:
        levelStr = "WARNING";
        color = Config::COLOR_WARNING;
        break;
    case LogLevel::ERR:
        levelStr = "ERR";
        color = Config::COLOR_ERROR;
        break;
    case LogLevel::CRITICAL:
        levelStr = "CRITICAL";
        color = Config::COLOR_ERROR;
        break;
    }

    std::string logEntry = "[" + Config::getCurrentDateTime() + "] [" + levelStr + "] " + message;

    logFile << logEntry << std::endl;
    logFile.flush();

    Config::setColor(color);
    std::cout << logEntry << std::endl;
    Config::resetColor();
}

Logger::~Logger() 
{
    if (logFile.is_open()) 
    {
        logFile.close();
    }
}