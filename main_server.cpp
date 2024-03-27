#include <sys/socket.h>
#include <iostream>
#include "server.h"
#include <unistd.h> 
#include <logger.h>

#include <termios.h>        //termios, TCSANOW, ECHO, ICANON 

int main(int argc, char **argv)
{
    const std::string log_filename = "log.txt";
    if (argc != 2)
    {
        std::cout << "Usage: server [listening port number]\n";
        return 1;
    }
    Logger* Logger = Logger::Init();
    Server server(argv[1], log_filename);
    if (!server.Start()){
        std::cerr<<"Exiting...";
        return 1;
    }
    //------this part is not required, but...-----
    std::cout<<"Press eny key to stop\n";
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::cin.get();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    //-------------------------------------------
    server.Stop();
    std::cout << "Server stopped\n";
}