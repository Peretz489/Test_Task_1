#pragma once
#include <fstream>
#include <string>
#include <mutex>

class Logger
{
public:
    static Logger* Init();
    void SetLogName(const std::string &filename);
    bool WriteToLog(const std::string &event);

private:
    Logger()= default;
    ~Logger();
    Logger(Logger &other) = delete;
    Logger &operator=(Logger &other) = delete;
    
    std::mutex _mu;
    std::string _log_filename;
    std::fstream _out;
};