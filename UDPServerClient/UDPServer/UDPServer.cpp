// UDPServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#define SERVERPORT 9000
#define BUFSIZE 512

int main()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN serveraddr = { 0 };
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(sock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr));
	////////////////////////////////////////////////
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	char buf[BUFSIZE + 1];
	while (1) {
		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(SOCKADDR*)&clientaddr, &addrlen);
		buf[retval] = '\0';
		printf("%s:%d - %s\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);
		retval = sendto(sock, buf, retval, 0,
			(SOCKADDR*)&clientaddr, sizeof(clientaddr));
	}
	////////////////////////////////////////////////
	closesocket(sock);
	WSACleanup();
	return 0;
}
