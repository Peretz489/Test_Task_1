#include "logger.h"



Logger::~Logger(){
    _out.close();
}

Logger *Logger::Init()
{
    static Logger inst;
      return &inst;
}

void Logger::SetLogName(const std::string& filename)
{
    _log_filename = filename;
}


bool Logger::WriteToLog(const std::string &event)
{
    std::lock_guard<std::mutex> guard(_mu);
    std::fstream out(_log_filename, std::ios::app);
    out<<event<<std::endl;
    return true; //remove
}

