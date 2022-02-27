#include "AsciiFlixDefinitions.h"
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cstring>
#include <string>
#include <thread>
#include <iostream>


#define PORT 8080

using namespace std;

// Establish a control connection with a new user
void welcomeUser(int sock_id)
{
	std::cout << "Inside welcomeUser for user "<< sock_id << " !!!\n";
    
    //print menu


    // get choosen option    
    int rec_bytes = -1, choice = 0;
    char buffer[4] = {0};
    CHECK_RET(read(sock_id , buffer, 4) > 0, "Failed to get option from socket");
    choice = static_cast<int>(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
	std::cout << "User has chose option: "<< choice << " !!!\n";

    
}


int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	std::string hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
    while(1)
    {
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        std::cout << "A Thread has been launched" << endl;

        // This thread is launched by using
        // function pointer as callable
        thread th1(welcomeUser, new_socket);
        th1.detach();

        // Wait for the threads to finish
        // Wait for thread t1 to finish
        // th1.join();
    }

	
	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello.c_str() , hello.length() , 0 );
	printf("Hello message sent\n");
	return 0;
}
