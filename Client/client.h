#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string>

class Client{

public:
explicit Client (uint16_t port, int request_interval);
~Client();

void Connect();
int Send(std::string& out_text);

private:
uint16_t _port;
int _request_interval;
std::string _client_name;
int _socket;
};