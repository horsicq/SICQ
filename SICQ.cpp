#include "StdAfx.h"
#include "SICQ.h"

//! Constructor
//! \param [in] hMainWnd Das ist ein Handle des Fensters, das alle Events bekommen muss
SICQ::SICQ(HWND hMainWnd)
{
	this->hMainWnd=hMainWnd;
	nError=SICQ_ERROR_SUCCESS;
	hEventWnd=0;
	nSequence=1;

	Start();
}


SICQ::~SICQ(void)
{
	Stop();
}

//! Get Last Error
//! \return a pointer to a string that specifies the error message
TCHAR *SICQ::GetErrorString()
{
	TCHAR *pszErrorString;

	switch(nError)
	{
	case SICQ_ERROR_SUCCESS:
		pszErrorString=TEXT("Success");
		break;
	case SICQ_ERROR_EVENTWINDOWISEXIST:
		pszErrorString=TEXT("Event Window is exist");
		break;
	case SICQ_ERROR_EVENTWINDOWISNOTEXIST:
		pszErrorString=TEXT("Event Window is not exist");
		break;
	case SICQ_ERROR_CANNOTCONNECTTOSERVER:
		pszErrorString=TEXT("Cannot connect to Server");
		break;
	default:
		pszErrorString=TEXT("Unknown!!!");
	}


	return pszErrorString;
}

//! Start Event Window
//! 
//! \return true  if success
//! \return false if fail
//! \sa GetErrorString()
bool SICQ::Start()
{
	WNDCLASS wc;
	HINSTANCE hInst=GetModuleHandle(0);

	if(!hEventWnd)
	{
		_ZeroMemory(&wc,sizeof(wc));

		wc.lpfnWndProc=SocketProc;
		wc.hInstance=hInst;
		wc.lpszClassName =TEXT("ICQ Socket");
		wc.style = 0;

		// Register Class
		RegisterClass(&wc);
		// Create Event Window
		hEventWnd=CreateWindowEx(WS_EX_TOOLWINDOW,TEXT("ICQ Socket"),TEXT("ICQ Socket"),0,0,0,0,0,NULL,NULL,hInst,NULL);
		// Set Success
		nError=SICQ_ERROR_SUCCESS;

		return true;
	}
	else
	{
		nError=SICQ_ERROR_EVENTWINDOWISEXIST;
		return false;
	}
}

//! Stop Event Window
//! \return true  if success
//! \return false if fail
//! \sa GetErrorString()
bool SICQ::Stop()
{
	if(hEventWnd)
	{
		SendMessage(hEventWnd,WM_CLOSE,0,0);
		hEventWnd=0;
		nError=SICQ_ERROR_SUCCESS;

		return true;
	}
	else
	{
		nError=SICQ_ERROR_EVENTWINDOWISNOTEXIST;

		return false;
	}
	
}

//! Login
//! \param pszServerIP [in] a pointer to a buffer that contains IP of ICQ server ( for example 205.188.251.43 )
//! \param nServerPort [in] a port of ICQ server ( usually 5190 )
//! \param pszUIN [in] a pointer to a buffer that contains ICQ UIN
//! \param pszPassword [in] a pointer to a buffer that contains ICQ Password
//! \note main window receives #WM_SICQ_MAINWND_LOGIN event
void SICQ::Login(TCHAR *pszServerIP,int nServerPort,TCHAR *pszUIN,TCHAR *pszPassword)
{
	lstrcpyn(szServerIP,pszServerIP,16);
	this->nServerPort=nServerPort;
	lstrcpyn(szUIN,pszUIN,16);
	lstrcpyn(szPassword,pszPassword,16);

	SendMessage(hEventWnd,WM_SICQ_EVENTWND_LOGIN,0,(LPARAM)this);
}
//! Login
void SICQ::ICQLogin()
{
	ICQPacket icqpacket;
	TCHAR szBuffer[256];

	sock=_socket();
	if(_connect(sock,szServerIP,nServerPort))
	{
		icqpacket.Recv(sock);
		if(icqpacket.IsHelloPacket())
		{
			icqpacket.CreateLoginPacket(nSequence,szUIN,szPassword);
			icqpacket.Send(sock);
			SequenceIncrement();

			icqpacket.Recv(sock);

			icqpacket.GetTLV_string(ICQ_TLV_BOS_SERVER,szBuffer,sizeof(szBuffer)/sizeof(TCHAR));
		}

		nError=SICQ_ERROR_SUCCESS;
	}
	else
	{
		nError=SICQ_ERROR_CANNOTCONNECTTOSERVER;
	}

	if(nError==SICQ_ERROR_SUCCESS)
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,1,(LPARAM)this);
	}
	else
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,0,(LPARAM)this);
	}
	
}

//! CALLBACK function
LRESULT CALLBACK SICQ::SocketProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_SICQ_EVENTWND_LOGIN:

		((SICQ *)lParam)->ICQLogin();

		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	default:

		DefWindowProc(hWnd,message,wParam,lParam);

	}
	return 1;
}

void SICQ::SequenceIncrement()
{
	if(nSequence<0x8000)
	{
		nSequence++;
	}
	else
	{
		nSequence=0;
	}
}