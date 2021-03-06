// MultithreadTCPServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

SOCKET client_sock_list[5];
int client_sock_count = 0;

DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr,
		&addrlen);
	char buf[BUFSIZE + 1];
	while (1) {
		int retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			printf("recv() error"); break;
		}
		else if (retval == 0) break;
		buf[retval] = '\0';
		printf("%s:%d - %s\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);
		for (int i = 0; i < client_sock_count; i++) {
			send(client_sock_list[i], buf, retval, 0);
		}
		/*retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			printf("send() error"); break;
		}*/
	}
	closesocket(client_sock);
	return 0;
}

int main()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		MessageBox(NULL, "socket()", "Error", MB_OK);
		exit(1);
	}
	SOCKADDR_IN serveraddr = { 0 };
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock,
		(SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		MessageBox(NULL, "bind()", "Error", MB_OK);
		exit(1);
	}
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		MessageBox(NULL, "listen()", "Error", MB_OK);
		exit(1);
	}
	//////////////////////////////
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	HANDLE hThread;
	while (1) {
		client_sock = accept(listen_sock,
			(SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock_count < 5) {
			client_sock_list[client_sock_count] =
				client_sock;
			client_sock_count++;
		}
		printf("ip=%s,port=%d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) closesocket(client_sock);
		else CloseHandle(hThread);
	}

	closesocket(listen_sock);
	WSACleanup();
    return 0;
}

