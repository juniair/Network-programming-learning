#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_NAME "[Server]"
#define CLIENT_NAME "[Client]"

// 에러 처리 함수
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_sock_size;
	char serv_msg[BUF_SIZE], clnt_msg[BUF_SIZE];
	int str_len;
	

	if(argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}

	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}
	

		
	while (1)
	{
		
		
		str_len = read(clnt_sock, clnt_msg, BUF_SIZE);

		if (!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			printf("%s: Bye!\n", CLIENT_NAME);
			close(clnt_sock);
		}
		printf("%s: %s", CLIENT_NAME, clnt_msg);
		printf("%s: ", SERVER_NAME);
		fgets(serv_msg, BUF_SIZE, stdin);
		write(clnt_sock, serv_msg, sizeof(serv_msg));
		if (!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
		{
			break;
		}
	}

	close(serv_sock);
	close(clnt_sock);
	exit(0);


}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
