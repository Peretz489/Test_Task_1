#include "logger.h"
#include <iostream>

// Logger::~Logger(){
// }

std::ostream &operator<<(std::ostream &out, tm *time_p)
{

    out << time_p->tm_year << " " << time_p->tm_mon << " " << time_p->tm_mday << " " << time_p->tm_hour << ":" << time_p->tm_min << ":" << time_p->tm_sec << ": ";
    return out;
}

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
    tm *event_time = GetTime();
    std::lock_guard<std::mutex> guard(_mu);
    std::fstream out(_log_filename, std::ios::app);
    out << event_time << event << std::endl;
    _out.close();
    delete(event_time);
    return true; // remove
}

tm *Logger::GetTime()
{
    time_t now = time(0);
    tm *current_time = localtime(&now);
    return current_time;
}
