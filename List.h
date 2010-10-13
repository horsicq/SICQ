#pragma once

#include "Utils.h"

//! \defgroup list List
//! \{
struct LISTHEADER
{
	int nSignature;								//!< a Signature "List"
	int nNumberOfEntries;						//!< a number of entries
	int nSize;									//!< a size of list without LISTHEADER
};
struct LISTENTRY
{
	int nID;									//!< a uniquely number 
	int nEntrySize;								//!< a size of entry
};

#define LIST_SIGNATURE		0x7473694C			//!< "List"

//! A class for saving a data.
class List
{
private:
	char *pData;
	int nDataSize;

	bool SetHeader(char *pData,int nDataSize);
	bool IsList(char *pData,int nDataSize);
	LISTENTRY *GetListEntryByIndex(int nIndex);
	LISTENTRY *GetListEntryByID(int nID);

public:

	List(void);
	~List(void);

	bool SetData(char *pData,int nDataSize);
	bool AddEntry(int nID,char *pEntry,int nEntrySize);
	bool RemoveEntry(int nID);
	char *GetEntryByIndex(int nIndex);
	char *GetEntryByID(int nID);
};
//! \}
