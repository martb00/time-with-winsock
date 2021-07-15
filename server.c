#include<stdio.h>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib") 
#pragma warning( disable : 4996)
#include <assert.h>
#include <time.h>
#include <windows.h>


void getDate(char* str) {
	char date[64];
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	assert(strftime(date, sizeof(date), "%c", tm));
	memcpy(str, date + 11, 53);
}

int main(int argc, char* argv[])
{
	WSADATA wsadata;
	SOCKET mySocket;
	struct sockaddr_in servAdd, clientAdd;
	int c;
	char date[53];

	if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0) {
		printf("Winsock init failed\n");
		return 1;
	}
	else
		printf("Winsock init done\n");
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySocket == INVALID_SOCKET)
	{
		printf("Socket creating failed\n");
		WSACleanup();
		return 1;
	}
	else
		printf("Socket was created\n\n");

	servAdd.sin_port = htons(80);
	servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdd.sin_family = AF_INET;

	if (bind(mySocket, (struct sockaddr*)&servAdd, sizeof(servAdd)) == SOCKET_ERROR)
	{
		printf("Bind failed:");
	}


	listen(mySocket, 3);

	puts("Waiting for connection\n");

	c = sizeof(struct sockaddr_in);

	int dataLen;
	char buf[10000];
	int i = 0;
	while (1)
	{
		
		mySocket = accept(mySocket, (struct sockaddr*)&clientAdd, &c);
		if (mySocket == INVALID_SOCKET)
		{
			return 0;
		}
		puts("Connection accepted");
		while ((dataLen = recv(mySocket, buf, 10000, 0)) > 0)
		{
			i = 0;
			while (buf[i] >= 32 || buf[i] == '\n' || buf[i] == '\r')
			{
				printf("%c", buf[i]);
				i += 1;
			}
			getDate(date);
			printf("   Now is %s", date);
			printf("\n");
		}
	}


	closesocket(mySocket);
	WSACleanup();

	return 0;
}
