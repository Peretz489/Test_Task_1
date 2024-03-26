#include "server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <sys/poll.h>
#include <thread>
#include <sstream>
#include <unistd.h>

void AcceptConnections(int socket, std::list<int> &accepted_sockets)
{
    struct sockaddr_storage incoming;
    socklen_t addr_size = sizeof(incoming);
    std::mutex mu;
    while (true)
    {
        mu.lock();
        accepted_sockets.push_back(accept(socket, (struct sockaddr *)&incoming, &addr_size));
        // if (setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) == -1)
        //     return false;
        // if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &ka_conf.ka_idle, sizeof(ka_conf.ka_idle)) == -1)
        //     return false;
        // if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &ka_conf.ka_intvl, sizeof(ka_conf.ka_intvl)) == -1)
        //     return false;
        // if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &ka_conf.ka_cnt, sizeof(ka_conf.ka_cnt)) == -1)
        //     return false;
        std::cout << accepted_sockets.back() << std::endl;
        mu.unlock();
    }
}

Server::Server(uint16_t port)
    : _port(port)
{
    std::cerr << "Server created\n";
}
Server::~Server()
{
    shutdown(_socket, 0);
    for (auto socket : _accepted_sockets)
    {
        close(socket);
    }
    close(_socket);
}

void Server::Start()
{
    struct addrinfo info;
    struct addrinfo *server_info;
    std::memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
    if (int status = getaddrinfo("127.0.0.1", "3333", &info, &server_info) != 0)
    {
        std::cerr << "Ересь в getaddrinfo\n";
        return;
    }
    _socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (int binded = bind(_socket, server_info->ai_addr, server_info->ai_addrlen) != 0)
    {
        std::cerr << "Not binded\n";
        return;
    }
    listen(_socket, 3); // max number of clients awating connection = 2

    _accepter = std::thread([this]
                            {                    
    struct sockaddr_storage incoming;
    socklen_t addr_size = sizeof(incoming);
    std::mutex mu;
    while (true)
    {
        mu.lock();
        this->_accepted_sockets.push_back(accept(this->_socket, (struct sockaddr *)&incoming, &addr_size));
        std::cout << this->_accepted_sockets.back() << std::endl;
        mu.unlock();
        if(this->_stop_command){
            break;
        }
    } });

    _pooler = std::thread([this]
                          {                 
    while (true)
    {
        for (auto socket : this->_accepted_sockets)
        {
            if (this->_stop_command)
                {
                    std::cout << "exit signal recieved\n";
                    return;
                }
            struct pollfd descriptor = {socket, POLLIN, 0};
            int ret = poll(&descriptor, 1, 100);
            if (ret == -1)
            {
                std::cout << "poll error occured, exiting\n";
                break;
            }
            else if (ret == 0)
            {
                continue; // socket closed?
            }
            else
            {
                this->ReadData(socket);
            }
        }
    } });
}

void Server::Stop() noexcept
{
    _stop_command = true;
    _pooler.join();
    _accepter.join();
}

const std::list<int> &Server::GetSockets() const noexcept
{
    return _accepted_sockets;
}

void Server::ReadData(int socket)
{
    _reader = std::thread([this, socket]
                          {

    char buff[256] = {0};
    int bytes_recieved = recv(socket, buff, 256, 0);
    if (bytes_recieved == 0)
    {
        std::cout << "client closed connection\n";
        return;
    }
    else if (bytes_recieved == -1)
    {
        std::cout << "recieve error\n";
    }
    else
    {
        std::ostringstream out;
        out << "recieved: " << buff << "\nbytes=" << bytes_recieved << std::endl;
        this->_logger->WriteToLog(out.str());
        memset(buff, 0, 256);
    } });
    _reader.detach();
}

Server &Server::SetLogName(const std::string &filename)
{
    _logger->SetLogName(filename);
    return *this;
}

Logger *Server::_logger = Logger::Init();