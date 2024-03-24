#include "client.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <chrono>

Client::Client(std::string port, std::string name, int request_interval)
	: _port(port), _client_name(name), _request_interval(request_interval)
{
	std::cerr << "client " << _client_name << " created\n";
}
Client::~Client()
{
	close(_socket);
}

void Client::Connect()
{
	struct addrinfo info;
	struct addrinfo *server_info;
	std::memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_flags = AI_PASSIVE;
	if (getaddrinfo("localhost", _port.c_str(), &info, &server_info) != 0)
	{
		std::cerr << "Wrong server data\n";
		return;
	}
	_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	//int connected;
	if (connect(_socket, server_info->ai_addr, server_info->ai_addrlen) != 0)
	{
		std::cerr << "No connection\n";
		return;
	}
}

int Client::Send()
{
	Connect();
	int bytes_send = send(_socket, _client_name.c_str(), _client_name.size(), 0);
	std::cout << "send " << bytes_send << " bytes \n";
	close(_socket);
	return bytes_send;
}