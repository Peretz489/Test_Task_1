#include <sys/socket.h>
#include <iostream>
#include "server.h"
#include <thread>
#include <termios.h>        //termios, TCSANOW, ECHO, ICANON
#include <unistd.h> 

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     std::cout << "Port value required to run";
    //     return 1;
    // }
    Server test_server(3333); //hardcoded port number
    test_server.Start();
    bool exit_flag = false;
    std::thread t1(Read, test_server.GetSocket(), std::ref(exit_flag));
    //-----------check this part carefuly---------
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //std::cout << "Press key to continue....\n";
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::cin.get();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    //----------------------
    exit_flag = true;
    std::cout << "server stopped\n";
    t1.join();
}