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
#define ERROR_S "CLIENT ERROR"
#define CLIENT_EXIT_SYMBOL '#'
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

bool is_client_connection_closed(const char * msg){
    for(int i = 0; i < strlen(msg); i++){
        if(msg[i] == CLIENT_EXIT_SYMBOL){
            return true;
        }
    }
    return false;
}


int main(int argc, char **argv){
    int client;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client <0){
        std::cout << ERROR_S << "establishing socket error" << std::endl;
        exit(0);
    }

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP,&server_address.sin_addr);
    char buffer[BUFFER_SIZE];
    std::cout <<"\n => Client socket is created.";
    int ret =connect(client, reinterpret_cast<sockaddr *> (&server_address), sizeof(server_address));
    if(ret==0){
        std::cout << "=> Connection to server"<<
        inet_ntoa(server_address.sin_addr)
        << "with port number" 
        <<DEFAULT_PORT
        <<"\n";
    }
        std::cout << "=> Waiting for server confirmation...";

        recv(client,buffer,BUFFER_SIZE,0);
        std::cout << "=>Connection established\n"
            << "Enter " <<CLIENT_EXIT_SYMBOL
            <<  " to close the connection"
        ;
        while(true){
            std::cout << "Client: ";
            std::cin.getline(buffer,BUFFER_SIZE);
            send(client,buffer,BUFFER_SIZE,0);
            if(is_client_connection_closed(buffer)){
                break;
            }
            std::cout << "Server: ";
            recv(client,buffer,BUFFER_SIZE,0);
            std::cout << buffer << std::endl;
            if(is_client_connection_closed(buffer)){
                break;
            }
        }
        close(client);
        std::cout <<"Goodbye!\n";
        
    
    return 0;
}   