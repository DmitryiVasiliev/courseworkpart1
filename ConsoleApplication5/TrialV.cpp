#include "stdafx.h"
#include <Windows.h>
#include "TrialV.h"
using namespace std;
#define LaunchCounts 30
#define PathKey _T("Software\\RegistrySample\\")
#define NameKey _T("Launch Counts")
TrialV::~TrialV() { RegCloseKey(hKey); }
bool TrialV::Check() { return check; }
void TrialV::AutoRun(){
	
  DWORD size = MAX_PATH;
  if (GetValue() == 1) {
	  HKEY hk;
	  wchar_t szPath[0x100];
	  GetModuleFileName(NULL, szPath, sizeof(szPath));
	  RegCreateKeyEx(HKEY_CURRENT_USER,
		  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		  0,
		  NULL,
		  REG_OPTION_NON_VOLATILE,
		  KEY_WRITE,
		  NULL,
		  &hk,
		  NULL);

	  if (hk)
	  {
		  RegSetValueEx(hk, L"CheckTime", 0, REG_SZ, (const BYTE*)&szPath, sizeof(szPath));
		  RegCloseKey(hk);
	  }
  
  }


}
TrialV::TrialV() {
	RegOpenKeyEx(HKEY_CURRENT_USER, PathKey, 0, KEY_ALL_ACCESS, &hKey);
	DWORD size = MAX_PATH;
	if (RegQueryValueEx(hKey, NameKey, NULL, NULL, (BYTE*)&value, &size) ==
		ERROR_SUCCESS) {
		if (value + 1 <= LaunchCounts) {
			check = true;
			SetValue(value);
		}
		else
			check = false;
	}
	else {
		check = true;
		CreateValue();
	}
}
void TrialV::SetValue(DWORD count) {
	DWORD c = count + 1;
	RegSetValueEx(hKey, NameKey, 0, REG_DWORD, (const BYTE*)&c, sizeof(c));
}

DWORD TrialV::GetValue() {
	DWORD size = MAX_PATH;
	RegQueryValueEx(hKey, NameKey, NULL, NULL, (BYTE*)&value, &size);
	return value;
}

void TrialV::CreateValue() {
	DWORD count = 1;
	value = 1;
	RegCreateKeyEx(HKEY_CURRENT_USER, PathKey, 0, NULL, REG_OPTION_VOLATILE,
		KEY_WRITE, NULL, &hKey, NULL);
	RegSetValueEx(hKey, NameKey, 0, REG_DWORD, (const BYTE*)&count,
		sizeof(count));
}