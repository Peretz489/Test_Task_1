#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>

class Server
{

public:
    explicit Server(uint16_t port);
    ~Server();
    void Start();
    int GetSocket() const noexcept;

private:
    uint16_t _port;
    int _socket;
    int _accept_socket;
};

void Read(int socket, bool& exit_flag);