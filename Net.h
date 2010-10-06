#pragma once
#include <windows.h>
#include "Strings.h"

//#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"wsock32.lib")

//! \defgroup tcpconnect_functions TCPconnect functions
//! \{
bool _LoadWS();
void _UnloadWS();
SOCKET _socket();
bool _connect(SOCKET sock,TCHAR *pszServerIP,int nServerPort);
int _recv(SOCKET sock,char *pBuffer,int nBufferSize);
int _send(SOCKET sock,char *pBuffer,int nBufferSize);
void _closeconnect(SOCKET sock);
//! \}