// winsock1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

#pragma comment(lib,"ws2_32")
#include <winsock2.h>
int main()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		MessageBox(NULL, "error", "error", MB_OK);
		WSACleanup();
		exit(1);
	}
	MessageBox(NULL, "소켓 생성 완료", "알림", MB_OK);

	closesocket(sock);

	WSACleanup();
    return 0;
}

