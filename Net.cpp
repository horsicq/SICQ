#include "stdafx.h"
#include "Net.h"


//! Load WinSocket
//! \return true  if success
//! \return false if fail
bool _LoadWS()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	return WSAStartup(wVersionRequested, &wsaData)==0;
}
//! Unload WinSocket
void _UnloadWS()
{
	WSACleanup();
}
//! The _socket function creates a socket
//! \return If no error occurs, socket returns a descriptor referencing the new socket.
SOCKET _socket()
{
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
//! The _connect function establishes a connection
//! \param sock [in] a descriptor identifying an unconnected socket
//! \param pszServerIP [in] a pointer to a buffer that contains IP of server
//! \param nServerPort [in] a port of server
//! \return true  if success
//! \return false if fail
bool _connect(SOCKET sock,TCHAR *pszServerIP,int nServerPort)
{
	sockaddr_in sa;
	char szBuffer[256];

	_StringToChars(szBuffer,sizeof(szBuffer),pszServerIP);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(nServerPort);
	sa.sin_addr.s_addr = inet_addr(szBuffer);

	return (connect(sock,(const sockaddr *)&sa,sizeof(sa))!= SOCKET_ERROR);
	
}
//! The _recv function receives data from a connected socket 
//! \param sock [in] a descriptor identifying a connected socket
//! \param pBuffer [out] a pointer to a buffer to receive the incoming data
//! \param nBufferSize [in] the size, in bytes, of the buffer pointed to by the pBuffer parameter
//! \return If no error occurs, _recv returns the number of bytes received and the buffer pointed to by 
//! the pBuffer parameter will contain this data received. If the connection has been gracefully closed, 
//! the return value is zero.
int _recv(SOCKET sock,char *pBuffer,int nBufferSize)
{
	return recv(sock,pBuffer,nBufferSize,0);
}

//! The _send function sends data on a connected socket 
//! \param sock [in] a descriptor identifying a connected socket
//! \param pBuffer [in] A pointer to a buffer containing the data to be transmitted
//! \param nBufferSize [in] the size, in bytes, of the buffer pointed to by the pBuffer parameter
//! \return If no error occurs, send returns the total number of bytes sent, which can be less 
//! than the number requested to be sent in the nBufferSize parameter
int _send(SOCKET sock,char *pBuffer,int nBufferSize)
{
	return send(sock,pBuffer,nBufferSize,0);
}
//! Close connection
//! \param sock [in] a descriptor identifying a connected socket
void _closeconnect(SOCKET sock)
{
	shutdown(sock,2);
	closesocket(sock);
}