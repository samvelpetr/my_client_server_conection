#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/in.h>


#define DEFAULT_PORT 1604
#define ERROR_S "SERVER ERROR"
#define CLIENT_EXIT_SYMBOL '#'
#define BUFFER_SIZE 1024


bool is_client_connection_closed(const char * msg){
    for(int i = 0; i < strlen(msg); i++){
        if(msg[i] == CLIENT_EXIT_SYMBOL){
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv){
    int client;
    int server;
    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client <0){
        std::cout << ERROR_S << "establishing socket error" << std::endl;
        exit(0);
    }
    std::cout << "SOCKET for SERVER was successfully created" << std::endl;

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    int ret =bind(client, reinterpret_cast<struct sockaddr*>(&server_address),sizeof(server_address));
    if(ret < 0){
        std::cout << ERROR_S << "bind failed";
        return -1;
    }
    socklen_t size = sizeof(server_address);
    std::cout << "Server listening on ";
    listen(client,1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);

    if(server <0){
        std::cout << ERROR_S << "accept failed" << std::endl;
    
    }
    char buffer[BUFFER_SIZE];
    bool isExit =false;
    while(server > 0){
        strcpy(buffer ,"=> Server connected!\n");
        send(server, buffer ,BUFFER_SIZE,0);
        std::cout << "=> Server connected to client #1" << std::endl <<"Enter # to end connection";

        std::cout << "\NClient: ";
        recv(server,buffer,BUFFER_SIZE,0);
        std::cout <<buffer <<std::endl;
        if(is_client_connection_closed(buffer)){
            isExit = true;
            
        }
        while(!isExit){
            std::cout <<"Server: ";
            std::cin.getline(buffer,BUFFER_SIZE);
            send(server,buffer,BUFFER_SIZE,0);
            if(is_client_connection_closed(buffer))
                break;
            
            std::cout <<"Client: ";
            recv(server,buffer,BUFFER_SIZE,0);
            std::cout <<buffer <<std::endl;
            if(is_client_connection_closed(buffer)){
                break;
            }
           
        }
        std::cout <<"Goodbye!\n";
        isExit = false;
        exit(1);
            
    }
    return 0;
} 