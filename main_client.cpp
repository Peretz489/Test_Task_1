#include <sys/socket.h>
#include <iostream>
#include "client.h"
#include <string>
#include <chrono>
#include <thread>

struct Parameters
{
    std::string port;
    std::string client_name;
    int pull_interval;
};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "Usage: client -p[server port] -n[client name] -i[pull interval, s].\nAll parameters are required to run\n";
        return 1;
    }
    Parameters params;
    for(int idx=1; idx<4; idx++){
        switch (argv[idx][1]){
            case 'p': {
                //std::string port{argv[idx]};
                char* port = argv[idx];
                //params.port = std::atoi(port.substr(2).c_str()); //not best solution...
                params.port = std::string(port+2);
                continue;
            }
            case 'n':{
                char* name = argv[idx];
                params.client_name = std::string{name+2};
                continue;
            }
            case 'i':{
                char* pull_interval = argv[idx];
                params.pull_interval= std::atoi(pull_interval+2);
                continue;
            }
            default: {
                //std::cout<<argv[idx][1]<<std::endl;
                std::cout<<"Wrong parameter "<<argv[idx]<<". Client stopped.\n";
                return 1;
            }
        }
    }

    Client client(params.port, params.client_name, params.pull_interval);
    client.Connect();
    while (true)
    {
        int bytes_send = client.Send();
        if(bytes_send<=0){
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(params.pull_interval));
    }
}