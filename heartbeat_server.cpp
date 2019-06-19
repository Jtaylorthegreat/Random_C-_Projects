#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <ctime>

//had to include stdlib to compile on rhel7

using namespace std;
 
 
void hblisten (int clientSocket){
    char buf[4096];
    memset(buf, 0, 4096);
    int clienthblisten = recv(clientSocket, buf, 4096, 0);
	int clientok;
	time_t now = time(0);
	char* servtime = ctime(&now);
    if (clienthblisten == -1) {
        cerr << "Error receiving client HB" << endl;
        exit(1);
    }
    if (clienthblisten == 0) {
        cout << "Client disconnected" << endl;
        exit(1);
    }
	if (clienthblisten >1){
		clientok == 1;
	}
		
 cout << "Last successful client heartbeat " << servtime << endl;
}
 
int main(int argc, char *argv[])
{
    
    //check if port number is provided
    if (argc != 2) {
        cerr << "no port number provided after command" << endl;
        return -1;
    }
    
    int mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mysocket == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
    
        
    
    
    // Bind the ip address and port to a socket
    int serverport;
    serverport = atoi(argv[1]);
    sockaddr_in tech;
    tech.sin_family = AF_INET;
    tech.sin_port = htons(serverport);
    tech.sin_addr.s_addr = INADDR_ANY;
    bind(mysocket, (sockaddr*)&tech, sizeof(tech));
    
    //added keep alive to socket:
    int optval;
    socklen_t optlen = sizeof(optval);
    setsockopt(mysocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen); 
    
    listen(mysocket, SOMAXCONN);
   
 
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientsocket, pid;
    
    
 while (true) {
    int clientsocket = accept(mysocket, (sockaddr*)&client, &clientSize);
    
        if (clientsocket < 0) {
            cerr << "Error on accept" << endl;
            return -1;
        }
        
        pid = fork ();
         
        if (pid <0) {
             cerr << "error forking" << endl;
             return -1;
        }
        
        if (pid == 0) {
            close(mysocket);
            hblisten(clientsocket);
            exit (0);
        }
        else {
            close(clientsocket);
        }
 
 }
 
 return 0;
 
}
