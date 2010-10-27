//! \file SICQ_define.h SICQ definitions
#pragma once

#define SICQ_ERROR_SUCCESS					0
#define SICQ_ERROR_EVENTWINDOWISEXIST		1
#define SICQ_ERROR_EVENTWINDOWISNOTEXIST	2
#define SICQ_ERROR_CANNOTCONNECTTOSERVER	3
#define SICQ_ERROR_INCORRECTUINORPASSWORD	4
#define SICQ_ERROR_RATELIMITEXCEEDED		5
#define SICQ_ERROR_UNKNOWN					0xFFFF

#define WM_SICQ_EVENTWND_LOGIN				WM_USER+101
//! \param wParam 0
//! \param lParam a pointer to buffer that contains SENDTEXTSTRUCT structur
#define WM_SICQ_EVENTWND_SENDMESSAGE		WM_USER+102
//! \param wParam status
//! \param lParam 0
#define WM_SICQ_EVENTWND_SETSTATUS			WM_USER+103

#define WM_SICQ_EVENTWND_RECVDATA			WM_USER+104

//! \defgroup statuses Statuses
//! \{
#define SICQ_STATUS_OFFLINE					0
#define SICQ_STATUS_ONLINE					1
#define SICQ_STATUS_AWAY					2
#define SICQ_STATUS_DND						3
#define SICQ_STATUS_NA						4
#define SICQ_STATUS_OCCUPIED				5
#define SICQ_STATUS_FREEFORCHAT				6
#define SICQ_STATUS_INVISIBLE				7
//! \}


//! \defgroup messages Messages
//! \{

//! Login
//! \param wParam true if success, false if fail
//! \param lParam a pointer to SICQ class
#define WM_SICQ_MAINWND_LOGIN				WM_USER+200
//! SendText
//! \param wParam true if success, false if fail
//! \param lParam a pointer to SICQ class
#define WM_SICQ_MAINWND_SENDTEXT			WM_USER+201
//! Status
//! \param wParam Status
//! \param lParam a pointer to SICQ class
//! \sa statuses
#define WM_SICQ_MAINWND_STATUS				WM_USER+202
//! MessageAck
//! \param wParam MessageID
//! \param lParam a pointer to SICQ class
#define WM_SICQ_MAINWND_MESSAGEACK			WM_USER+203
//! RecvText
//! \param wParam a pointer to RECVMESSAGESTRUCT
//! \param lParam a pointer to SICQ class
//! \sa RECVMESSAGESTRUCT
#define WM_SICQ_MAINWND_RECVTEXT			WM_USER+204
//! \}

