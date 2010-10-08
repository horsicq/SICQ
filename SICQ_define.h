//! \file SICQ_define.h SICQ definitions
#pragma once

#define SICQ_ERROR_SUCCESS					0
#define SICQ_ERROR_EVENTWINDOWISEXIST		1
#define SICQ_ERROR_EVENTWINDOWISNOTEXIST	2
#define SICQ_ERROR_CANNOTCONNECTTOSERVER	3
#define SICQ_ERROR_INCORRECTUINORPASSWORD	4
#define SICQ_ERROR_UNKNOWN					0xFFFF


#define WM_SICQ_EVENTWND_LOGIN_PLAIN		WM_USER+100
#define WM_SICQ_EVENTWND_LOGIN_MD5			WM_USER+101

#define SICQ_STATUS_OFFLINE					0
#define SICQ_STATUS_ONLINE					1


//! \defgroup messages Messages
//! \{

//! Login
//! \param wParam true if success, false if fail
//! \param lParam a pointer to SICQ class
#define WM_SICQ_MAINWND_LOGIN				WM_USER+200

//! \}