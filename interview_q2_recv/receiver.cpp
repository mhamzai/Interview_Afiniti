#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

int main()
{
	WSADATA wsa_data;
	SOCKADDR_IN server_addr, client_addr;
	string buf = "";
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	const auto server = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);

	bind(server, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(server, 0);

	cout << "Listening for incoming connections..." << endl;

	int client_addr_size = sizeof(client_addr);

		SOCKET client;
		int data_size = 0;
		client = accept(server, reinterpret_cast<SOCKADDR*>(&client_addr), &client_addr_size);
		char buffer[10000];
		cout << "Client connected!" << endl;
		int get_dat;
		ofstream myfile("td1.jpg", ios::out | ios::in | ios::app | ios::binary);
		do
		{
			memset(buffer, 0, sizeof(buffer));
			get_dat = recv(client, buffer, sizeof(buffer), 0);
			if (get_dat < 0) { break; }
			for (int i = 0; i < get_dat; i++)
			{
				cout << buffer[i];
				myfile << buffer[i];
			}
			data_size += get_dat;
			buf += buffer;
		} while (get_dat > 1);

		//string s(buffer, data_size);
		
		cout << "data size = " << data_size << endl;
		
		myfile.close();

		closesocket(client);
		cout << "Client disconnected." << endl;
}
