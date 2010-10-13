#pragma once

#include "Utils.h"

#define EPOCH_BIAS  116444736000000000i64

//! \defgroup time_functions Time functions
//! \{
int _FileTimeToUnixTime(FILETIME *pFileTeme);
void _UnixTimeToFileTime(int nUnixTime,FILETIME *pFileTeme);
int _SystemTimeAsUnixTime();
int _LocalTimeAsUnixTime();
int _UnixTimeAsString(int nUnixTime,PTSTR pszString,int nStringLength);
//! \}