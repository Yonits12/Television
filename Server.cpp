#include "Server.h"

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
#include <arpa/inet.h>
#include <netinet/in.h>


#define CONTROL_PORT 8080
#define MOVIE_PORT 7070

using namespace std;


bool Movie::getCurrentFrame(string &o_lined_frame) const
{
	o_lined_frame = "";
	for(int i=0; i<5; ++i)
	{
		o_lined_frame.append(this->m_frame[i]);
		o_lined_frame.append("\n");
	}
	return true;
}

int streamToUser(int bcaster_fd, sockaddr_in user_socket)
{
    fstream stream;
    cout << "Yoni is the Coder\n";
    Movie mv{};
    stream.open(mv.m_path, ios::in);
    string line;
    getline(stream, line);
    
    while(line != ";;;")
    {
        if(stream.is_open())
        {
            for(int i=0; i<5; ++i)
            {
                getline(stream, line);
                mv.m_frame[i] = line;
                cout << mv.m_frame[i] << "\n";
            }
			// sendto(bcaster_fd, mv.m_frame[i].c_str(), mv.m_frame[i].length(),
			// 	MSG_CONFIRM, (const struct sockaddr *) &user_socket, 
			// 		sizeof(user_socket));
            usleep(100000);
            system("clear");
            getline(stream, line);
        }
    }
    
    stream.close();
	return 0;
}


// Establish a control connection with a new user
void welcomeUser(int sock_id)
{
	std::cout << "Inside welcomeUser for user "<< sock_id << " !!!\n";

    // get choosen option    
    int rec_bytes = -1, choice = 0;
    char buffer[4] = {0};
    CHECK_RET(read(sock_id , buffer, 4) > 0, "Failed to get option from socket");
    choice = static_cast<int>(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
	std::cout << "User has chose option: "<< choice << " !!!\n";

	//____________ Establish UDP connection for movie ____________
	int broadcaster_sock_fd;
	char *movie_data = "Movie dataaaaaa";
	struct sockaddr_in servaddr;

	// Creating socket file descriptor
	if ( (broadcaster_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(MOVIE_PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;


	streamToUser(broadcaster_sock_fd, servaddr);
	
	printf("Movie message sent.\n");
	close(broadcaster_sock_fd);
}



bool broadcastMovie(int socket)
{
	return true;
}


int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	int server_movie_fd, movie_socket, valread_movie;
	struct sockaddr_in address;
	struct sockaddr_in address_movie, client_addr;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char buffer_movie[1024] = {0};
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
	address.sin_port = htons(CONTROL_PORT);
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	thread th1;
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
		th1 = thread(welcomeUser, new_socket);
    }

	// Wait for the threads to finish
	// Wait for thread t1 to finish
	th1.join();

	// thread broadcaster{streamToUser};
	// broadcaster.join();
	
	return 0;
}
