#pragma once
#include <Windows.h>
class TrialV {
private:
	HKEY hKey;
	bool check;
	DWORD value;
public:
	bool Check();
	TrialV();
	~TrialV();
	void SetValue(DWORD count);
	DWORD GetValue();
	void CreateValue();
	void AutoRun();

};