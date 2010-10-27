#include "ICQPacket.h"


//! Constructor
ICQPacket::ICQPacket(void)
{
	pPacket=(char *)_Alloc(ICQPACKET_MAXPACKETSIZE);
	nPacketSize=0;
}
//! Destructor
ICQPacket::~ICQPacket(void)
{
	_Free(pPacket);
}
//! GetPacketPointer
//! \return a pointer to ICQ Packet
char *ICQPacket::GetPacketPointer()
{
	return pPacket;
}
//! GetPacketSize
//! \return a size of ICQ Packet
int ICQPacket::GetPacketSize()
{
	return nPacketSize;
}
//! Receives ICQ Packet from a connected socket 
//! \param sock [in] a descriptor identifying a connected socket
//! \return If no error occurs, returns the number of bytes received
int ICQPacket::Recv(SOCKET sock)
{
	nPacketSize=0;

	if(_recv(sock,pPacket,sizeof(FLAP_HEADER)))
	{
		if(IsFLAPPacket())
		{
			nPacketSize=GetFLAPDataSize();

			if(_recv(sock,pPacket+sizeof(FLAP_HEADER),nPacketSize))
			{
				nPacketSize+=sizeof(FLAP_HEADER);
			}
			else
			{
				nPacketSize=0;
			}
		}
		else
		{

#ifdef  _DEBUG
			//##################################################
			_PrintDebugTextNS(TEXT("It's not FLAP"));
			//##################################################
#endif
		}


	}

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("----------[recv]---------------"));
	TCHAR szBuffer[256];
	wsprintf(szBuffer,TEXT("Packet Size %d(%X) bytes"),nPacketSize,nPacketSize);
	_PrintDebugTextNS(szBuffer);
	_PrintDebugHEXTable(GetPacketPointer(),GetPacketSize());
	_PrintDebugTextNS(TEXT("-------------------------------"));
	//##################################################
#endif

	return nPacketSize;
}
//! Sends ICQ Packet on a connected socket
//! \param sock [in] a descriptor identifying a connected socket
//! \return If no error occurs, returns the total number of bytes sent
int ICQPacket::Send(SOCKET sock)
{
	int nResult=0;

	if(IsFLAPPacket())
	{
		nResult=_send(sock,pPacket,nPacketSize);

		if(nResult)
		{
#ifdef  _DEBUG
			//##################################################
			_PrintDebugTextNS(TEXT("----------[send]---------------"));
			TCHAR szBuffer[256];
			wsprintf(szBuffer,TEXT("Packet Size %d(%X) bytes"),nPacketSize,nPacketSize);
			_PrintDebugTextNS(szBuffer);
			_PrintDebugHEXTable(GetPacketPointer(),GetPacketSize());
			_PrintDebugTextNS(TEXT("-------------------------------"));
			//##################################################
#endif
		}
	}
	
	return nResult;
}
//! Is Packet FLAP
//! \return true  if success
//! \return false if fail
//! \sa FLAP_HEADER
bool ICQPacket::IsFLAPPacket()
{
	return *pPacket==ICQ_FLAP_SIGNATURE;
}
//! \return FLAP channel
//! \sa FLAP_HEADER
unsigned char ICQPacket::GetFLAPChannel()
{
	return ((FLAP_HEADER *)pPacket)->channel;
}
//! \return FLAP sequence
//! \sa FLAP_HEADER
unsigned short ICQPacket::GetFLAPSequence()
{
	return (ntohs)(((FLAP_HEADER *)pPacket)->sequence);
}
//! \return FLAP DataSize
//! \sa FLAP_HEADER
unsigned short ICQPacket::GetFLAPDataSize()
{
	return (ntohs)(((FLAP_HEADER *)pPacket)->lenght);
}
//! \return true  if success
//! \return false if fail
bool ICQPacket::IsHelloPacket()
{
	char channel=GetFLAPChannel();
	int DataSize=GetFLAPDataSize();

	if((channel==ICQ_FLAP_CHANNEL_SIGNON)&&(DataSize==4))
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's Hello Packet"));
		//##################################################
#endif
		return true;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's NOT Hello Packet"));
		//##################################################
#endif
		return false;
	}
}

void ICQPacket::SetFLAPChannel(unsigned char channel)
{
	((FLAP_HEADER *)pPacket)->channel=channel;
}
void ICQPacket::SetFLAPSequence(unsigned short sequence)
{
	((FLAP_HEADER *)pPacket)->sequence=htons(sequence);
}
void ICQPacket::SetFLAPDataSize(unsigned short DataSize)
{
	((FLAP_HEADER *)pPacket)->lenght=htons(DataSize);
}
void ICQPacket::SetFLAPHeader(char Channel,short Sequence)
{
	((FLAP_HEADER *)pPacket)->signature=0x2A;
	SetFLAPChannel(Channel);
	SetFLAPSequence(Sequence);
	SetFLAPDataSize(0);

	nPacketSize=sizeof(FLAP_HEADER);
}
int ICQPacket::Add_u8(unsigned char cU8)
{
	*(pPacket+nPacketSize)=cU8;
	nPacketSize++;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 1;
}
int ICQPacket::Add_u16_BE(unsigned short sU16)
{
	*(unsigned short *)(pPacket+nPacketSize)=htons(sU16);
	nPacketSize+=2;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 2;
}
int ICQPacket::Add_u32_BE(unsigned int nU32)
{
	*(unsigned int *)(pPacket+nPacketSize)=htonl(nU32);
	nPacketSize+=4;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 4;
}
int ICQPacket::Add_blob(char *bData,int nDataSize)
{
	_CopyMemory(pPacket+nPacketSize,bData,nDataSize);
	nPacketSize+=nDataSize;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return nDataSize;
}
int ICQPacket::Add_string(TCHAR *pszString)
{
	int nStringLength=lstrlen(pszString);
	char *pChars=(char *)_Alloc(nStringLength+1);

	_StringToChars(pChars,nStringLength+1,pszString);
	Add_blob(pChars,nStringLength);

	_Free(pChars);

	return nStringLength;
}
int ICQPacket::Add_string08(TCHAR *pszString)
{
	int nLength=lstrlen(pszString);

	Add_u8((char)nLength);
	nLength+=Add_string(pszString);

	return nLength;
}
int ICQPacket::Add_TLVHeader(unsigned short Type,unsigned short Length)
{
	Add_u16_BE(Type);
	Add_u16_BE(Length);

	return sizeof(TLV_HEADER);
}
int ICQPacket::Add_TLV_u16(unsigned short Type,unsigned short sU16)
{
	Add_TLVHeader(Type,2);
	Add_u16_BE(sU16);

	return 2+sizeof(TLV_HEADER);
}
int ICQPacket::Add_TLV_u32(unsigned short Type,unsigned int nU32)
{
	Add_TLVHeader(Type,4);
	Add_u32_BE(nU32);

	return 4+sizeof(TLV_HEADER);
}
int ICQPacket::Add_TLV_blob(unsigned short Type,char *bData,int nDataSize)
{
	Add_TLVHeader(Type,nDataSize);
	Add_blob(bData,nDataSize);

	return nDataSize+sizeof(TLV_HEADER);
}
int ICQPacket::Add_TLV_string(unsigned short Type,TCHAR *pszString)
{
	int nStringLength=lstrlen(pszString);

	Add_TLVHeader(Type,nStringLength);
	Add_string(pszString);

	return nStringLength+sizeof(TLV_HEADER);
}
int ICQPacket::Add_TLV_empty(unsigned short Type)
{
	Add_TLVHeader(Type,0);

	return 0;
}
int ICQPacket::Add_TLV_password(unsigned short Type,TCHAR *pszPassword)
{
	unsigned char MagicXor[]={0xF3, 0x26, 0x81, 0xC4, 0x39, 0x86, 0xDB, 0x92, 0x71, 0xA3, 0xB9, 0xE6, 0x53, 0x7A, 0x95, 0x7C};

	int nPasswordLength=lstrlen(pszPassword);
	char *pChars=(char *)_Alloc(nPasswordLength+1);

	_StringToChars(pChars,nPasswordLength+1,pszPassword);

	for(int i=0;i<nPasswordLength;i++)
	{
		pChars[i]^=MagicXor[i];
	}

	Add_TLVHeader(Type,nPasswordLength);
	Add_blob(pChars,nPasswordLength);

	_Free(pChars);

	return nPasswordLength+sizeof(TLV_HEADER);
}
//! CreatePlainLoginPacket
//! \param nSequence [in] a Sequence
//! \param pszUIN [in] a pointer to a buffer that contains ICQ UIN
//! \param pszPassword [in] a pointer to a buffer that contains ICQ Password
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreatePlainLoginPacket(int nSequence,TCHAR *pszUIN,TCHAR *pszPassword)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_string(ICQ_TLV_UIN,pszUIN);
	Add_TLV_password(ICQ_TLV_PASSWORD,pszPassword);
	Add_TLV_string(ICQ_TLV_CLIENTIDSTRING,TEXT("ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85"));
	Add_TLV_u16(ICQ_TLV_CLIENTID,0x010A);
	Add_TLV_u16(ICQ_TLV_CLIENTMAJORVER,0x0005);
	Add_TLV_u16(ICQ_TLV_CLIENTMINORVER,0x0025);
	Add_TLV_u16(ICQ_TLV_CLIENTLESSERVER,0x0001);
	Add_TLV_u16(ICQ_TLV_CLIENTBUILDNUM,0x0E90);
	Add_TLV_u32(ICQ_TLV_DISTRIBNUMBER,0x00005500);
	Add_TLV_string(ICQ_TLV_CLIENTLANG,TEXT("en"));
	Add_TLV_string(ICQ_TLV_CLIENTCOUNTRY,TEXT("us"));

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Plain Login Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! CreateGoodByePacket
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateGoodByePacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNOFF,nSequence);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Goodbye Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! CreateSendCookiesPacket
//! \param nSequence [in] a Sequence
//! \param pCookies [in] a pointer to a buffer that contains cookies data
//! \param nCookiesSize [in] a size of cookies data
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateCookiesPacket(int nSequence,char *pCookies,int nCookiesSize)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_blob(ICQ_TLV_AUTHCOOKIE,pCookies,nCookiesSize);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Cookies Packet"));
	//##################################################
#endif

	return nPacketSize;
}

bool ICQPacket::IsTLVPresent(unsigned short Type)
{
	return (GetTLVPointer(Type)!=0);
}
char *ICQPacket::GetTLVPointer(unsigned short Type)
{
	int nDataSize=nPacketSize;
	char *pOffset=pPacket;

	if(nDataSize>=sizeof(TLV_HEADER))
	{
		pOffset+=sizeof(FLAP_HEADER);

		while(nDataSize>0)
		{
			if(GetTLVTypeFromOffset(pOffset)==Type)
			{
				return pOffset;
			}
			else
			{
				nDataSize-=sizeof(TLV_HEADER)+GetTLVLehgthFromOffset(pOffset);
				pOffset+=sizeof(TLV_HEADER)+GetTLVLehgthFromOffset(pOffset);
			}
		}
	}
	return 0;

}
unsigned short ICQPacket::GetTLVTypeFromOffset(char *pOffset)
{
	return ntohs(((TLV_HEADER *)pOffset)->type);
}
unsigned short ICQPacket::GetTLVLehgthFromOffset(char *pOffset)
{
	return ntohs(((TLV_HEADER *)pOffset)->length);
}

//! Get TLV bytes
//! \param Type TLV type.
//! \param pBuffer [out] a pointer to a buffer that receives TLV bytes.
//! \param nBufferSize [in] a size, in characters, of the buffer indicated by pBuffer.
//! \return if no error occurs, returns the number of bytes.
//! \return The function returns 0 if it does not succeed.
//! \sa TLV_HEADER
int ICQPacket::GetTLV_blob(unsigned short Type,char *pBuffer,int nBufferSize)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferSize)
		{
			_CopyMemory(pBuffer,pOffset+sizeof(TLV_HEADER),nSize);
			return nSize;
		}
	}
	return 0;
}
//! Get TLV string
//! \param Type TLV type.
//! \param pszBuffer [out] a pointer to a buffer that receives TLV string.
//! \param nBufferLength [in] a size, in characters, of the buffer indicated by pszBuffer.
//! \return if no error occurs, returns the number of symbols.
//! \return The function returns 0 if it does not succeed.
//! \sa TLV_HEADER
int ICQPacket::GetTLV_string(unsigned short Type,TCHAR *pszBuffer,int nBufferLength)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferLength-1)
		{
			char *pBuffer=(char *)_Alloc(nSize+1);
			_CopyMemory(pBuffer,pOffset+sizeof(TLV_HEADER),nSize);
			*(pBuffer+nSize)=0;
			_CharsToString(pszBuffer,nBufferLength,pBuffer);
			_Free(pBuffer);

			return nSize;
		}
	}
	return 0;
}
//! Get TLV short int
//! \param Type TLV type.
//! \return TLV short int
//! \sa TLV_HEADER
unsigned short ICQPacket::GetTLV_u16(unsigned short Type)
{

	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		if(GetTLVLehgthFromOffset(pOffset)==2)
		{
			return htons(*((unsigned short *)(pOffset+sizeof(TLV_HEADER))));
		}
	}
	return 0;
}

char *ICQPacket::GetSNACPointer()
{
	if(GetPacketSize()>=sizeof(FLAP_HEADER)+sizeof(SNAC_HEADER))
	{
		return GetPacketPointer()+sizeof(FLAP_HEADER);
	}
	else
	{
		return 0;
	}
}

unsigned short ICQPacket::GetSNACFamily()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->family);
	}
	else
	{
		return 0;
	}
}
unsigned short ICQPacket::GetSNACSubtype()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->subtype);
	}
	else
	{
		return 0;
	}
}
unsigned short ICQPacket::GetSNACFlags()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->flags);
	}
	else
	{
		return 0;
	}
}
unsigned int ICQPacket::GetSNACRequestid()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohl(((SNAC_HEADER *)pOffset)->requestid);
	}
	else
	{
		return 0;
	}
}
//! IsSNACPresent
//! \param family [in] a SNAC family
//! \param subtype [in] a SNAC subtype
//! \return true if success
//! \return false if fail
//! \sa FLAP_HEADER, SNAC_HEADER
bool ICQPacket::IsSNACPresent(unsigned short family,unsigned short subtype)
{
#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("----------[IsSNACPresent]------"));
	//##################################################
#endif
	if((GetSNACFamily()==family)&&(GetSNACSubtype()==subtype))
	{
#ifdef  _DEBUG
		TCHAR szBuffer[256];
		wsprintf(szBuffer,TEXT("It's SNAC(family=%X, subtype=%X)"),family,subtype);
		_PrintDebugTextNS(szBuffer);

		if(family==ICQ_SNAC_FOODGROUP_OSERVICE)
		{
			if(subtype==ICQ_SNAC_OSERVICE_CLIENTREADY)
			{
				_PrintDebugTextNS(TEXT("OService: Client Ready"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_FAMILIES)
			{
				_PrintDebugTextNS(TEXT("OService: Families"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_REQUESTRATELIMITS)
			{
				_PrintDebugTextNS(TEXT("OService: Request Rate Limits"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_RATELIMITS)
			{
				_PrintDebugTextNS(TEXT("OService: Rate Limits"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS)
			{
				_PrintDebugTextNS(TEXT("OService: Accept Rate Limits"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_ONLINEINFO)
			{
				_PrintDebugTextNS(TEXT("OService: Online Info"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY)
			{
				_PrintDebugTextNS(TEXT("OService: Message Of The Day"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_WELLKNOWNURLS)
			{
				_PrintDebugTextNS(TEXT("OService: Well Known URLs"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_REQUESTVERSIONS)
			{
				_PrintDebugTextNS(TEXT("OService: Request Versions"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_VERSIONS)
			{
				_PrintDebugTextNS(TEXT("OService: Versions"));
			}
			else if(subtype==ICQ_SNAC_OSERVICE_SETSTATUS)
			{
				_PrintDebugTextNS(TEXT("OService: Set Status"));
			}
			else if(ICQ_SNAC_OSERVICE_EXTENDEDSTATUS)
			{
				_PrintDebugTextNS(TEXT("OService: Extended Status"));
			}
		}
		else if(family==ICQ_SNAC_FOODGROUP_BUDDY)
		{
			if(subtype==ICQ_SNAC_BUDDY_REQUESTPARAMETERS)
			{
				_PrintDebugTextNS(TEXT("Buddy: Request Parameters"));
			}
			else if(subtype==ICQ_SNAC_BUDDY_PARAMETERS)
			{
				_PrintDebugTextNS(TEXT("Buddy: Parameters"));
			}
			else if(subtype==ICQ_SNAC_BUDDY_USERONLINE)
			{
				_PrintDebugTextNS(TEXT("Buddy: User Online"));
			}
			else if(subtype==ICQ_SNAC_BUDDY_USEROFFLINE)
			{
				_PrintDebugTextNS(TEXT("Buddy: User Offline"));
			}
		}
		else if(family==ICQ_SNAC_FOODGROUP_ICBM)
		{
			if(subtype==ICQ_SNAC_ICBM_ERROR)
			{
				_PrintDebugTextNS(TEXT("Buddy: Error"));
			}
			else if(subtype==ICQ_SNAC_ICBM_SETPARAMETERS)
			{
				_PrintDebugTextNS(TEXT("ICBM: Set Parameters"));
			}
			else if(subtype==ICQ_SNAC_ICBM_SENDMESSAGE)
			{
				_PrintDebugTextNS(TEXT("ICBM: Send Message"));
			}
			else if(subtype==ICQ_SNAC_ICBM_RECVMESSAGE)
			{
				_PrintDebugTextNS(TEXT("ICBM: Recv Message"));
			}
			else if(subtype==ICQ_SNAC_ICBM_MESSAGEACK)
			{
				_PrintDebugTextNS(TEXT("ICBM: Message Ack"));
			}
		}
		else if(family==ICQ_SNAC_FOODGROUP_STATS)
		{
			if(subtype==ICQ_SNAC_STATS_SETMINIMUMINTERVAL)
			{
				_PrintDebugTextNS(TEXT("Stats: Set Minimum Interval"));
			}
		}
		else if(family==ICQ_SNAC_FOODGROUP_SSI)
		{
			if(subtype==ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME)
			{
				_PrintDebugTextNS(TEXT("SSI: Request Roster(First Time)"));
			}
			else if(subtype==ICQ_SNAC_SSI_ROSTER)
			{
				_PrintDebugTextNS(TEXT("SSI: Roster"));
			}
			else if(subtype==ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN)
			{
				_PrintDebugTextNS(TEXT("SSI: Load Roster After Login"));
			}
		}
		_PrintDebugTextNS(TEXT("-------------------------------"));
		//##################################################
#endif

		return true;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		TCHAR szBuffer[256];
		wsprintf(szBuffer,TEXT("It's NOT SNAC(family=%X, subtype=%X)"),family,subtype);
		_PrintDebugTextNS(szBuffer);
		_PrintDebugTextNS(TEXT("-------------------------------"));
		//##################################################
#endif

		return false;
	}
}
char *ICQPacket::GetSNACDataPointer()
{
	if(GetSNACDataSize())
	{
		return GetSNACPointer()+sizeof(SNAC_HEADER);
	}
	else
	{
		return 0;
	}
}
int ICQPacket::GetSNACDataSize()
{
	int nSize=GetPacketSize()-(sizeof(FLAP_HEADER)+sizeof(SNAC_HEADER));
	if(nSize>0)
	{
		return nSize;
	}
	else
	{
		return 0;
	}
}
//! ReadFoodGroupsFamiliesPacket
//! \param pFgs [in] a pointer to FOODGROUPS structure
//! \return true if success
//! \return false if fail
//! \sa FLAP_HEADER, TLV_HEADER, FOODGROUPS
bool ICQPacket::ReadFoodGroupsFamiliesPacket(FOODGROUPS *pFgs)
{
	_ZeroMemory(pFgs,sizeof(FOODGROUPS));

	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
	{
		while(nSize>0)
		{
			switch(Get_u16_BE_FromOffset(pOffset))
			{
			case ICQ_SNAC_FOODGROUP_OSERVICE:
				pFgs->Oservice.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				pFgs->Locate.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				pFgs->Buddy.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				pFgs->ICBM.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				pFgs->Invite.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				pFgs->Popup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				pFgs->PD.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				pFgs->Lookup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				pFgs->Stats.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				pFgs->Trans.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				pFgs->SSI.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				pFgs->Spec.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				pFgs->Unk1.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				pFgs->Unk2.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				pFgs->Unk3.Support=true;
				break;
			}

			pOffset+=2;
			nSize-=2;
		}

		return true;
	}
	else
	{
		return false;
	}
}
//! CreateFoodGroupsVersionsPacket
//! \param nSequence [in] a Sequence
//! \param pFgs [in] a pointer to FOODGROUPS structure
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER, FOODGROUPS
int ICQPacket::CreateFoodGroupsVersionsPacket(int nSequence,FOODGROUPS *pFgs)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTVERSIONS,0,nSequence);

	if(pFgs->Unk1.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_UNK1);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Oservice.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_OSERVICE);
		Add_u16_BE(0x0004); // Version
	}
	if(pFgs->SSI.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SSI);
		Add_u16_BE(0x0004); // Version
	}
	if(pFgs->Locate.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOCATE);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Buddy.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_BUDDY);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Spec.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SPEC);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->ICBM.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_ICBM);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Invite.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_INVITE);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->PD.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_PD);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Lookup.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOOKUP);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Stats.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_STATS);
		Add_u16_BE(0x0001); // Version
	}

	return nPacketSize;
}

unsigned int ICQPacket::Get_u32_BE_FromOffset(char *pOffset)
{
	return ntohl(*((unsigned int *)pOffset));
}
unsigned short ICQPacket::Get_u16_BE_FromOffset(char *pOffset)
{
	return ntohs(*((unsigned short *)pOffset));
}
unsigned char ICQPacket::Get_u8_FromOffset(char *pOffset)
{
	return *pOffset;
}
int ICQPacket::Get_string08(char *pOffset,TCHAR *pszString,int nStringLength)
{
	int nSize=Get_u8_FromOffset(pOffset);

	if(nSize<nStringLength)
	{
		pOffset++;
		return _CharsToString(pszString,nStringLength,pOffset);
	}

	return 0;
}

int ICQPacket::Add_SNACHeader(unsigned short family,unsigned short subtype,unsigned short flags,unsigned int requestid)
{
	Add_u16_BE(family);
	Add_u16_BE(subtype);
	Add_u16_BE(flags);
	Add_u32_BE(requestid);

	return sizeof(SNAC_HEADER);
}
//! ReadFoodGroupsVersionsPacket
//! \param pFgs [in] a pointer to FOODGROUPS structure
//! \return true if success
//! \return false if fail
//! \sa FLAP_HEADER, TLV_HEADER, FOODGROUPS
bool ICQPacket::ReadFoodGroupsVersionsPacket(FOODGROUPS *pFgs)
{
	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_VERSIONS))
	{
		while(nSize>0)
		{
			switch(Get_u16_BE_FromOffset(pOffset))
			{
			case ICQ_SNAC_FOODGROUP_OSERVICE:
				pFgs->Oservice.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				pFgs->Locate.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				pFgs->Buddy.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				pFgs->ICBM.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				pFgs->Invite.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				pFgs->Popup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				pFgs->PD.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				pFgs->Lookup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				pFgs->Stats.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				pFgs->Trans.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				pFgs->SSI.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				pFgs->Spec.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				pFgs->Unk1.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				pFgs->Unk2.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				pFgs->Unk3.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			}

			pOffset+=4;
			nSize-=4;
		}

		return true;
	}
	else
	{
		return false;
	}
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateRequestRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTRATELIMITS,0,nSequence);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Request Rates Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateAcceptRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS,0,nSequence);

	Add_u16_BE(0x0001);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0003);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0005);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Accept Rates Packet"));
	//##################################################
#endif
	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateRequestRosterFirstTimePacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME,0,nSequence);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Request Roster First Time Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateLoadRosterAfterLoginPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN,0,nSequence);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Load Roster After Login Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateSetICBMParametersPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_SETPARAMETERS,0,nSequence);

	Add_u16_BE(0x0002);
	Add_u32_BE(0x00000003); // ICBM__PARAMETER_FLAGS
	Add_u16_BE(0x1F40);
	Add_u16_BE(0x03E7);
	Add_u16_BE(0x03E7);
	Add_u32_BE(0x00000000);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Set ICBM Parameters Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateRequestBuddyParametersPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_BUDDY,ICQ_SNAC_BUDDY_REQUESTPARAMETERS,0,nSequence);

	Add_u16_BE(0x0005);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0003);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Request Buddy Parameters Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \param nStatus [in] a ICQ status
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateSetStatusPacket(int nSequence,int nStatus)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_SETSTATUS,0,nSequence);

	Add_TLV_u32(ICQ_TLV_USERSTATUS,nStatus);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Create Set Status Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int ICQPacket::CreateClientReadyPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_CLIENTREADY,0,nSequence);

	Add_u16_BE(0x0022);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0001);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0013);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0002);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0003);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0015);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0004);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0006);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0009);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x000A);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x000B);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Client Ready Packet"));
	//##################################################
#endif

	return nPacketSize;
}
//! IsErrorChannel
//! \return true if error channel
//! \return false if not
//! \sa FLAP_HEADER
bool ICQPacket::IsErrorChannel()
{
	if(GetFLAPChannel()==ICQ_FLAP_CHANNEL_ERROR)
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's Error Channel"));
		//##################################################
#endif
		return true;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's NOT Error Channel"));
		//##################################################
#endif
		return false;
	}
}
//! IsSignOffChannel
//! \return true if sign off channel
//! \return false if not
//! \sa FLAP_HEADER
bool ICQPacket::IsSignOffChannel()
{
	if(GetFLAPChannel()==ICQ_FLAP_CHANNEL_SIGNOFF)
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's Sigh Off Channel"));
		//##################################################
#endif
	return true;
	}
	else
	{
#ifdef  _DEBUG
		//##################################################
		_PrintDebugTextNS(TEXT("It's NOT Sigh Off Channel"));
		//##################################################
#endif
		return false;
	}
}
//! CreateSendTextUnicodePacket
//! \param nSequence [in] a Sequence
//! \param pSts [in] a pointer to SENDTEXTSTRUCT structure
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER, SENDTEXTSTRUCT
int ICQPacket::CreateSendTextUnicodePacket(int nSequence,SENDTEXTSTRUCT *pSts)
{
	char *pMemory;
	int nMessageSize=pSts->nTextLength*sizeof(WCHAR);

	pMemory=(char *)_Alloc(nMessageSize);
	_StringToUnicode((WCHAR *)pMemory,pSts->nTextLength,pSts->pszText);
	_UnicodeToNet((WCHAR *)pMemory,pSts->nTextLength,(WCHAR *)pMemory);

	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_SENDMESSAGE,0,nSequence);

	Add_u32_BE(pSts->cookie.nCookies1);
	Add_u32_BE(pSts->cookie.nCookies2);
	Add_u16_BE(0x0001);	// Channel
	Add_string08(pSts->pszUIN);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x000E+nMessageSize);
	Add_u8(0x05);
	Add_u8(0x01);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0106);
	Add_u8(0x01);
	Add_u8(0x01);
	Add_u16_BE(0x0004+nMessageSize);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0000);
	Add_blob(pMemory,nMessageSize);
	Add_TLV_blob(ICQ_TLV_USERSTATUS,0,0);

	_Free(pMemory);

#ifdef  _DEBUG
	//##################################################
	_PrintDebugTextNS(TEXT("Send Text Unicode"));
	_PrintDebugTextNS(pSts->pszUIN);
	_PrintDebugTextNS(pSts->pszText);
	//##################################################
#endif

	return nPacketSize;
}
bool ICQPacket::ReadMessageAckPacket(MESSAGEACKSTRUCT *pMas)
{
	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_MESSAGEACK))
	{
		pMas->cookie.nCookies1=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pMas->cookie.nCookies2=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pMas->sChannel=Get_u16_BE_FromOffset(pOffset);
		pOffset+=2;
		Get_string08(pOffset,pMas->szUIN,sizeof(pMas->szUIN));

		return true;
	}
	else
	{
		return false;
	}
}
bool ICQPacket::ReadRecvMessagePacket(RECVMESSAGESTRUCT *pRms)
{
	
	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_RECVMESSAGE))
	{
		pRms->cookie.nCookies1=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pRms->cookie.nCookies2=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pRms->sChannel=Get_u16_BE_FromOffset(pOffset);
		pOffset+=2;
		
		pOffset+=ReadNickInfoFromOffset(pOffset,&(pRms->NickInfo));


		return true;
	}
	else
	{
		return false;
	}
}
int ICQPacket::ReadNickInfoFromOffset(char *pOffset,NICKINFOSTRUCT *pNis)
{
	int nTLVs=0;
	short sTLVType,sTLVLength;
	char *pData=pOffset;

	pData+=Get_string08(pData,pNis->szUIN,sizeof(pNis->szUIN));
	pData++;
	pNis->sWarningLevel=Get_u16_BE_FromOffset(pData);
	pData+=2;
	nTLVs=Get_u16_BE_FromOffset(pData);
	pData+=2;

	for(int i=0;i<nTLVs;i++)
	{
		sTLVType=GetTLVTypeFromOffset(pOffset);
		sTLVLength=GetTLVLehgthFromOffset(pOffset);
		pData+=sizeof(TLV_HEADER);

		if(sTLVType==0x0001) // User Class
		{
			
		}
		else if(sTLVType==0x0003) // Online since
		{
			
		}
		else if(sTLVType==0x0005) // Registered
		{
			
		}
		else if(sTLVType==0x0006) // Status
		{
			
		}
		else if(sTLVType==0x000A) // 
		{
			
		}
		else if(sTLVType==0x000C) // CliToCli
		{
			
		}
		else if(sTLVType==0x000F) // Session Timer
		{
			
		}

		pData+=sTLVLength;
	}


	return pData-pOffset;
}