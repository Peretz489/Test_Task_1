#include "logger.h"
#include <iostream>
#include <chrono>
#include <sstream>

// Logger::~Logger(){
// }

Logger *Logger::Init()
{
    static Logger inst;
    return &inst;
}

void Logger::SetLogName(const std::string &filename)
{
    _log_filename = filename;
}

bool Logger::WriteToLog(const std::string &event)
{
    std::lock_guard<std::mutex> guard(_mu);
    std::fstream out(_log_filename, std::ios::app);
    out << GetTime() << " " << event << std::endl;
    _out.close();
    return true; // remove
}

std::string Logger::GetTime()
{
    using std::chrono::system_clock;
    auto currentTime = std::chrono::system_clock::now();
    char buffer[80];

    auto transformed = currentTime.time_since_epoch().count() / 1000000;

    auto millis = transformed % 1000;

    std::time_t tt;
    tt = system_clock::to_time_t(currentTime);
    auto timeinfo = localtime(&tt);
    strftime(buffer, 80, "%F %H:%M:%S", timeinfo);
    std::stringstream out;
    out << buffer << ".";
    if (millis == 0)
    {
        out << "000";
    }
    else if (millis < 10)
    {
        out << "00" << millis;
    }
    else if (millis < 100)
    {
        out << "0" << millis;
    }
    else
    {
        out << millis;
    };

    return out.str();
}