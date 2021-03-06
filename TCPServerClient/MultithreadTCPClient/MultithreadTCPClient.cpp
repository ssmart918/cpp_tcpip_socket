// MultithreadTCPClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received; char *ptr = buf; int left = len;
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

DWORD WINAPI ProcessThread(LPVOID arg) {
	SOCKET sock = (SOCKET)arg;
	char buf[BUFSIZE + 1];
	while (1) {
		int retval = recv(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			printf("\nrecv Error\n"); break;
		}
		else if (retval == 0) break;
		buf[retval] = '\0';
		printf("\n%s (Receive %d bytes)\n", buf, retval);
	}
	return 0;
}

int main()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		MessageBox(NULL, "socket() error", "error", MB_OK);
	}
	///////////////////////////////////////////////
	SOCKADDR_IN serveraddr = { 0 };
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		MessageBox(NULL, "connect() error", "error", MB_OK);
		exit(1);
	}
	printf("connection success!");
	HANDLE hThread = CreateThread(NULL, 0, ProcessThread,
		(LPVOID)sock, 0, NULL);
	while (1) {
		printf("\n[Input Data] ");
		char buf[BUFSIZE + 1] = { 0 };
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;
		int len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			printf("\nsend Error\n"); break;
		}
		printf("send %d bytes\n", retval);
	}
	///////////////////////////////////////////////
	closesocket(sock);
	WSACleanup();
	return 0;
}