//! \file ICQPacket_define.h ICQPacket definitions
#pragma once


//! \defgroup oscar OSCAR protocol 

//! \defgroup oscar_flap FLAP
//! \ingroup oscar
//! \{
//! \struct FLAP_HEADER
struct FLAP_HEADER
{

	unsigned char signature;				//!< FLAP signature (always 0x2a) \sa ICQ_FLAP_SIGNATURE
	unsigned char channel;					//!< FLAP channel \sa oscar_flap_channels
	unsigned short sequence;				//!< FLAP sequence 
	unsigned short lenght;					//!< size of FLAP data without FLAP header
};

#define ICQPACKET_MAXPACKETSIZE				8192
#define ICQ_FLAP_SIGNATURE					0x2a
//! \}

//! \defgroup oscar_flap_channels Channels
//! \ingroup oscar_flap
//! \{
#define ICQ_FLAP_CHANNEL_SIGNON				0x01
#define ICQ_FLAP_CHANNEL_SNACDATA			0x02
#define ICQ_FLAP_CHANNEL_ERROR				0x03
#define ICQ_FLAP_CHANNEL_SIGNOFF			0x04
#define ICQ_FLAP_CHANNEL_KEEPALIVE			0x05
//! \}

//! \defgroup oscar_tlv TLV
//! \ingroup oscar
//! \{
//! \struct TLV_HEADER
struct TLV_HEADER
{
	unsigned short type;					//!< TLV type \sa oscar_tlv_types
	unsigned short length;					//!< size of TLV data without TLV header
};
//! \}

//! \defgroup oscar_tlv_types Types
//! \ingroup oscar_tlv
//! \{
#define ICQ_TLV_UIN							0x0001
#define ICQ_TLV_PASSWORD					0x0002
#define ICQ_TLV_CLIENTIDSTRING				0x0003
#define ICQ_TLV_DESCRIPTIONURL				0x0004
#define ICQ_TLV_BOSSERVER					0x0005
#define ICQ_TLV_AUTHCOOKIE					0x0006
#define ICQ_TLV_USERSTATUS					0x0006
#define ICQ_TLV_AUTHERRORCODE				0x0008
#define ICQ_TLV_CLIENTCOUNTRY				0x000E
#define ICQ_TLV_CLIENTLANG					0x000F
#define ICQ_TLV_DISTRIBNUMBER				0x0014
#define ICQ_TLV_CLIENTID					0x0016
#define ICQ_TLV_CLIENTMAJORVER				0x0017
#define ICQ_TLV_CLIENTMINORVER				0x0018
#define ICQ_TLV_CLIENTLESSERVER				0x0019
#define ICQ_TLV_CLIENTBUILDNUM				0x001A
//! \}

//! \defgroup oscar_snac SNAC
//! \ingroup oscar
//! \{
//! \struct SNAC_HEADER
#pragma pack(push, 1)
struct SNAC_HEADER
{
	unsigned short family;					//!< SNAC family
	unsigned short subtype;					//!< SNAC subtype
	unsigned short flags;					//!< SNAC flags
	unsigned int requestid;					//!< SNAC requestid
};
#pragma pack (pop)
//! \}

//! \defgroup oscar_snac_foodgroups Foodgroups
//! \ingroup oscar_snac
//! \{
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
//! \}

//! \defgroup oscar_snac_foodgroups_oservice Oservice
//! \ingroup oscar_snac_foodgroups
//! \{
#define ICQ_SNAC_OSERVICE_ERROR				0x0001
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
//! \}

//! \defgroup oscar_snac_foodgroups_buddy Buddy
//! \ingroup oscar_snac_foodgroups
//! \{
#define ICQ_SNAC_BUDDY_ERROR				0x0001
#define ICQ_SNAC_BUDDY_REQUESTPARAMETERS	0x0002
#define ICQ_SNAC_BUDDY_PARAMETERS			0x0003
#define ICQ_SNAC_BUDDY_USERONLINE			0x000B
#define ICQ_SNAC_BUDDY_USEROFFLINE			0x000C
//! \}

//! \defgroup oscar_snac_foodgroups_icbm ICBM
//! \ingroup oscar_snac_foodgroups
//! \{
#define ICQ_SNAC_ICBM_ERROR					0x0001
#define ICQ_SNAC_ICBM_SETPARAMETERS			0x0002
#define ICQ_SNAC_ICBM_SENDMESSAGE			0x0006
#define ICQ_SNAC_ICBM_RECVMESSAGE			0x0007
#define ICQ_SNAC_ICBM_MESSAGEACK			0x000C
//! \}

//! \defgroup oscar_snac_foodgroups_stats Stats
//! \ingroup oscar_snac_foodgroups
//! \{
#define ICQ_SNAC_STATS_ERROR				0x0001
#define ICQ_SNAC_STATS_SETMINIMUMINTERVAL	0x0002
#define ICQ_SNAC_STATS_STATSREPORT			0x0003
#define ICQ_SNAC_STATS_STATSREPORTACK		0x0004
//! \}

//! \defgroup oscar_snac_foodgroups_ssi SSI
//! \ingroup oscar_snac_foodgroups
//! \{
#define ICQ_SNAC_SSI_ERROR					0x0001
#define ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME	0x0004
#define ICQ_SNAC_SSI_ROSTER					0x0006
#define ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN	0x0007
//! \}

//! \defgroup oscar_autherrors Auth Errors
//! \ingroup oscar
//! \{
#define ICQ_AUTHERROR_INVALIDNICKORPASS		0x0001
#define ICQ_AUTHERROR_SERVICEUNAVAILABLE	0x0002
#define ICQ_AUTHERROR_OTHER					0x0003
#define ICQ_AUTHERROR_INCORRECTNICKORPASS	0x0004
#define ICQ_AUTHERROR_MISMATCHNICKORPASS	0x0005
#define ICQ_AUTHERROR_INTERNAL				0x0006
#define ICQ_AUTHERROR_INVALIDACCOUNT		0x0007
#define ICQ_AUTHERROR_DELETEDACCOUNT		0x0008
#define ICQ_AUTHERROR_EXPIREDACCOUNT		0x0009
#define ICQ_AUTHERROR_NOACCESSTODATABASE	0x000A
#define ICQ_AUTHERROR_NOACCESSTORESOLVER	0x000B
#define ICQ_AUTHERROR_INVALIDDATABSEFIELDS	0x000C
#define ICQ_AUTHERROR_BADDATABASESTATUS		0x000D
#define ICQ_AUTHERROR_BADRESOLVERSTATUS		0x000E
#define ICQ_AUTHERROR_INTERNALERROR			0x000F
#define ICQ_AUTHERROR_SERVICETEMPOFFLINE	0x0010
#define ICQ_AUTHERROR_SUSPENDEDACCOUNT		0x0011
#define ICQ_AUTHERROR_DBSENDERROR			0x0012
#define ICQ_AUTHERROR_DBLINKERROR			0x0013
#define ICQ_AUTHERROR_RESERVATIONMAPERROR	0x0014
#define ICQ_AUTHERROR_RESERVATIONLINKERROR	0x0015
#define ICQ_AUTHERROR_MAXCONNECTFROMIP		0x0016
#define ICQ_AUTHERROR_MAXCONNECTFROMIPRES	0x0017
#define ICQ_AUTHERROR_RATELIMITEXCEEDEDRES	0x0018
#define ICQ_AUTHERROR_TOOHEAVILYWARNED		0x0019
#define ICQ_AUTHERROR_RESERVATIONTIMEOUT	0x001A
#define ICQ_AUTHERROR_OLDVERSIONUPGRADEREQ	0x001B
#define ICQ_AUTHERROR_OLDVERSIONUPGRADEREC	0x001C
#define ICQ_AUTHERROR_RATELIMITEXCEEDED		0x001D
#define ICQ_AUTHERROR_CANTREGISTERONNETWORK	0x001E
#define ICQ_AUTHERROR_INVALIDSECUREID		0x0020
#define ICQ_AUTHERROR_ACCOUNTSUSPENDEDAGE	0x0022
//! \}

//! \defgroup oscar_statuses Statuses
//! \ingroup oscar
//! \{
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

//! \}

//! \defgroup advanced Advanced
//! \{
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
struct ICBMCOOKIESTRUCT
{
	int nCookies1;
	int nCookies2;
};
struct SENDTEXTSTRUCT
{
	int MessageTime;
	TCHAR *pszUIN;
	ICBMCOOKIESTRUCT cookie;
	TCHAR *pszText;
	int nTextLength;
};
struct NICKINFOSTRUCT
{
	TCHAR szUIN[16];
	short sWarningLevel;
};
struct MESSAGEACKSTRUCT
{
	ICBMCOOKIESTRUCT cookie;
	short sChannel;
	TCHAR szUIN[16];
};
struct RECVMESSAGESTRUCT
{
	ICBMCOOKIESTRUCT cookie;
	short sChannel;
	NICKINFOSTRUCT NickInfo;
	TCHAR szText[1024];
};
struct FRAGMENT
{
	unsigned char id;
	unsigned char version;
	unsigned short Length;
};
//! \}