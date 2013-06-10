#include "Resources.h"
#include "windows.h"

const std::wstring GetString(int id)
{
	wchar_t buffer[512];
	LoadString(GetModuleHandle(NULL), id, buffer, sizeof(buffer) / sizeof(buffer[0]));
	return buffer;
}
