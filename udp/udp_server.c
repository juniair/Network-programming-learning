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
	int serv_sock;
	char serv_msg[BUF_SIZE], clnt_msg[BUF_SIZE];
	socklen_t clnt_addr_size;
	int str_len;

	struct sockaddr_in serv_addr, clnt_addr;

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error.");
	}

	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, clnt_msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		clnt_msg[str_len] = 0;
		if(!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			printf("%s: Bye!\n", CLIENT_NAME);
			break;
		}

		printf("%s: %s", CLIENT_NAME, clnt_msg);
		printf("%s: ",SERVER_NAME);
		fgets(serv_msg, BUF_SIZE, stdin);
		sendto(serv_sock, serv_msg, strlen(serv_msg), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
		if(!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
		{
			break;
		}

	}
	
	close(serv_sock);
	exit(0);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
