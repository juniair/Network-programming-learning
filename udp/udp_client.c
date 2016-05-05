#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define CLIENT_NAME "[Client]"
#define SERVER_NAME "[Server]"

void error_handling(char* message);

int main(int argc, char *argv[])
{
	int sock;
	char serv_msg[BUF_SIZE], clnt_msg[BUF_SIZE];
	socklen_t addr_size;
	int str_len;
	struct sockaddr_in serv_addr, from_addr;
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while(1)
	{
		printf("%s: ", CLIENT_NAME);
		fgets(clnt_msg, sizeof(clnt_msg), stdin);

		sendto(sock, clnt_msg, strlen(clnt_msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

		if(!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			break;
		}

		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, serv_msg, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_size);
		serv_msg[str_len] = 0;
		if(!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
		{
			printf("Server Shutdown.\n");
			break;
		}
		printf("%s: %s", SERVER_NAME, serv_msg);
	}
	close(sock);
	exit(0);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
