// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include "AsciiFlixDefinitions.h"

#define PORT 8080
#define MOVIE_PORT 7070

using namespace std;

// Menu printing
void printMenu()
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
	cout << "Please enter your choice:" << "\n";
	cout << "1. Change Movie" << "\n";
	cout << "2. Ask For Premium" << "\n";
	cout << "3. Return To Watch" << "\n";
	cout << "4. Quit Program" << "\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
}

bool intToBytes(int paramInt, char* o_int_bytes)
{
    std::cout << "~~~~~~~~~~~~~~~~\n";
    for (int i = 0; i < 4; i++)
    {
    o_int_bytes[3 - i] = (paramInt >> (i * 8));
    printf("%02x\n", o_int_bytes[3 - i]);
    }
    return true;
}

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	std::string hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	while (1)
	{
		// Print menu
		printMenu();

		// Reiceve option
		char option_bytes[4] = {0};
		int chosen_option{};
		cin >> chosen_option;
		CHECK_RET(chosen_option <= 4 && chosen_option >= 1, "Invalid option, Aborted.");

		// Serial and send to server
		if (!intToBytes(chosen_option, option_bytes))	
		{
			printf("\nConvertion Failed \n");
			return -1;
		}
		send(sock , option_bytes, 4 , 0);


		int client_movie_fd;
		struct sockaddr_in address_movie;
		char buffer_movie[1024] = {0};

		if ((client_movie_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
		{
			perror("UDP socket failed");
			exit(EXIT_FAILURE);
		}
		
		// // Forcefully attaching socket to the port 7070
		// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		// 											&opt, sizeof(opt)))
		// {
		// 	perror("setsockopt");
		// 	exit(EXIT_FAILURE);
		// }
		address_movie.sin_family = AF_INET;
		address_movie.sin_addr.s_addr = INADDR_ANY;
		address_movie.sin_port = htons(MOVIE_PORT);
		
		// Forcefully attaching socket to the port 7070
		if (bind(client_movie_fd, (struct sockaddr *)&address_movie,
									sizeof(address_movie))<0)
		{
			perror("bind failed");
			exit(EXIT_FAILURE);
		}

		unsigned int len, n;
   
		len = sizeof(address_movie);  //len is value/resuslt

		printf("Now recieving data from Client...\n");
		while(1)
		{
			n = recvfrom(client_movie_fd, (char *)buffer_movie, 1024, 
						MSG_WAITALL, ( struct sockaddr *) &address_movie, static_cast<socklen_t*>(&len));
			buffer_movie[n] = '\0';
			printf("%s", buffer_movie);
			usleep(300000);
			system("clear");
		}

	}

	return 0;
}