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
	memcpy(str, date+11, 53);
}
int main(int argc, char* argv[])
{
	WSADATA wsadata;
	SOCKET mySocket;
	struct sockaddr_in server;
	char* server_reply[2000];
	int recv_size;
	char date[53];

	if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) {
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

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);


	if (connect(mySocket, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("error at connecting");
		return 1;
	}

	puts("Connected");

	
	while (1)
	{
		getDate(date);
		Sleep(1000);
		if (send(mySocket, date, 54, 0) < 0)
		{
			puts("Send failed");
			return -1;
		}
		printf("sent: %s \n", date);
		Sleep(1000);
	}
	return 0;

}
