#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <ctime>
using namespace std;
class ActiveProcess {
private:
	int day;
	int month;
	int year;
	int FirstTime;
	int SecondTime;
	struct AllProc {
		string NameProccess;
		string PathProc;
		long FirstTime;
		long SecondTime;
		long timers;
		AllProc *next;
	} *head;

	string ProcName;
	fstream write;

public:
	string check;
	int count;

	void SetProc(string name, string name2);
	ActiveProcess();
	~ActiveProcess();
	void WriteFile();
	void WriteAllFile();
	void WriteTempFile();
	string GetProc();
};