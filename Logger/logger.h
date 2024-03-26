#pragma once
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>

class Logger
{
public:
    static Logger* Init();
    void SetLogName(const std::string &filename);
    bool WriteToLog(const std::string &event);
    tm* GetTime();

private:
    Logger()= default;
    ~Logger()= default;
    Logger(Logger &other) = delete;
    Logger &operator=(Logger &other) = delete;
    
    std::mutex _mu;
    std::string _log_filename;
    std::fstream _out;
};