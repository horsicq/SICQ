#include "StdAfx.h"
#include "SICQ.h"

//! Constructor
//! \param [in] hMainWnd a handle to the window whose window procedure will receive the message
SICQ::SICQ(HWND hMainWnd)
{

	this->hMainWnd=hMainWnd;
	nError=SICQ_ERROR_SUCCESS;
	hEventWnd=0;
	nSequence=1;
	nStatus=SICQ_STATUS_OFFLINE;
	_ZeroMemory(&FoodGroups,sizeof(FoodGroups));

	Start();
}

//! Destructor
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
	case SICQ_ERROR_INCORRECTUINORPASSWORD:
		pszErrorString=TEXT("Incorrect UIN or Password");
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
	List _list;

	SICQ *pThis;

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

		if(_list.SetData(HwndList,sizeof(HwndList)))
		{
			pThis=this;
			_list.AddEntry((int)hEventWnd,(char *)&pThis,sizeof(this));
			//_list.AddEntry((int)hEventWnd,(char *)&hInst,sizeof(this));
		}
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
	List _list;

	if(hEventWnd)
	{
		SendMessage(hEventWnd,WM_CLOSE,0,0);
		if(_list.SetData(HwndList,sizeof(HwndList)))
		{
			_list.RemoveEntry((int)hEventWnd);
		}
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
//! \return true  if success
//! \return false if fail
//! \note main window receives #WM_SICQ_MAINWND_LOGIN event
bool SICQ::Login(TCHAR *pszServerIP,int nServerPort,TCHAR *pszUIN,TCHAR *pszPassword)
{
	lstrcpyn(szServerIP,pszServerIP,16);
	this->nServerPort=nServerPort;
	lstrcpyn(szUIN,pszUIN,16);
	lstrcpyn(szPassword,pszPassword,16);

	return SendMessage(hEventWnd,WM_SICQ_EVENTWND_LOGIN,0,0)==1;
}
//! Send Text
//! \param pszUIN [in] a pointer to a buffer that contains ICQ UIN 
//! \param pszText [in] a pointer to a buffer that contains a text message
//! \param nTextSize [in] a size of a text message
//! \return true  if success
//! \return false if fail
//! \note main window receives #WM_SICQ_MAINWND_SENDTEXT event
bool SICQ::SendText(TCHAR *pszUIN,TCHAR *pszText,int nTextSize)
{
	return SendMessage(hEventWnd,WM_SICQ_EVENTWND_SENDMESSAGE,0,0)==1;
}
//! Set Status
//! \param nStatus ICQ Status
//! \return true  if success
//! \return false if fail
bool SICQ::SetStatus(int nStatus)
{
	return SendMessage(hEventWnd,WM_SICQ_EVENTWND_SETSTATUS,nStatus,0)==1;
}
//! Login
bool SICQ::ICQLogin()
{
	return ICQLoginPlain();
	//return ICQLoginMD5();
}
bool SICQ::ICQLoginPlain()
{
	TCHAR szBuffer[256];
	char Cookies[256];
	int nCookiesSize;


	sock=_socket();
	if(_connect(sock,szServerIP,nServerPort))
	{
		Recv(sock);

		if(IsHelloPacket())
		{
			CreatePlainLoginPacket(nSequence,szUIN,szPassword);
			Send(sock);
			SequenceIncrement();

			Recv(sock);

			if(IsSignOffChannel())
			{
				nCookiesSize=GetTLV_blob(ICQ_TLV_AUTHCOOKIE,Cookies,sizeof(Cookies));
				GetTLV_string(ICQ_TLV_BOSSERVER,szBuffer,sizeof(szBuffer)/sizeof(TCHAR));

				ICQBOSServerConnect(szBuffer,Cookies,sizeof(Cookies));
			}
			else
			{
#ifdef  _DEBUG
				//##################################################
				_PrintTextNS(TEXT("Login Error!!!"));

				TCHAR szBuffer[256];
				wsprintf(szBuffer,TEXT("Auther Error Code %X"),GetTLV_u16(ICQ_TLV_AUTHERRORCODE));
				_PrintTextNS(szBuffer);
				//##################################################
#endif

				switch(GetTLV_u16(ICQ_TLV_AUTHERRORCODE))
				{
				case ICQ_AUTHERROR_INCORRECTNICKORPASS:
				case ICQ_AUTHERROR_MISMATCHNICKORPASS:
					nError=SICQ_ERROR_INCORRECTUINORPASSWORD;
					break;
				default:
					nError=SICQ_ERROR_UNKNOWN;
				}
			}
		}
	}
	else
	{
		nError=SICQ_ERROR_CANNOTCONNECTTOSERVER;
	}

	if(nError==SICQ_ERROR_SUCCESS)
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,1,(LPARAM)this);
		nStatus=SICQ_STATUS_ONLINE;
		return true;
	}
	else
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,0,(LPARAM)this);
		return false;
	}
	
}

bool SICQ::ICQLoginMD5()
{
	if(nError==SICQ_ERROR_SUCCESS)
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,1,(LPARAM)this);
		nStatus=SICQ_STATUS_ONLINE;
		return true;
	}
	else
	{
		SendMessage(hMainWnd,WM_SICQ_MAINWND_LOGIN,0,(LPARAM)this);
		return false;
	}
}

int SICQ::StatusToICQ(int nStatus)
{
	int nResult;

	switch(nStatus)
	{
	case SICQ_STATUS_ONLINE:
		nResult=ICQ_STATUS_ONLINE;
		break;
	case SICQ_STATUS_AWAY:
		nResult=ICQ_STATUS_AWAY;
		break;
	case SICQ_STATUS_DND:
		nResult=ICQ_STATUS_DND;
		break;
	case SICQ_STATUS_NA:
		nResult=ICQ_STATUS_NA;
		break;
	case SICQ_STATUS_INVISIBLE:
		nResult=ICQ_STATUS_INVISIBLE;
		break;
	case SICQ_STATUS_OCCUPIED:
		nResult=ICQ_STATUS_OCCUPIED;
		break;
	case SICQ_STATUS_FREEFORCHAT:
		nResult=ICQ_STATUS_FREEFORCHAT;
		break;

	}

	return nResult;
}

bool SICQ::ICQSetStatus(int nStatus)
{
	if(nStatus==SICQ_STATUS_OFFLINE)
	{
		if(this->nStatus!=SICQ_STATUS_OFFLINE)
		{
			CreateGoodByePacket(nSequence);
			Send(sock);
			SequenceIncrement();

			_closeconnect(sock);
		}
	}
	else
	{
		if(this->nStatus==SICQ_STATUS_OFFLINE)
		{
			CreateGoodByePacket(nSequence);
			Send(sock);
			SequenceIncrement();

			_closeconnect(sock);
		}
		else
		{
			CreateSetStatusPacket(nSequence,StatusToICQ(nStatus));
			Send(sock);
			SequenceIncrement();
		}
	}

	this->nStatus=nStatus;

	return true;
}

void SICQ::ICQBOSServerConnect(TCHAR *pszBOSServerIPAndPort,char *pCookies,int nCookiesSize)
{
	TCHAR szBOSServer[64],*pszOffset;
	int nBOSServerPort;

	pszOffset=StrStr(pszBOSServerIPAndPort,_TEXT(":"));
	pszOffset++;
	lstrcpyn(szBOSServer,pszBOSServerIPAndPort,(pszOffset-pszBOSServerIPAndPort));
	nBOSServerPort=StrToInt(pszOffset);

	CreateGoodByePacket(nSequence);
	Send(sock);
	SequenceIncrement();

	_closeconnect(sock);

	// BOS Server Connection
#ifdef  _DEBUG
	//##################################################
	_PrintTextNS(TEXT("BOS Server connection"));
	//##################################################
#endif
	sock=_socket();
	if(_connect(sock,szBOSServer,nBOSServerPort))
	{
		while(true)
		{
			Recv(sock);

			if(IsHelloPacket())
			{

				CreateCookiesPacket(nSequence,pCookies,nCookiesSize);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
			{

				ReadFoodGroupsFamiliesPacket(&FoodGroups);

				CreateFoodGroupsVersionsPacket(nSequence,&FoodGroups);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_VERSIONS))
			{
				ReadFoodGroupsVersionsPacket(&FoodGroups);
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY))
			{
				CreateRequestRatesPacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_WELLKNOWNURLS))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_RATELIMITS))
			{
				CreateAcceptRatesPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateRequestRosterFirstTimePacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_ROSTER))
			{
				CreateLoadRosterAfterLoginPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateSetICBMParametersPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateRequestBuddyParametersPacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_ONLINEINFO))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_EXTENDEDSTATUS))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_BUDDY,ICQ_SNAC_BUDDY_PARAMETERS))
			{
				CreateSetStatusPacket(nSequence,ICQ_STATUS_ONLINE);
				Send(sock);
				SequenceIncrement();

				CreateClientReadyPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				nError=SICQ_ERROR_SUCCESS;

				WSAAsyncSelect(sock,hEventWnd,WM_SICQ_EVENTWND_RECVDATA,FD_READ);

				break;
			}
			else
			{
#ifdef  _DEBUG
				//##################################################
				_PrintTextNS(TEXT("Unknown"));
				//##################################################
#endif
			}
		}
	}
	else
	{
		nError=SICQ_ERROR_CANNOTCONNECTTOSERVER;
	}
}

//! CALLBACK function
LRESULT CALLBACK SICQ::SocketProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	List _list;
	SICQ *pSICQ;

	_list.SetData(HwndList,sizeof(HwndList));
	pSICQ=(SICQ *)(*((int *)_list.GetEntryByID((int)hWnd)));

	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_SICQ_EVENTWND_LOGIN:

		//return ((SICQ *)lParam)->ICQLogin();
		return pSICQ->ICQLogin();
		
		break;

	case WM_SICQ_EVENTWND_SETSTATUS:

		//return ((SICQ *)lParam)->ICQSetStatus(wParam);
		return pSICQ->ICQSetStatus(wParam);

		break;

	case WM_SICQ_EVENTWND_RECVDATA:


#ifdef  _DEBUG
		//##################################################
		_PrintTextNS(TEXT("Recv Data"));
		//##################################################
#endif


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

char SICQ::HwndList[1000]={0};