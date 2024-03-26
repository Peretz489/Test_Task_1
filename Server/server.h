#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <logger.h>
#include <mutex>
#include <list>
#include <thread>

class Server
{

public:
    explicit Server(uint16_t port);
    ~Server();
    void Start();
    //void AcceptConnections();
    const std::list<int>& GetSockets() const noexcept;
    //static void Poll(const std::list<int>& sockets, bool &exit_flag);
    void ReadData(int socket);
    Server& SetLogName(const std::string& filename);
    void Stop() noexcept;

private:
    uint16_t _port;
    int _socket;
    std::list<int>_accepted_sockets{};
    std::mutex _mu;
    std::string _log_filename;
    static Logger *_logger;

    bool _stop_command=false;

    std::thread _accepter;
    std::thread _reader;
    std::thread _pooler;
};
