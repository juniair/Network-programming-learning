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
	
	while (1)
	{
		// Ŭ���̾�Ʈ�� �޽��� �Է� ����
		printf("%s: ", CLIENT_NAME);
		fgets(clnt_msg, BUF_SIZE, stdin);
		
		// Ŭ���̾�Ʈ ���� ���� �ǵ�
		if (!strcmp(clnt_msg, "q\n") || !strcmp(clnt_msg, "Q\n"))
		{
			// ������ ���� �ϰڴٴ� �޽����� ������.
			write(sock, clnt_msg, sizeof(clnt_msg));
			break; // while�� ����
		}
		
		// ������ Ŭ���̾�Ʈ �޽��� ����
		write(sock, clnt_msg, sizeof(clnt_msg));
		// �������� Ŭ���̾�Ʈ�� �޽����� �д´�.
		read(sock, serv_msg, sizeof(serv_msg));

		// �������� ���� �޽��� ���� ���� Ȯ��
		if (!strcmp(serv_msg, "q\n") || !strcmp(serv_msg, "Q\n"))
		{
			printf("Server down\n");	// ����� ���� �ٿ� �޽��� ���
			break;	// while�� Ż��
		}

		printf("%s: %s", SERVER_NAME, serv_msg);	// ���� �޽����� �ƴҽ� �������� ���� �޽��� ���
	}

	// ������ �ݴ´�.
	close(sock);
	exit(0);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}
