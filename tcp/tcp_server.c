#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_NAME "Server"
// 에러 처리 함수
void error_handling(char *message)

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_sock_size;
	char message[BUF_SIZE];

	if(argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		excption("socket() error");
	}

	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		excption("bind() error");
	}

	if(listen(serv_sock, 5) == -1)
	{
		excption("listen() error");
	}
	
	clnt_sock_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_sock_size); // 연결 확인
	if (clnt_sock == -1)
	{
		excption("accept() error");
	}
		
	while(1)
	{
		
		if(read(clnt_sock, message, sizeof(message)) == 0) // 클라이언트의 메시지를 우선적으로 받는다.
		{
			continue;
		}
		else	// 클라이언트에게 메시지 도착
		{
			// 클라이언트가 연결이 종료 요청일때 
			if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			{
				printf("[Client]: Bye Server!\n");
				close(clnt_sock);	// 새로운 클라이언트를 받은 준비를함
			}
			else	// 클라이언트가 채팅 메시지가 옴
			{
				printf("%s", message);	// 클라이언트가 보낸 메시지 출력
				fgets(message, BUF_SIZE,  stdin);	// 서버는 클라이언트에게 보낼 메시지 입력

				// 서버 종료일 경우 다음 문장 수행
				if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
				{
					sprintf(message, "Server shutdown\n");
					break;	// while문 탈출
				}
				sprintf(message, "%s: %s", SERVER_NAME, message);
				write(clnt_sock, message, sizeof(message));
			}
		}
	}

	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);

	exit(0);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
