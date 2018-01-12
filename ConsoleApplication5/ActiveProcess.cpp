#include "stdafx.h"
#include "ActiveProcess.h"
#include <string>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include <ctime>
using namespace std;

ActiveProcess::ActiveProcess() {
	head = NULL;
	check = "";
	count = 0;
	time_t t;
	t = time(NULL);
	SYSTEMTIME st;
	GetSystemTime(&st);
	day = st.wDay;
	month = st.wMonth;
	year = st.wYear;
	FirstTime = t;
}
void ActiveProcess::WriteAllFile() {
	AllProc *tmp = head;
	time_t t;
	t = time(NULL);
	SecondTime = t;
	write.open("AllData.txt", std::ios::app);
	write << day << "$" << month << "$" << year << "$";
	while (tmp) {
		string s = tmp->PathProc;
		if (s[0] != 77 && s[3] != 77 && s[5] != 77 && tmp->NameProccess != "")
			write << tmp->NameProccess << "$" << tmp->timers << "$" << tmp->PathProc
			<< "$";
		tmp = tmp->next;
	}
	write << (SecondTime - FirstTime) << endl;
	write.close();
}
string ActiveProcess::GetProc() {
	DWORD pid;
	HWND hwnd = GetForegroundWindow();
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE h =
		OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	char name[MAX_PATH];
	DWORD value = MAX_PATH;
	char str[MAX_PATH];
	wchar_t buffer[MAX_PATH];
	QueryFullProcessImageNameA(h, 0, name, &value);
	GetWindowTextA(hwnd, str, 255);
	string s = (string)str;

	CloseHandle(h);
	ProcName = (string)name;
	SetProc(s, ProcName);

	return ProcName;
}

void ActiveProcess::WriteTempFile() {
	AllProc *tmp = head;
	ofstream write("C:\\Users\\Dmitryi\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication5\\Debug\\test.txt");
	//ofstream write("test.txt");

	while (tmp) {
		string s = tmp->PathProc;
		string s2 = tmp->NameProccess;
		if (s[0] != 77 && s[3] != 77 && s[5] != 77 && tmp->NameProccess != "")
			write << tmp->PathProc << "$" << tmp->NameProccess << "$" << tmp->timers
			<< "$";
		tmp = tmp->next;
	}

	write.close();
}

void ActiveProcess::SetProc(string n, string p) {
	count++;
	bool e = false;
	AllProc *tmp = head;
	AllProc *tmpr = head;
	time_t t;
	bool b = false;
	bool flag = true;
	t = time(NULL);

	if (tmpr) {
		while (tmpr->NameProccess != check) {
			tmpr = tmpr->next;
		}
		tmpr->SecondTime = t;
		tmpr->timers = tmpr->timers + (tmpr->SecondTime - tmpr->FirstTime);
	}
	while (tmp) {
		if (tmp->NameProccess == n) {
			tmp->FirstTime = t;
			flag = false;
			check = n;
			break;

		}
		else {
			tmp = tmp->next;
		}
	}

	if (!head || flag) {
		AllProc *pd = new AllProc();
		pd->NameProccess = n;
		pd->PathProc = p;
		pd->FirstTime = t;
		pd->SecondTime = 0;
		pd->timers = 0;
		pd->next = head;
		head = pd;
		check = pd->NameProccess;
	}
}

ActiveProcess::~ActiveProcess() {
	while (head != NULL) {
		AllProc *tmp = head->next;
		delete head;
		head = tmp;
	}
}
