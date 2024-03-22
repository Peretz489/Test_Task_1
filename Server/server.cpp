#include "server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <iostream>


Server::Server(uint16_t port)
    : _port(port)
{
    std::cerr << "Server created\n";
}
Server::~Server()
{
    shutdown(_socket, 0);
}

void Server::Start()
{
    int status;
    struct addrinfo info;
    struct addrinfo *server_info;
    std::memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
    if (status = getaddrinfo("127.0.0.1", "3333", &info, &server_info) != 0)
    {
        std::cerr << "Ересь в getaddrinfo\n";
        return;
    }
    _socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    int binded;
    if (binded = bind(_socket, server_info->ai_addr, server_info->ai_addrlen) != 0)
    {
        std::cerr << "Not binded\n";
        return;
    }
    listen(_socket, 2);
    struct sockaddr_storage incoming;
    socklen_t addr_size = sizeof(incoming);
    _accept_socket = accept(_socket, (struct sockaddr *)&incoming, &addr_size);
}

int Server::GetSocket(){
    return _accept_socket;
}


void Read(int socket, bool &exit_flag){
    char buff[256] = {0};
    while (true)
    {
        int bytes_recieved = recv(socket, buff, 256, 0);
        if(bytes_recieved==0){
            std::cout<<"client closed connection\n";
            exit_flag=true;
            break;
        } else {
        std::cout << "recieved: " << buff << "\nbytes=" << bytes_recieved << std::endl;
        memset(buff, 0, 256);
        }
        if(exit_flag){
            std::cout<<"exit signal recieved\n";
            break;
        }
    }
}