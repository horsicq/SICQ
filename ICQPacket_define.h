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
#define ICQ_TLV_CLIENTIDSTRING				0x0003
#define ICQ_TLV_DESCRIPTIONURL				0x0004
#define ICQ_TLV_BOSSERVER					0x0005
#define ICQ_TLV_AUTHCOOKIE					0x0006
#define ICQ_TLV_USERSTATUS					0x0006
//#define ICQ_TLV_				0x0007
#define ICQ_TLV_AUTHERRORCODE				0x0008
//#define ICQ_TLV_				0x0009
//#define ICQ_TLV_				0x000A
//#define ICQ_TLV_				0x000B
//#define ICQ_TLV_				0x000C
//#define ICQ_TLV_				0x000D
#define ICQ_TLV_CLIENTCOUNTRY				0x000E
#define ICQ_TLV_CLIENTLANG					0x000F
//#define ICQ_TLV_				0x0010
//#define ICQ_TLV_				0x0011
//#define ICQ_TLV_				0x0012
//#define ICQ_TLV_				0x0013
#define ICQ_TLV_DISTRIBNUMBER				0x0014
//#define ICQ_TLV_				0x0015
#define ICQ_TLV_CLIENTID					0x0016
#define ICQ_TLV_CLIENTMAJORVER				0x0017
#define ICQ_TLV_CLIENTMINORVER				0x0018
#define ICQ_TLV_CLIENTLESSERVER				0x0019
#define ICQ_TLV_CLIENTBUILDNUM				0x001A
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

#define ICQ_SNAC_OSERVICE_CLIENTREADY		0x0002
#define ICQ_SNAC_OSERVICE_FAMILIES			0x0003
#define ICQ_SNAC_OSERVICE_REQUESTRATELIMITS	0x0006
#define ICQ_SNAC_OSERVICE_RATELIMITS		0x0007
#define ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS	0x0008
#define ICQ_SNAC_OSERVICE_ONLINEINFO		0x000F
#define ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY	0x0013
#define ICQ_SNAC_OSERVICE_WELLKNOWNURLS		0x0015
#define ICQ_SNAC_OSERVICE_REQUESTVERSIONS	0x0017
#define ICQ_SNAC_OSERVICE_VERSIONS			0x0018
#define ICQ_SNAC_OSERVICE_SETSTATUS			0x001E
#define ICQ_SNAC_OSERVICE_EXTENDEDSTATUS	0x0021

#define ICQ_SNAC_BUDDY_REQUESTPARAMETERS	0x0002
#define ICQ_SNAC_BUDDY_PARAMETERS			0x0003

#define ICQ_SNAC_ICBM_SETPARAMETERS			0x0002

#define ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME	0x0004
#define ICQ_SNAC_SSI_ROSTER					0x0006
#define ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN	0x0007

#define ICQ_STATUS_WEBAWARE					0x00010000		// Status webaware flag
#define ICQ_STATUS_SHOWIP					0x00020000		// Status show ip flag
#define ICQ_STATUS_BIRTHDAY					0x00080000		// User birthday flag
#define ICQ_STATUS_WEBFRONT					0x00200000		// User active webfront flag
#define ICQ_STATUS_DCDISABLED				0x01000000		// Direct connection not supported
#define ICQ_STATUS_DCAUTH					0x10000000		// Direct connection upon authorization
#define ICQ_STATUS_DCCONT					0x20000000		// DC only with contact users

#define ICQ_STATUS_ONLINE					0x00000000		// Status is online
#define ICQ_STATUS_AWAY						0x00000001		// Status is away
#define ICQ_STATUS_DND						0x00000002		// Status is no not disturb (DND)
#define ICQ_STATUS_NA						0x00000004		// Status is not available (N/A)
#define ICQ_STATUS_OCCUPIED					0x00000010		// Status is occupied (BISY)
#define ICQ_STATUS_FREEFORCHAT				0x00000020		// Status is free for chat
#define ICQ_STATUS_INVISIBLE				0x00000100		// Status is invisible

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
