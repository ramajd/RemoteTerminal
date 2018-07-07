#pragma once

#include <Windows.h>

#include <iostream>
#include <string>

using namespace std;

#define GUID_FORMAT "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"
#define GUID_ARG(guid) guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]

	
string CreateGUIDString()
{
	GUID guid;
	char strGuid[100] = { 0x00 };
	CoCreateGuid(&guid);
	sprintf_s(strGuid, GUID_FORMAT, GUID_ARG(guid));

	return string(strGuid);
}
