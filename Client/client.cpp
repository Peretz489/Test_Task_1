#include "client.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

Client::Client(uint16_t port, int request_interval)
	: _port(port), _request_interval(request_interval)
{
	std::cerr << "client created\n";
}
Client::~Client(){
	close(_socket);
}

void Client::Connect()
{
	int status;
	struct addrinfo info;
	struct addrinfo *server_info;
	std::memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_flags = AI_PASSIVE;
	if (status = getaddrinfo("localhost", "3333", &info, &server_info) != 0)
	{
		std::cerr << "Wrong data in getadddrinfo\n";
		return;
	}
	_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	int connected;
	if (connected = connect(_socket, server_info->ai_addr, server_info->ai_addrlen) != 0)
	{
		std::cerr << "No connection\n";
		return;
	}
}

int Client::Send(std::string& out_text){
	int bytes_send = send(_socket, out_text.c_str(), out_text.size(), 0);
	std::cout << "send " << bytes_send << " bytes \n";
	return bytes_send;
}