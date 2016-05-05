/*
	UDP 통신 방법
	클라이언트에서 메시지를 우선적으로 받는다.
	그 뒤 서버에서 클라이언트에게 메시지를 보낸다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define CLIENT_NAME "[Client]"
#define SERVER_NAME "[Server]"

// 서버 예외 처리 핸들러
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

	// 소켓 생성 시작
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
	// 소켓 생성 끝


	while(1)
	{
		// 클라이언트에서 온 메시지 수신
		clnt_addr_size = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, clnt_msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		clnt_msg[str_len] = 0;

		// 클라이언트 종료여부 확인
		if(!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			printf("%s: Bye!\n", CLIENT_NAME);
			break;	// while문 수행 종료
		}

		printf("%s: %s", CLIENT_NAME, clnt_msg);	// 클라이언트에서 온 메시지 출력

		// 서버에서 메시지 입력
		printf("%s: ",SERVER_NAME);
		fgets(serv_msg, BUF_SIZE, stdin);

		// 클라언트에서 입력한 메시지 전송
		sendto(serv_sock, serv_msg, strlen(serv_msg), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);

		// 서버 종료 여부 판독
		if(!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
		{
			break;	// while문 수행 종료
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
