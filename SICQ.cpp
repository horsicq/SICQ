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
	_ZeroMemory(&FoodGroups,sizeof(FoodGroups));

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
	TCHAR szBuffer[256],szBOSServer[64],*pszOffset;
	char Cookies[256];
	int nCookiesSize,nBOSServerPort;


	sock=_socket();
	if(_connect(sock,szServerIP,nServerPort))
	{
		Recv(sock);
		if(IsHelloPacket())
		{
#ifdef  _DEBUG
			//##################################################
			_PrintHEXTable(GetPacketPointer(),GetPacketSize());
			_PrintTextNS(TEXT("Hello Packet"));
			//##################################################
#endif
			CreateLoginPacket(nSequence,szUIN,szPassword);

#ifdef  _DEBUG
			//##################################################
			_PrintHEXTable(GetPacketPointer(),GetPacketSize());
			_PrintTextNS(TEXT("Create Login Packet"));
			//##################################################
#endif
			Send(sock);
			SequenceIncrement();


			Recv(sock);

#ifdef  _DEBUG
			//##################################################
			_PrintHEXTable(GetPacketPointer(),GetPacketSize());
			_PrintTextNS(TEXT("Recv Cookies"));
			//##################################################
#endif
			nCookiesSize=GetTLV_blob(ICQ_TLV_AUTH_COOKIE,Cookies,sizeof(Cookies));
			GetTLV_string(ICQ_TLV_BOS_SERVER,szBuffer,sizeof(szBuffer)/sizeof(TCHAR));
			pszOffset=StrStr(szBuffer,_TEXT(":"));
			pszOffset++;
			lstrcpyn(szBOSServer,szBuffer,(pszOffset-szBuffer));
			nBOSServerPort=StrToInt(pszOffset);

			CreateGoodByePacket(nSequence);
#ifdef  _DEBUG
			//##################################################
			_PrintHEXTable(GetPacketPointer(),GetPacketSize());
			_PrintTextNS(TEXT("Create Goodbye Packet"));
			//##################################################
#endif
			Send(sock);
			SequenceIncrement();

			_closeconnect(sock);

			// BOS Server Connection
			sock=_socket();
			if(_connect(sock,szBOSServer,nBOSServerPort))
			{
				while(true)
				{
					Recv(sock);

#ifdef  _DEBUG
					//##################################################
					_PrintHEXTable(GetPacketPointer(),GetPacketSize());
					//##################################################
#endif

					if(IsHelloPacket())
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Hello Packet"));
						//##################################################
#endif
						CreateCookiesPacket(nSequence,Cookies,nCookiesSize);

#ifdef  _DEBUG
						//##################################################
						_PrintHEXTable(GetPacketPointer(),GetPacketSize());
						_PrintTextNS(TEXT("Create Cookies Packet"));
						//##################################################
#endif

						Send(sock);
						SequenceIncrement();
					}
					else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Services Families"));
						_PrintTextNS(TEXT("Get Food Groups"));
						//##################################################
#endif
						ReadFoodGroupsFamiliesPacket(&FoodGroups);
						CreateFoodGroupsVersionsPacket(nSequence,&FoodGroups);

#ifdef  _DEBUG
						//##################################################
						_PrintHEXTable(GetPacketPointer(),GetPacketSize());
						_PrintTextNS(TEXT("Set Foods Group Versions"));
						//##################################################
#endif

						Send(sock);
						SequenceIncrement();
					}
					else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_VERSIONS))
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Services Versions"));
						_PrintTextNS(TEXT("Get Services Versions"));
						//##################################################
#endif

						ReadFoodGroupsVersionsPacket(&FoodGroups);

						//Send(sock);
						//SequenceIncrement();
					}
					else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY))
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Message of the Day"));
						//##################################################
#endif
						CreateRequestRatesPacket(nSequence);
#ifdef  _DEBUG
						//##################################################
						_PrintHEXTable(GetPacketPointer(),GetPacketSize());
						_PrintTextNS(TEXT("Create Request Rates"));
						//##################################################
#endif

						Send(sock);
						SequenceIncrement();
					}
					else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_WELLKNOWNURLS))
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Well known URLs"));
						//##################################################
#endif
					}
					else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_RATELIMITS))
					{
#ifdef  _DEBUG
						//##################################################
						_PrintTextNS(TEXT("Rate Limits"));
						//##################################################
#endif
					}
				}
			}
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