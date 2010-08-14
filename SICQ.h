//! \file SICQ.h 

#pragma once
#include "Utils.h"
#include "Net.h"
#include "ICQPacket.h"

#include "SICQ_define.h"

//! A class for OSCAR protocol (ICQ/AIM)
/*!
An application or DLL is required to perform a successful WSAStartup call before it can use SICQ
\code
WORD wVersionRequested;
WSADATA wsaData;
int err;

wVersionRequested = MAKEWORD(2, 2);

err = WSAStartup(wVersionRequested, &wsaData);
if (err != 0) {                                
	printf("WSAStartup failed with error: %d\n", err);
	return 1;
}
\endcode
When it has completed the use of SICQ, the application or DLL must call WSACleanup to deregister 
itself from a Windows Sockets implementation and allow the implementation to free any resources 
allocated on behalf of the application or DLL.
\code
WSACleanup();
\endcode

\sa _LoadWS, _UnloadWS
*/

class SICQ
{
private:
	int nError;
	HWND hEventWnd;
	TCHAR szUIN[20];
	TCHAR szPassword[20];
	TCHAR szServerIP[20];
	int nServerPort;
	SOCKET sock;
	int nSequence;

	HWND hMainWnd;

	void SequenceIncrement();

	// Start
	bool Start();
	// Stop
	bool Stop();
	// ICQ Login
	void ICQLogin();

	// CALLBACK function
	static LRESULT CALLBACK SocketProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	SICQ(HWND hMainWnd);
	~SICQ(void);

	// Get Last Error
	TCHAR *GetErrorString();
	
	// Login
	void Login(TCHAR *pszServerIP,int nServerPort,TCHAR *pszUIN,TCHAR *pszPassword);
	
};



