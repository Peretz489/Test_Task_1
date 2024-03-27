#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <logger.h>
#include <mutex>
#include <list>
#include <thread>
#include <map>

class Server
{

    typedef int Socket;
    typedef int AcceptedSocket;
    

    enum class ClientStatus{
        CONNECTED,
        DISCONNECTED
    };

    struct Client
    {
        sockaddr_storage descriptor;
        socklen_t  descriptor_size;
        AcceptedSocket socket;
        ClientStatus status;
    };

    using ClientList = std::list<std::unique_ptr<Client>>;
    using ClientIt = std::list<std::unique_ptr<Client>>::iterator;

public:
    explicit Server(std::string port, const std::string& log_name);
    bool Start();
    void Stop() noexcept;

private:
    void ReadData(ClientIt It);
    std::string _port;
    Socket _socket;
    ClientList _connected_clients;
    std::mutex _mu;
    std::string _log_filename;
    static Logger *_logger;

    bool _stop_command = false;

    std::thread _accepter;
    std::thread _reader;
    std::thread _pooler;
};
