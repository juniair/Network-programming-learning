/*
 * 해당 서버는 클라이언트의 메시지를 우선적으로 받는다.
 * 그리고 서버에서 메시지를 보낸다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024	// 메시지 최대 크기

// 화면에 출력된 메시지가 누구인지 나타내기 위한 이름 정의
#define SERVER_NAME "[Server]"
#define CLIENT_NAME "[Client]"

// 에러 처리 함수
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char serv_msg[BUF_SIZE], clnt_msg[BUF_SIZE];	// 코드 분석을 위해 서버와 클라이언트의 메시지 내용을 따로 저장	

	if(argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// TCP 소켓 제작
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
	// TCP 소켓 생성 완료



	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	if (clnt_sock == -1)
	{
		error_handling("accept() error.");
	}
		
	while (1)
	{
		// 클라이언트에서 메시지를 읽는다.
		read(clnt_sock, clnt_msg, BUF_SIZE);

		// 클라이언트에서 메시지가 종료 메시지인지 판독
		if (!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			printf("%s: Bye!\n", CLIENT_NAME);	// 클라이언트가 종료 하였음을 출력
			break;	// while문 수행 중지
		}
		else // 종료 메시지가 아닌 경우
		{
			
			printf("%s: %s", CLIENT_NAME, clnt_msg);	// 클라이언트에서 보낸 메시지 출력
			
			// 서버에서 클라이언트에게 보낼 메시지 작성
			printf("%s: ", SERVER_NAME);	
			fgets(serv_msg, BUF_SIZE, stdin);

			write(clnt_sock, serv_msg, sizeof(serv_msg));	// 클라이언트에게 서버 메시지 전송

			// 서버 종료 여부 확인
			if (!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
			{
				break;	// while문 수행 중지
			}	
		}
	}

	// 클라이언트와 서버 소켓을 닫는다.
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
