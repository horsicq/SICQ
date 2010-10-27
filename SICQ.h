//! \file SICQ.h Header for SICQ.cpp

#pragma once
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "Utils.h"
#include "Net.h"
#include "ICQPacket.h"
#include "SICQ_define.h"
#include "List.h"
#include "random.h"
#include "Time.h"

//! \defgroup sicq SICQ
//! \{
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
//! A class for OSCAR protocol (ICQ/AIM)
class SICQ: private ICQPacket
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
	int nStatus;
	static char HwndList[1000];
	FOODGROUPS FoodGroups;
	HWND hMainWnd;

	void SequenceIncrement();
	bool Start();
	bool Stop();
	bool ICQLogin();
	bool ICQLoginPlain();
	bool ICQLoginMD5();
	void ICQBOSServerConnect(TCHAR *pszBOSServerIPAndPort,char *pCookies,int nCookiesSize);
	void ICQSetStatus(int nStatus);
	int StatusToICQ(int nStatus);
	void ICQSendText(SENDTEXTSTRUCT *pSts);
	void ICQPing();
	void DefProc();
	// CALLBACK function
	static LRESULT CALLBACK SocketProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	SICQ();
	~SICQ(void);

	void SetWindowsHandle(HWND hMainWnd);
	TCHAR *GetErrorString();
	void Login(TCHAR *pszServerIP,int nServerPort,TCHAR *pszUIN,TCHAR *pszPassword);
	int SendText(TCHAR *pszUIN,TCHAR *pszText,int nTextLength);
	bool SetStatus(int nStatus);
};
//! \}


