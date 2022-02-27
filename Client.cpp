// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#define PORT 8080

using namespace std;

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
    char option_bytes[4] = {0};
    if (!intToBytes(7, option_bytes))
    {
        printf("\nConvertion Failed \n");
		return -1;
    }
	send(sock , option_bytes, 4 , 0);
	printf("Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}


// #include "Client.h"
// #include <iostream>
// #include <unistd.h>

// using namespace std;

// int main()
// {
//     cout << "Yoni is the Coder\n";
//     Movie mv{};
//     fstream stream;
//     stream.open(mv.path, ios::in);
//     string line;
//     getline(stream, line);
    
//     while(line != ";;;")
//     {
//         if(stream.is_open())
//         {
//             for(int i=0; i<5; ++i)
//             {
//                 getline(stream, line);
//                 mv.frame[i] = line;
//                 cout << mv.frame[i] << "\n";
//             }
//             usleep(200000);
//             system("clear");
//             getline(stream, line);
//         }
//     }
    
    
//     stream.close();
//     return 0;
// }