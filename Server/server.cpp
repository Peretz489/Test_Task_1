#include "server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <sys/poll.h>
#include <unistd.h>

const int MAX_CONNECTIONS = 100;
const std::string LOCAL_ADDRESS = "127.0.0.1";
const int POLL_TIMEOUT = 10;


Server::Server(std::string port, const std::string &log_name)
    : _port(port)
{
    _logger->SetLogName(log_name);
}

bool Server::Start()
{
    struct addrinfo info;
    struct addrinfo *server_info;
    std::memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
    if (int status = getaddrinfo(LOCAL_ADDRESS.c_str(), _port.c_str(), &info, &server_info) != 0)
    {
        std::cerr << "Error in getaddrinfo\n";
        return false;
    }
    _socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (int binded = bind(_socket, server_info->ai_addr, server_info->ai_addrlen) != 0)
    {
        std::cerr << "Socket not binded\n";
        return false;
    }
    listen(_socket, MAX_CONNECTIONS);

    _accepter = std::thread([this]
                            {                    
    while (!this->_stop_command)
    {
        sockaddr_storage descriptor;
        std::memset(&descriptor, 0, sizeof(info));
auto client_ptr = std::make_unique<Client>(Client{descriptor, sizeof(sockaddr_storage), 0});

        client_ptr->socket = accept(this->_socket, (struct sockaddr *)&client_ptr->descriptor, &client_ptr->descriptor_size);
        if (client_ptr->socket==-1){
            break;
        }
        client_ptr->status=ClientStatus::CONNECTED;
        _mu.lock();
        this->_connected_clients.emplace_back(std::move(client_ptr));
        _mu.unlock();
    } 
    });

    _pooler = std::thread([this]
                          {                 
    while (!this->_stop_command)
    {
        auto It = _connected_clients.begin();
        while(It!=_connected_clients.end())
        {
            if(It->get()->status==ClientStatus::DISCONNECTED){
                auto next_client=std::next(It);
                _mu.lock();
                _connected_clients.erase(It);
                _mu.unlock();
                It=next_client;
                continue;
            }    
            struct pollfd descriptor = {It->get()->socket, POLLIN, POLL_TIMEOUT};
            int ret = poll(&descriptor, 1, POLL_TIMEOUT); 
            if (ret == -1)
            {
                std::cerr << "poll error occured\n";
                break;
            }
            else if (ret == 0)
            {
                ++It;
                continue;
            }
            else
            {
                this->ReadData(It);
            }
            ++It;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_TIMEOUT));
    } 
    });
    return true;
}

void Server::Stop() noexcept
{
    std::cout<<"Stop command recieved by server"<<std::endl;
    _stop_command = true;
     shutdown(_socket, 0);
    for (auto &client : _connected_clients)
    {
        close(client->socket);
    }
    close(_socket);
    _pooler.join();
    _accepter.join();
}

void Server::ReadData(ClientIt It)
{
    _reader = std::thread([this, It]
                          {

    char buff[256] = {0};
    int bytes_recieved = recv(It->get()->socket, buff, 256, 0);
    if (bytes_recieved == 0)
    {
        It->get()->status = ClientStatus::DISCONNECTED;
        return;
    } else if (bytes_recieved == -1)
    { 
        if(!_stop_command){
        std::cerr << "Client data read error\n";
        }
    }
    else
    {
        this->_logger->WriteToLog(std::string{buff});
        std::memset(buff, 0, 256);
    } });
    _reader.detach();
}

Logger *Server::_logger = Logger::Init();