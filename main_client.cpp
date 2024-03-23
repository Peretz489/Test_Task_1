#include <sys/socket.h>
#include <iostream>
#include "client.h"
#include <vector>
#include <memory>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Port value required to run";
        return 1;
    }

    std::string port{*argv[1]};
    std::vector<std::unique_ptr<Client>> clients;
    for (int request_interval = 1; request_interval <= 5; request_interval++)
    {
        clients.emplace_back(std::make_unique<Client>(std::atoi(port.c_str()),request_interval));
    }

    Client test_client(3333, 1); // remove hardcoded port number
    test_client.Connect();
    while (true)
    {
        std::string text_to_send;
        std::getline(std::cin, text_to_send);
        test_client.Send(text_to_send);
    }
}