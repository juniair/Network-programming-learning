#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_NAME "[Server]"
#define CLIENT_NAME "[Client]"


void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	
	char serv_msg[BUF_SIZE], clnt_msg[BUF_SIZE];

	if (argc != 3)
	{
		printf("Usage %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connet() error");
	}
	
	printf("Welcome to 'Hello Server'\n");
	sprintf(clnt_msg, "%s: Hi Server!\n", CLIENT_NAME);
	printf("%s", clnt_msg);
	write(sock, clnt_msg, sizeof(clnt_msg));

	while (1)
	{
		printf("%s: ", CLIENT_NAME);
		fgets(clnt_msg, BUF_SIZE, stdin);
				
		if (!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			write(sock, clnt_msg, sizeof(clnt_msg));
			read(sock, serv_msg, sizeof(serv_msg));
			printf("%s", serv_msg);
			break;
		}
		else
		{
			sprintf(clnt_msg, "%s: %s", CLIENT_NAME, clnt_msg);
			switch (write(sock, serv_msg, BUF_SIZE))
			{
				case -1:
					error_handling("write() error.");
					break;
				case 0:
					printf("peer connection closed.");
					close(sock);
					exit(EXIT_SUCCESS);
					break;
				default:		
					read(sock, serv_msg, BUF_SIZE);
					if (!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
					{
						printf("[SERVER]: Bye Clinet!\n");
						close(sock);
						exit(EXIT_SUCCESS);
					}
					printf("[SERVER]: %s", serv_msg);
			}
		
			
		}
			
	}
	
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}