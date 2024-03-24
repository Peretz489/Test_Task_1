#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <logger.h>
#include <mutex>

class Server
{

public:
    explicit Server(uint16_t port);
    ~Server();
    void Start();
    int GetSocket() const noexcept;
    static void Poll(int socket, bool &exit_flag);
    static void ReadData(int socket, Logger* logger);
    Server& SetLogName(const std::string& filename);

private:
    uint16_t _port;
    int _socket;
    int _accept_socket;
    std::mutex _mu;
    std::string _log_filename;
    static Logger *_logger;
};
