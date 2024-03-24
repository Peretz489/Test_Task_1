#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>

class Client{

public:
explicit Client (std::string port, std::string name, int request_interval);
~Client();

void Connect();
int Send();

private:
std::string _port;
std::string _client_name;
int _request_interval;
int _socket;
};