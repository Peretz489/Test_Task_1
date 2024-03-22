#include <sys/socket.h>
#include <iostream>
#include "client.h"

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     std::cout << "Port value required to run";
    //     return 1;
    // }
    Client test_client(3333,1);
    test_client.Connect();
    while (true){
        std::string text_to_send;
        std::getline(std::cin,text_to_send);
        test_client.Send(text_to_send);
    }
}