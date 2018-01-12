// ConsoleApplication5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <psapi.h>
#include <fstream>
#include <string>
#include <time.h>
#include "ActiveProcess.h"
#include "resource1.h"
#include "TrialV.h"
using namespace std;
NOTIFYICONDATA nid;
HWND h1;
ActiveProcess *ap = new ActiveProcess();
HANDLE ev1;
HWND hWnd_button;
NOTIFYICONDATA iconData;
HANDLE ev2;
BOOL CreateNotifyIcon(HWND, UINT, UINT);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
const UINT CallbackMsg = WM_USER + 1;
const UINT iconID = 1;

int Win(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine,
	int nCommandShow, int count) {
	wchar_t buf1[256];
	wchar_t buf2[256];
	wsprintfW(buf1, L"%d", 30 - count);
	wsprintfW(buf2, L"Это триал версия приложения\nУ вас осталось запусков: ");
	wchar_t *wszNewStr =
		new wchar_t[(sizeof(buf1) + sizeof(buf2)) / sizeof(wchar_t)];
	wcscpy(wszNewStr, wcscat(buf2, buf1));
	int result = MessageBox(NULL, wszNewStr, L"Trial Version", MB_OK);

	switch (result) {
	case IDOK:

		break;
	}
	return NULL;
}

int MainTray(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,
	int nCmdShow) {
	ev1 = CreateEvent(NULL, TRUE, FALSE, L"ev1");

	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	wchar_t szClassName[] = L"MyClassName";
	WNDCLASS WndClass;
	ZeroMemory(&WndClass, sizeof(WndClass));
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon =
		::LoadIcon(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_ICON2));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WindowProc;
	WndClass.lpszClassName = szClassName;

	ATOM atom = RegisterClass(&WndClass);
	HWND hWndMain =
		CreateWindow(szClassName, L"CheckTime", WS_OVERLAPPEDWINDOW, x - 300,
			y - 130, 300, 90, NULL, NULL, hInstance, 0);

	hWnd_button = CreateWindow(L"button", L"Запустить",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 80,
		30, hWndMain, (HMENU)10000, hInstance, NULL);
	CreateNotifyIcon(hWndMain, iconID, CallbackMsg);

	UpdateWindow(hWndMain);
	ShowWindow(hWndMain, SW_SHOW);

	MSG Msg;
	while (GetMessage(&Msg, 0, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	CreateNotifyIcon(hWndMain, iconID, 0);
	return FALSE;
}

BOOL CreateNotifyIcon(HWND hWnd, UINT uID, UINT uCallbackMessage) {
	ZeroMemory(&iconData, sizeof(iconData));
	iconData.cbSize = sizeof(iconData);
	iconData.uID = uID;
	iconData.hIcon =
		::LoadIcon(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_ICON2));
	iconData.hWnd = hWnd;
	DWORD dwMessage;
	if (uCallbackMessage) {
		lstrcpyn(iconData.szTip, L"CheckTime",
			sizeof(iconData.szTip) / sizeof(iconData.szTip[0]));
		dwMessage = NIM_ADD;
		iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		iconData.uCallbackMessage = uCallbackMessage;

	}
	else
		dwMessage = NIM_DELETE;

	return Shell_NotifyIcon(dwMessage, &iconData);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;
	switch (uMsg) {
	case CallbackMsg:
		if (wParam == iconID) {
			switch (lParam) {
			case WM_LBUTTONDOWN:
				ShowWindow(hWnd, SW_SHOW);
				ShowWindow(hWnd, SW_RESTORE);
				Shell_NotifyIcon(NIM_DELETE, &iconData);
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		switch (wParam) {
		case SIZE_MINIMIZED:
			ShowWindow(hWnd, SW_HIDE);
			Shell_NotifyIcon(NIM_ADD, &iconData);
			break;
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 10000) {
			SetEvent(ev1);
			SetWindowText(hWnd_button, L"Запущено");
		}
		break;
	case WM_CLOSE:
		ap->WriteAllFile();
		Shell_NotifyIcon(NIM_DELETE, &iconData);
		exit(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void FuncTexit(ActiveProcess *ap) {
	HANDLE h = CreateEvent(NULL, FALSE, FALSE, L"1");
	HANDLE h1 = CreateEvent(NULL, FALSE, FALSE, L"2");
	while (true) {
		WaitForSingleObject(h1, INFINITE);
		ap->WriteTempFile();
		SetEvent(h);
	}
}

int _tmain(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine,
	int nCommandShow) {
	ShowWindow(GetForegroundWindow(), SW_HIDE);
	TrialV *trial = new TrialV();
	trial->AutoRun();
	if (trial->Check()) {
		Win(hInst, hPreviousInst, lpCommandLine, nCommandShow, trial->GetValue());
	}
	else {
		MessageBox(NULL, L"Извините, количество запусков исчерпано",
			L"Пробная версия закончилась", MB_OK);
		exit(0);
	}

	ev2 = CreateEvent(NULL, FALSE, FALSE, L"ev2");

	setlocale(LC_ALL, "");

	SetConsoleTitle(L"CheckTime");
	h1 = GetForegroundWindow();
	HANDLE h =
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainTray, NULL, 0, NULL);


	HANDLE h2 =
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FuncTexit, ap, 0, NULL);

	setlocale(LC_ALL, "");
	int i = 0;
	Sleep(10);
	HANDLE e = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"ev1");
	WaitForSingleObject(e, INFINITE);
	while (true) {
		ap->GetProc();
		Sleep(1000);
	}

	return 0;
}

