//! \file ICQPacket_define.h ICQ definitions
#pragma once

#define ICQPACKET_MAXPACKETSIZE				8192

struct FLAP
{
	//! always 0x2A
	unsigned char id; 
	unsigned char channel;
	unsigned short sequence; 
	unsigned short DataSize;
};

struct TLV
{
	unsigned short Type;
	unsigned short Length;
};

#pragma pack(push, 1)

struct SNAC
{
	unsigned short family;
	unsigned short subtype;
	unsigned short flags;
	unsigned int requestid;
};

#pragma pack (pop)

#define ICQ_FLAPSIGNATURE					0x2a

#define ICQ_CHANNEL_SIGNON					0x01
#define ICQ_CHANNEL_SNACDATA				0x02
#define ICQ_CHANNEL_ERROR					0x03
#define ICQ_CHANNEL_SIGNOFF					0x04
#define ICQ_CHANNEL_KEEPALIVE				0x05

#define ICQ_TLV_UIN							0x0001
#define ICQ_TLV_PASSWORD					0x0002
#define ICQ_TLV_CLIENT_ID_STRING			0x0003
//#define ICQ_TLV_				0x0004
#define ICQ_TLV_BOS_SERVER					0x0005
#define ICQ_TLV_AUTH_COOKIE					0x0006
//#define ICQ_TLV_				0x0007
#define ICQ_TLV_AUTH_ERROR_CODE				0x0008
//#define ICQ_TLV_				0x0009
//#define ICQ_TLV_				0x000A
//#define ICQ_TLV_				0x000B
//#define ICQ_TLV_				0x000C
//#define ICQ_TLV_				0x000D
#define ICQ_TLV_CLIENT_COUNTRY				0x000E
#define ICQ_TLV_CLIENT_LANG					0x000F
//#define ICQ_TLV_				0x0010
//#define ICQ_TLV_				0x0011
//#define ICQ_TLV_				0x0012
//#define ICQ_TLV_				0x0013
#define ICQ_TLV_DISTRIB_NUMBER				0x0014
//#define ICQ_TLV_				0x0015
#define ICQ_TLV_CLIENT_ID					0x0016
#define ICQ_TLV_CLIENT_MAJOR_VER			0x0017
#define ICQ_TLV_CLIENT_MINOR_VER			0x0018
#define ICQ_TLV_CLIENT_LESSER_VER			0x0019
#define ICQ_TLV_CLIENT_BUILD_NUM			0x001A
//#define ICQ_TLV_				0x001B
//#define ICQ_TLV_				0x001C
//#define ICQ_TLV_				0x001D
//#define ICQ_TLV_				0x001E
//#define ICQ_TLV_				0x001F

//! FOODGROUPS
#define ICQ_SNAC_FOODGROUP_OSERVICE			0x0001
#define ICQ_SNAC_FOODGROUP_LOCATE			0x0002
#define ICQ_SNAC_FOODGROUP_BUDDY			0x0003
#define ICQ_SNAC_FOODGROUP_ICBM				0x0004
#define ICQ_SNAC_FOODGROUP_INVITE			0x0006
#define ICQ_SNAC_FOODGROUP_POPUP			0x0008
#define ICQ_SNAC_FOODGROUP_PD				0x0009
#define ICQ_SNAC_FOODGROUP_LOOKUP			0x000A
#define ICQ_SNAC_FOODGROUP_STATS			0x000B
#define ICQ_SNAC_FOODGROUP_TRANS			0x000C
#define ICQ_SNAC_FOODGROUP_SSI				0x0013
#define ICQ_SNAC_FOODGROUP_SPEC				0x0015
#define ICQ_SNAC_FOODGROUP_UNK1				0x0022
#define ICQ_SNAC_FOODGROUP_UNK2				0x0024
#define ICQ_SNAC_FOODGROUP_UNK3				0x0025


#define ICQ_SNAC_OSERVICE_FAMILIES			0x0003
#define ICQ_SNAC_OSERVICE_REQUESTRATELIMITS	0x0006
#define ICQ_SNAC_OSERVICE_RATELIMITS		0x0007
#define ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS	0x0008
#define ICQ_SNAC_OSERVICE_ONLINEINFO		0x000F
#define ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY	0x0013
#define ICQ_SNAC_OSERVICE_WELLKNOWNURLS		0x0015
#define ICQ_SNAC_OSERVICE_REQUESTVERSIONS	0x0017
#define ICQ_SNAC_OSERVICE_VERSIONS			0x0018
#define ICQ_SNAC_OSERVICE_EXTENDEDSTATUS	0x0021

#define ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME	0x0004
#define ICQ_SNAC_SSI_ROSTER					0x0006
#define ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN	0x0007

struct FOODGROUP
{
	bool Support;
	unsigned short Version;
};

struct FOODGROUPS
{
	FOODGROUP Oservice;
	FOODGROUP Locate;
	FOODGROUP Buddy;
	FOODGROUP ICBM;
	FOODGROUP Invite;
	FOODGROUP Popup;
	FOODGROUP PD;
	FOODGROUP Lookup;
	FOODGROUP Stats;
	FOODGROUP Trans;
	FOODGROUP SSI;
	FOODGROUP Spec;
	FOODGROUP Unk1;
	FOODGROUP Unk2;
	FOODGROUP Unk3;
};
