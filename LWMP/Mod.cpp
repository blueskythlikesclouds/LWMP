#include "Server.h"
#include <cstdio>
#include <thread>
#include <ws2ipdef.h>
#include <WS2tcpip.h>
#include "WinSocket.h"
#include "Client.h"

void receiveTest()
{
	Server* server = new Server(42069);
	char ip[INET_ADDRSTRLEN];
	uint32_t addr = server->getAddress();
	inet_ntop(AF_INET, &addr, ip, sizeof(ip));
	printf("%s:%d\n", ip, server->getPort());
	char data[32];
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		memset(data, 0, 32);
		printf("Help ");
		printf("%d ", server->receive((uint8_t*)data, 31));
		printf("%s\n", data);
		// I NEED TO FORWARD THE PORT BUT I CAN'T REMEMBER THE ROUTER'S PASSWORD
	}
}

extern "C" void __declspec(dllexport) __cdecl Init(const void* data)
{
	if (!WinSocket::startup())
	{
		fprintf(stderr, "Init(): WinSocket::startup() failed\n");
		return;
	}
	
	//new std::thread(receiveTest);
	//receiveTest();
	Client client("sajid's ip ;)", 5930);
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		const char* data = "your mum $#$#!$ sajid";
		client.send((uint8_t*)data, strlen(data) + 1);
	}
}
