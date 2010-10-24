#include "Net.h"

//! Load WinSocket
//! \return true  if success
//! \return false if fail
bool _LoadWS()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

#ifdef  _DEBUG
//##################################################
	_PrintDebugTextNS(TEXT("Load Win Socket"));
//##################################################
#endif

	return WSAStartup(wVersionRequested, &wsaData)==0;
}
//! Unload WinSocket
//! 
void _UnloadWS()
{
#ifdef  _DEBUG
//##################################################
	_PrintDebugTextNS(TEXT("Unload Win Socket"));
//##################################################
#endif
	WSACleanup();
}
//! The _socket function creates a socket
//! \return if no error occurs, _socket returns a descriptor referencing the new socket. If error 0;
SOCKET _socket()
{
	int nResult=0;
#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Socket"));
	//##################################################
#endif
	nResult=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nResult!=SOCKET_ERROR)
	{
		return nResult;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		char szBuffer[256];
		wsprintf(szBuffer,TEXT("Socket Error!!!(%X code)"),WSAGetLastError());
		_PrintDebugTextNS(szBuffer);
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
		_PrintDebugTextNS((PTSTR)lpMsgBuf);

		//##################################################
#endif
		return 0;
	}
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
	int nResult=0;

	_StringToChars(szBuffer,sizeof(szBuffer),pszServerIP);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(nServerPort);
	sa.sin_addr.s_addr = inet_addr(szBuffer);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Connect"));
	TCHAR szBuffer2[256];
	wsprintf(szBuffer2,TEXT("Server IP: %s"),pszServerIP);
	_PrintDebugTextNS(szBuffer2);
	wsprintf(szBuffer2,TEXT("Server port: %d"),nServerPort);
	_PrintDebugTextNS(szBuffer2);
	//##################################################
#endif

	nResult=connect(sock,(const sockaddr *)&sa,sizeof(sa));

	if(nResult!=SOCKET_ERROR)
	{
		return true;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		wsprintf(szBuffer,TEXT("Socket Error!!!(%X code)"),WSAGetLastError());
		_PrintDebugTextNS(szBuffer);
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
		_PrintDebugTextNS((PTSTR)lpMsgBuf);

		//##################################################
#endif
		return false;
	}

	
	
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
#ifdef  _DEBUG
	//##################################################
	TCHAR szBuffer[256];
	//##################################################
#endif

	int nSize=0,nResult=0;

	for(int i=0,j=nBufferSize;j>0;i+=1000,j-=1000)
	{
		nResult=recv(sock,&pBuffer[i],_MinInt(j,1000),0);
		if(nResult!=SOCKET_ERROR)
		{
			nSize+=nResult;
		}
		else if(WSAGetLastError()==WSAEWOULDBLOCK)
		{

		}
		else
		{
			nSize=0;
			j=0;
#ifdef  _DEBUG
			//##################################################
			wsprintf(szBuffer,TEXT("Socket Error!!!(%X code)"),WSAGetLastError());
			_PrintDebugTextNS(szBuffer);
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
				NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);
			_PrintDebugTextNS((PTSTR)lpMsgBuf);

			//##################################################
#endif
		}
		
	}

#ifdef  _DEBUG
	//##################################################

	wsprintf(szBuffer,TEXT("Recv %d(%X) bytes"),nSize,nSize);
	_PrintDebugTextNS(szBuffer);
	//##################################################
#endif

	return nSize;

}

//! The _send function sends data on a connected socket 
//! \param sock [in] a descriptor identifying a connected socket
//! \param pBuffer [in] A pointer to a buffer containing the data to be transmitted
//! \param nBufferSize [in] the size, in bytes, of the buffer pointed to by the pBuffer parameter
//! \return If no error occurs, send returns the total number of bytes sent, which can be less 
//! than the number requested to be sent in the nBufferSize parameter
int _send(SOCKET sock,char *pBuffer,int nBufferSize)
{
	int nSize=0;

	for(int i=0,j=nBufferSize;j>0;i+=1000,j-=1000)
	{
		nSize+=send(sock,&pBuffer[i],_MinInt(j,1000),0);
	}

#ifdef  _DEBUG
	//##################################################
	TCHAR szBuffer[256];
	wsprintf(szBuffer,TEXT("Send %d(%X) bytes"),nSize,nSize);
	_PrintDebugTextNS(szBuffer);
	//##################################################
#endif

	return nSize;
}
//! Close connection
//! \param sock [in] a descriptor identifying a connected socket
void _closeconnect(SOCKET sock)
{
#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Close Socket"));
	//##################################################
#endif

	shutdown(sock,2);
	closesocket(sock);
}