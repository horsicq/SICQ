#include "StdAfx.h"
#include "ICQPacket.h"


ICQPacket::ICQPacket(void)
{
	pPacket=(char *)_Alloc(ICQPACKET_MAXPACKETSIZE);
	nPacketSize=0;
}

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

	if(_recv(sock,pPacket,sizeof(FLAP)))
	{
		if(IsFLAPPacket())
		{
			nPacketSize=GetFLAPDataSize();

#ifdef  _DEBUG
			//##################################################
			TCHAR szBuffer[256];
			wsprintf(szBuffer,TEXT("Packet Size %d(%X) bytes"),nPacketSize,nPacketSize);
			_PrintTextNS(szBuffer);
			//##################################################
#endif

			if(_recv(sock,pPacket+sizeof(FLAP),nPacketSize))
			{
				nPacketSize+=sizeof(FLAP);

//#ifdef  _DEBUG
//				//##################################################
//				_PrintHEXTable(pPacket,nPacketSize);
//				//##################################################
//#endif
			}
			else
			{
				nPacketSize=0;
			}
		}
	}

	return nPacketSize;
}
//! Sends ICQ Packet on a connected socket
//! \param sock [in] a descriptor identifying a connected socket
//! \return If no error occurs, returns the total number of bytes sent
int ICQPacket::Send(SOCKET sock)
{
	if(IsFLAPPacket())
	{
//#ifdef  _DEBUG
//		//##################################################
//		_PrintHEXTable(pPacket,nPacketSize);
//		//##################################################
//#endif
		return _send(sock,pPacket,nPacketSize);
	}
	else
	{
		return 0;
	}
}
//! Is Packet FLAP
//! \return true  if success
//! \return false if fail
//! \sa FLAP
bool ICQPacket::IsFLAPPacket()
{
	return *pPacket==ICQ_FLAPSIGNATURE;
}
//! \return FLAP channel
//! \sa FLAP
unsigned char ICQPacket::GetFLAPChannel()
{
	return ((FLAP *)pPacket)->channel;
}
//! \return FLAP sequence
//! \sa FLAP
unsigned short ICQPacket::GetFLAPSequence()
{
	return (ntohs)(((FLAP *)pPacket)->sequence);
}
//! \return FLAP DataSize
//! \sa FLAP
unsigned short ICQPacket::GetFLAPDataSize()
{
	return (ntohs)(((FLAP *)pPacket)->DataSize);
}
//! \return true  if success
//! \return false if fail
bool ICQPacket::IsHelloPacket()
{
	char channel=GetFLAPChannel();
	int DataSize=GetFLAPDataSize();

	return (channel==ICQ_CHANNEL_SIGNON)&&(DataSize==4);
}

void ICQPacket::SetFLAPChannel(unsigned char channel)
{
	((FLAP *)pPacket)->channel=channel;
}
void ICQPacket::SetFLAPSequence(unsigned short sequence)
{
	((FLAP *)pPacket)->sequence=htons(sequence);
}
void ICQPacket::SetFLAPDataSize(unsigned short DataSize)
{
	((FLAP *)pPacket)->DataSize=htons(DataSize);
}
void ICQPacket::SetFLAPHeader(char Channel,short Sequence)
{
	((FLAP *)pPacket)->id=0x2A;
	SetFLAPChannel(Channel);
	SetFLAPSequence(Sequence);
	SetFLAPDataSize(0);

	nPacketSize=sizeof(FLAP);
}
int ICQPacket::Add_u8(unsigned char cU8)
{
	*(pPacket+nPacketSize)=cU8;
	nPacketSize++;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP));

	return 1;
}
int ICQPacket::Add_u16_BE(unsigned short sU16)
{
	*(unsigned short *)(pPacket+nPacketSize)=htons(sU16);
	nPacketSize+=2;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP));

	return 2;
}
int ICQPacket::Add_u32_BE(unsigned int nU32)
{
	*(unsigned int *)(pPacket+nPacketSize)=htonl(nU32);
	nPacketSize+=4;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP));

	return 4;
}
int ICQPacket::Add_blob(char *bData,int nDataSize)
{
	_CopyMemory(pPacket+nPacketSize,bData,nDataSize);
	nPacketSize+=nDataSize;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP));

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
int ICQPacket::Add_TLVHeader(unsigned short Type,unsigned short Length)
{
	Add_u16_BE(Type);
	Add_u16_BE(Length);

	return sizeof(TLV);
}
int ICQPacket::Add_TLV_u16(unsigned short Type,unsigned short sU16)
{
	Add_TLVHeader(Type,2);
	Add_u16_BE(sU16);

	return 2+sizeof(TLV);
}
int ICQPacket::Add_TLV_u32(unsigned short Type,unsigned int nU32)
{
	Add_TLVHeader(Type,4);
	Add_u32_BE(nU32);

	return 4+sizeof(TLV);
}
int ICQPacket::Add_TLV_blob(unsigned short Type,char *bData,int nDataSize)
{
	Add_TLVHeader(Type,nDataSize);
	Add_blob(bData,nDataSize);

	return nDataSize+sizeof(TLV);
}
int ICQPacket::Add_TLV_string(unsigned short Type,TCHAR *pszString)
{
	int nStringLength=lstrlen(pszString);

	Add_TLVHeader(Type,nStringLength);
	Add_string(pszString);

	return nStringLength+sizeof(TLV);
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

	return nPasswordLength+sizeof(TLV);
}
//! CreateLoginPacket
//! \param nSequence [in] a Sequence
//! \param pszUIN [in] a pointer to a buffer that contains ICQ UIN
//! \param pszPassword [in] a pointer to a buffer that contains ICQ Password
//! \return a size of ICQ Packet
//! \sa FLAP, TLV
int ICQPacket::CreateLoginPacket(int nSequence,TCHAR *pszUIN,TCHAR *pszPassword)
{
	SetFLAPHeader(ICQ_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_string(ICQ_TLV_UIN,pszUIN);
	Add_TLV_password(ICQ_TLV_PASSWORD,pszPassword);
	Add_TLV_string(ICQ_TLV_CLIENT_ID_STRING,TEXT("ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85"));
	Add_TLV_u16(ICQ_TLV_CLIENT_ID,0x010A);
	Add_TLV_u16(ICQ_TLV_CLIENT_MAJOR_VER,0x0005);
	Add_TLV_u16(ICQ_TLV_CLIENT_MINOR_VER,0x0025);
	Add_TLV_u16(ICQ_TLV_CLIENT_LESSER_VER,0x0001);
	Add_TLV_u16(ICQ_TLV_CLIENT_BUILD_NUM,0x0E90);
	Add_TLV_u32(ICQ_TLV_DISTRIB_NUMBER,0x00005500);
	Add_TLV_string(ICQ_TLV_CLIENT_LANG,TEXT("en"));
	Add_TLV_string(ICQ_TLV_CLIENT_COUNTRY,TEXT("us"));

	return nPacketSize;
}
//! CreateGoodByePacket
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP, TLV
int ICQPacket::CreateGoodByePacket(int nSequence)
{
	SetFLAPHeader(ICQ_CHANNEL_SIGNOFF,nSequence);

	return nPacketSize;
}
//! CreateSendCookiesPacket
//! \param nSequence [in] a Sequence
//! \param pCookies [in] a pointer to a buffer that contains cookies data
//! \param nCookiesSize [in] a size of cookies data
//! \return a size of ICQ Packet
//! \sa FLAP, TLV
int ICQPacket::CreateCookiesPacket(int nSequence,char *pCookies,int nCookiesSize)
{
	SetFLAPHeader(ICQ_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_blob(ICQ_TLV_AUTH_COOKIE,pCookies,nCookiesSize);

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

	if(nDataSize>=sizeof(TLV))
	{
		pOffset+=sizeof(FLAP);

		while(nDataSize>0)
		{
			if(GetTLVTypeFromOffset(pOffset)==Type)
			{
				return pOffset;
			}
			else
			{
				nDataSize-=sizeof(TLV)+GetTLVLehgthFromOffset(pOffset);
				pOffset+=sizeof(TLV)+GetTLVLehgthFromOffset(pOffset);
			}
		}
	}
	return 0;

}
unsigned short ICQPacket::GetTLVTypeFromOffset(char *pOffset)
{
	return ntohs(((TLV *)pOffset)->Type);
}
unsigned short ICQPacket::GetTLVLehgthFromOffset(char *pOffset)
{
	return ntohs(((TLV *)pOffset)->Length);
}
//! Get TLV bytes
//! \param Type TLV type.
//! \param pBuffer [out] a pointer to a buffer that receives TLV bytes.
//! \param nBufferSize [in] a size, in characters, of the buffer indicated by pBuffer.
//! \return if no error occurs, returns the number of bytes.
//! \return The function returns 0 if it does not succeed.
//! \sa TLV
int ICQPacket::GetTLV_blob(unsigned short Type,char *pBuffer,int nBufferSize)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferSize)
		{
			_CopyMemory(pBuffer,pOffset+sizeof(TLV),nSize);
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
//! \sa TLV
int ICQPacket::GetTLV_string(unsigned short Type,TCHAR *pszBuffer,int nBufferLength)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferLength-1)
		{
			char *pBuffer=(char *)_Alloc(nSize+1);
			_CopyMemory(pBuffer,pOffset+sizeof(TLV),nSize);
			*(pBuffer+nSize)=0;
			_CharsToString(pszBuffer,nBufferLength,pBuffer);
			_Free(pBuffer);

			return nSize;
		}
	}
	return 0;
}

char *ICQPacket::GetSNACPointer()
{
	if(GetPacketSize()>=sizeof(FLAP)+sizeof(SNAC))
	{
		return GetPacketPointer()+sizeof(FLAP);
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
		return ntohs(((SNAC *)pOffset)->family);
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
		return ntohs(((SNAC *)pOffset)->subtype);
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
		return ntohs(((SNAC *)pOffset)->flags);
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
		return ntohl(((SNAC *)pOffset)->requestid);
	}
	else
	{
		return 0;
	}
}
bool ICQPacket::IsSNACPresent(unsigned short family,unsigned short subtype)
{
	return ((GetSNACFamily()==family)&&(GetSNACSubtype()==subtype));
}
char *ICQPacket::GetSNACDataPointer()
{
	if(GetSNACDataSize())
	{
		return GetSNACPointer()+sizeof(SNAC);
	}
	else
	{
		return 0;
	}
}
int ICQPacket::GetSNACDataSize()
{
	int nSize=GetPacketSize()-(sizeof(FLAP)+sizeof(SNAC));
	if(nSize>0)
	{
		return nSize;
	}
	else
	{
		return 0;
	}
}

bool ICQPacket::ReadFoodGroupsFamiliesPacket(FOODGROUPS *fgs)
{
	_ZeroMemory(fgs,sizeof(FOODGROUPS));

	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
	{
		while(nSize>0)
		{
			switch(Get_u16_BE_FromOffset(pOffset))
			{
			case ICQ_SNAC_FOODGROUP_OSERVICE:
				fgs->Oservice.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				fgs->Locate.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				fgs->Buddy.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				fgs->ICBM.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				fgs->Invite.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				fgs->Popup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				fgs->PD.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				fgs->Lookup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				fgs->Stats.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				fgs->Trans.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				fgs->SSI.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				fgs->Spec.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				fgs->Unk1.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				fgs->Unk2.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				fgs->Unk3.Support=true;
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

int ICQPacket::CreateFoodGroupsVersionsPacket(int nSequence,FOODGROUPS *fgs)
{
	SetFLAPHeader(ICQ_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTVERSIONS,0,nSequence);

	if(fgs->Unk1.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_UNK1);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Oservice.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_OSERVICE);
		Add_u16_BE(0x0004); // Version
	}
	if(fgs->SSI.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SSI);
		Add_u16_BE(0x0004); // Version
	}
	if(fgs->Locate.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOCATE);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Buddy.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_BUDDY);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Spec.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SPEC);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->ICBM.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_ICBM);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Invite.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_INVITE);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->PD.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_PD);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Lookup.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOOKUP);
		Add_u16_BE(0x0001); // Version
	}
	if(fgs->Stats.Support)
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

int ICQPacket::Add_SNACHeader(unsigned short family,unsigned short subtype,unsigned short flags,unsigned int requestid)
{
	Add_u16_BE(family);
	Add_u16_BE(subtype);
	Add_u16_BE(flags);
	Add_u32_BE(requestid);

	return sizeof(SNAC);
}

bool ICQPacket::ReadFoodGroupsVersionsPacket(FOODGROUPS *fgs)
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
				fgs->Oservice.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				fgs->Locate.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				fgs->Buddy.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				fgs->ICBM.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				fgs->Invite.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				fgs->Popup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				fgs->PD.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				fgs->Lookup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				fgs->Stats.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				fgs->Trans.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				fgs->SSI.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				fgs->Spec.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				fgs->Unk1.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				fgs->Unk2.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				fgs->Unk3.Version=Get_u16_BE_FromOffset(pOffset+2);
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
int ICQPacket::CreateRequestRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTRATELIMITS,0,nSequence);

	return nPacketSize;
}
int ICQPacket::CreateAcceptRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS,0,nSequence);

	Add_u16_BE(0x0001);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0003);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0005);

	return nPacketSize;
}
int ICQPacket::CreateRequestRosterFirstTimePacket(int nSequence)
{
	SetFLAPHeader(ICQ_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME,0,nSequence);

	return nPacketSize;
}
int ICQPacket::CreateLoadRosterAfterLoginPacket(int nSequence)
{
	SetFLAPHeader(ICQ_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN,0,nSequence);

	return nPacketSize;
}