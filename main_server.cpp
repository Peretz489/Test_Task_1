#include <sys/socket.h>
#include <iostream>
#include "server.h"
#include <thread>

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     std::cout << "Port value required to run";
    //     return 1;
    // }
    Server test_server(3333);
    test_server.Start();
    bool exit_flag = false;
    std::thread t1 (Read,test_server.GetSocket(), std::ref(exit_flag));
    // std::string s;
    // if (!exit_flag){
    // std::getline(std::cin,s);
    // }
    exit_flag = true;
    std::cout<<"server stopped\n";
    t1.join();
}