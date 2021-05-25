#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s;
	string line;
	struct sockaddr_in server;
	ifstream myfile("td1.jpg", ios::out | ios::in | ios::app | ios::binary);

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");


	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	if (myfile.is_open())
	{
		string data="";
		while (getline(myfile, line))
		{
			data += line;
		}
		cout << data << endl;
		if (send(s, &data[0], strlen(&data[0]), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		myfile.close();
	}
	send(s, "\0", strlen("\0"), 0);
	puts("Data Send\n");

	return 0;
}