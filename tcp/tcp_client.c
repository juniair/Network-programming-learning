#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define CLINET_NAME "[Clinet]"
void excpetion(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	
	char message[BUF_SIZE];

	if(argc != 3)
	{
		printf("Usage %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(sock == -1)
	{
		excpetion("socket() error");
	}

	memset(serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connet(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		excpetion("connet() error");
	}
	
	printf("Welcome to 'Hello Server'\n");
	sprintf(message, "%s: Hi Server!\n", CLINET_NAME);
	printf("%s", message);
	write(sock, message, sizeof(meesage));

	while(1)
	{
		fgets(message, BUF_SIZE, stdin);
		sprinf(message "%s: %s", CLINET_NAME, message);
		if(write(sock, message, sizeof(message)) == 0)
		{
			continue;
