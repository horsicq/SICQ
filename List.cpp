#include "List.h"

//! Constructor
List::List(void)
{

}
//! Destructor
List::~List(void)
{

}
//! Set Data
//! \param pData [in] a pointer to a buffer that contains data 
//! \param nDataSize [in] a size of data
//! \return true  if success
//! \return false if fail
bool List::SetData(char *pData,int nDataSize)
{
	if(!IsList(pData,nDataSize))
	{
		if(!SetHeader(pData,nDataSize))
		{
			return false;
		}
	}
	this->pData=pData;
	this->nDataSize=nDataSize;

	return true;
}
//! Add Entry
//! \param nID [in] a uniquely number 
//! \param pEntry [in] a pointer to a buffer that contains entry
//! \param nEntrySize [in] a size of entry
//! \return true  if success
//! \return false if fail
bool List::AddEntry(int nID,char *pEntry,int nEntrySize)
{
	char *pOffset=0;

	if(IsList(pData,nDataSize))
	{
		if(((((LISTHEADER *)pData)->nSize)+(int)sizeof(LISTENTRY)+nEntrySize)<=nDataSize)
		{
			pOffset=pData+sizeof(LISTHEADER)+((LISTHEADER *)pData)->nSize;

			((LISTENTRY *)pOffset)->nID=nID;
			((LISTENTRY *)pOffset)->nEntrySize=nEntrySize;

			pOffset+=sizeof(LISTENTRY);
			_CopyMemory(pOffset,pEntry,nEntrySize);

			((LISTHEADER *)pData)->nSize+=sizeof(LISTENTRY)+nEntrySize;
			((LISTHEADER *)pData)->nNumberOfEntries++;

			return true;
		}
	}
	
	return false;
}
//! Remove Entry
//! \param nID [in] a uniquely number 
//! \return true  if success
//! \return false if fail
bool List::RemoveEntry(int nID)
{
	if(IsList(pData,nDataSize))
	{
	}

	return false;
}
//! \param pData [in] a pointer to a buffer that contains data 
//! \param nDataSize [in] a size of data
//! \return true  if success
//! \return false if fail
bool List::IsList(char *pData,int nDataSize)
{
	if(nDataSize>=sizeof(LISTHEADER))
	{
		if(((LISTHEADER *)pData)->nSignature==LIST_SIGNATURE)
		{
			if(((LISTHEADER *)pData)->nSize<=nDataSize-(int)sizeof(LISTHEADER))
			{
				return true;
			}
		}
	}
	return false;
}
//! \Set Header
//! \param pData [in] a pointer to a buffer that contains data 
//! \param nDataSize [in] a size of data
//! \return true  if success
//! \return false if fail
bool List::SetHeader(char *pData,int nDataSize)
{
	if(nDataSize>=sizeof(LISTHEADER))
	{
		((LISTHEADER *)pData)->nSignature=LIST_SIGNATURE;
		((LISTHEADER *)pData)->nNumberOfEntries=0;
		((LISTHEADER *)pData)->nSize=0;
		return true;
	}
	return false;
}
LISTENTRY *List::GetListEntryByIndex(int nIndex)
{
	char *pOffset=pData;
	int nSize;

	if(IsList(pData,nDataSize))
	{
		if(((LISTHEADER *)pData)->nNumberOfEntries>nIndex)
		{
			pOffset+=sizeof(LISTHEADER);

			for(int i=0;i!=nIndex;i++)
			{
				nSize=((LISTENTRY *)pOffset)->nEntrySize+sizeof(LISTENTRY);
				pOffset+=nSize;
			}

			return (LISTENTRY *)pOffset;
		}
	}

	return 0;
}
LISTENTRY *List::GetListEntryByID(int nID)
{
	int nNumberOfEntries;
	LISTENTRY *pListEntry=(LISTENTRY *)pData;

	if(IsList(pData,nDataSize))
	{
		nNumberOfEntries=((LISTHEADER *)pData)->nNumberOfEntries;

		for(int i=0;i<nNumberOfEntries;i++)
		{
			pListEntry=GetListEntryByIndex(i);
			if(pListEntry->nID==nID)
			{
				return pListEntry;
			}
		}
	}
	return 0;
}
char *List::GetEntryByIndex(int nIndex)
{
	char *pOffset=(char *)GetListEntryByIndex(nIndex);
	if(pOffset)
	{
		return pOffset+sizeof(LISTENTRY);
	}
	return 0;
}
char *List::GetEntryByID(int nID)
{
	char *pOffset=(char *)GetListEntryByID(nID);
	if(pOffset)
	{
		return pOffset+sizeof(LISTENTRY);
	}
	return 0;
}