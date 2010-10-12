//! \file ICQPacket.h Header for ICQPacket.cpp
#pragma once

#include "ICQPacket_define.h"
#include "Utils.h"
#include "Strings.h"
#include "Net.h"


//! A class for low-level work with ICQ-packets
//! \defgroup sicq SICQ
//! \{
class ICQPacket
{
private:
	char *pPacket;
	int nPacketSize;

	unsigned char GetFLAPChannel();
	void SetFLAPChannel(unsigned char Channel);
	unsigned short GetFLAPSequence();
	void SetFLAPSequence(unsigned short Sequence);
	unsigned short GetFLAPDataSize();
	void SetFLAPDataSize(unsigned short DataSize);
	bool IsFLAPPacket();
	void SetFLAPHeader(char Channel,short Sequence);
	int Add_u8(unsigned char cU8);
	int Add_u16_BE(unsigned short sU16);
	int Add_u32_BE(unsigned int nU32);
	int Add_blob(char *bData,int nDataSize);
	int Add_string(TCHAR *pszString);
	int Add_TLVHeader(unsigned short Type,unsigned short Length);
	int Add_TLV_u16(unsigned short Type,unsigned short sU16);
	int Add_TLV_u32(unsigned short Type,unsigned int nU32);
	int Add_TLV_blob(unsigned short Type,char *bData,int nDataSize);
	int Add_TLV_string(unsigned short Type,TCHAR *pszString);
	int Add_TLV_empty(unsigned short Type);
	int Add_TLV_password(unsigned short Type,TCHAR *pszPassword);
	bool IsTLVPresent(unsigned short Type);
	char *GetTLVPointer(unsigned short Type);
	unsigned short GetTLVTypeFromOffset(char *pOffset);
	unsigned short GetTLVLehgthFromOffset(char *pOffset);

	char *GetSNACPointer();
	unsigned short GetSNACFamily();
	unsigned short GetSNACSubtype();
	unsigned short GetSNACFlags();
	unsigned int GetSNACRequestid();
	char *GetSNACDataPointer();
	int GetSNACDataSize();

	unsigned int Get_u32_BE_FromOffset(char *pOffset);
	unsigned short Get_u16_BE_FromOffset(char *pOffset);

	int Add_SNACHeader(unsigned short family,unsigned short subtype,unsigned short flags,unsigned int requestid);

protected:
	int GetTLV_blob(unsigned short Type,char *pBuffer,int nBufferSize);
	int GetTLV_string(unsigned short Type,TCHAR *pszBuffer,int nBufferLength);
	unsigned short GetTLV_u16(unsigned short Type);
	bool IsHelloPacket();
	bool IsSNACPresent(unsigned short family,unsigned short subtype);

	bool ReadFoodGroupsFamiliesPacket(FOODGROUPS *fgs);
	int CreateFoodGroupsVersionsPacket(int nSequence,FOODGROUPS *fgs);
	bool ReadFoodGroupsVersionsPacket(FOODGROUPS *fgs);
	int CreateRequestRatesPacket(int nSequence);
	int CreateAcceptRatesPacket(int nSequence);
	int CreateRequestRosterFirstTimePacket(int nSequence);
	int CreateLoadRosterAfterLoginPacket(int nSequence);
	int CreateSetICBMParametersPacket(int nSequence);
	int CreateRequestBuddyParametersPacket(int nSequence);
	int CreateSetStatusPacket(int nSequence,int nStatus);
	int CreateClientReadyPacket(int nSequence);
	bool IsErrorChannel();
	bool IsSignOffChannel();
public:
	ICQPacket(void);
	~ICQPacket(void);
	char *GetPacketPointer();
	int GetPacketSize();
	int Recv(SOCKET sock);
	int Send(SOCKET sock);
	int CreatePlainLoginPacket(int nSequence,TCHAR *pszUIN,TCHAR *pszPassword);
	int CreateGoodByePacket(int nSequence);
	int CreateCookiesPacket(int nSequence,char *pCookies,int nCookiesSize);
};
//! \}