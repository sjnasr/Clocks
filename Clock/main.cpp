//create a simple clock first
//Start with displaying the time
//Figure out how to display without flickering

#include <time.h> //needed for C++ time
#include <iostream> //needed for any input and output streams
#include <Windows.h> //needed for win32 calls
#include <tchar.h> //has the TChar macro
#include "resource1.h" //all IDs found here
#include "Clock.h"
#include "Configuration.h"
#include "Logging.h"

using namespace std;

//Global variables
HINSTANCE hInst;
static LPCTSTR lpszAppName = "Digital Clock";
ClockClass clocking;
Configuration config;
string saveStart, saveStop; //save and freeze the start and stop time global.

//Rules to trigger events
bool startTimer = false;
bool stopTimer = false;
bool loggingEnabled = false;
bool clearLap = false;
bool enableMilliesecond = false;

//Forward declarations
INT_PTR CALLBACK About(HWND hDlg, UINT uMsg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);

//
//PURPOSE: Registers the window class
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	//Contains information about the window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = lpszAppName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	return RegisterClassEx(&wcex);
}

//
//PURPOSE: Creates main window
//
BOOL initWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	//Create the main application window
	hWnd = CreateWindow(
		lpszAppName, //the name of the application
		"Digital Clock", //Text for window title bar
		WS_OVERLAPPEDWINDOW|WS_CAPTION|WS_SYSMENU|WS_MINIMIZE, //Window style...look more into window style when I have time otherwise use default. Don't get caught up in UI
		0, 0, //Use default positioning
		800, 600, //(width, length)
		NULL, //the parent of this window, Overlapped windows have no parent
		NULL, //this application does not have a menu bar, Use the windows class menu but look more into that
		hInstance, //This instance owns this window, the first parameter from WinMai
		NULL
	);

	if (!hWnd)
	{
		return FALSE;
	}

	//Show the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//PURPOSE: Processes messages for the main window
//
// WM_COMMAND - process the application menu
// WM_PAINT   - Paint the main window
// WM_Destroy - kill the window
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HMENU hMenu, hSubMenu, hAboutMenu, hEditMenu; //Menu variables
	string saveStartTime, saveStopTime; //string to grab the start time
	RECT startTimeRect, stopTimeRect, printLoggingMsgRect, clearing, loggingRateMsg, OverOrAppend; //Rectangle for displaying start time, stop time, and clearing both these fields.
	//Print logging message currently doesn't work FIXME: Print if logging is enabled or not
	string millisecondEnable, loggingEnable;
	int error;
	HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); //White brush

	//store what is in the global
	HINSTANCE hInstance = hInst;

	switch (uMsg) {
	case WM_CREATE:
		//Button to enable Millisecond
		CreateWindow(
			"button",
			"Milliseconds",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			110,
			125,
			100,
			30,
			hwnd,
			(HMENU)ID_MILLISECONDS,
			hInst,
			0);
		//Button to record start time
		CreateWindow(
			"button",
			"Start",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			215,
			90,
			50,
			30,
			hwnd,
			(HMENU)ID_START,
			hInst,
			0);

		//Button to record stop time
		CreateWindow(
			"button",
			"Stop",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			270,
			90,
			50,
			30,
			hwnd,
			(HMENU)ID_STOP,
			hInst,
			0);
		//Button to clear start and stop time
		CreateWindow(
			"button",
			"Clear",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			238,
			125,
			50,
			30,
			hwnd,
			(HMENU)ID_CLEARLAP,
			hInst,
			0);
		//Button to enable/disable logging
		//Currently doesn't display if logging is enabled or disabled
		CreateWindow(
			"button",
			"Logging",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			325,
			125,
			100,
			30,
			hwnd,
			(HMENU)ID_ELOGGING,
			hInst,
			0);
		//Dropdown to select logging rate
		CreateWindow(
			"ComboBox",
			"Logging Rate",
			WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,
			430,
			125,
			150,
			200,
			hwnd,
			(HMENU)ID_COMBOBOX,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);

		//change this to set default value from configuration file
		SendMessage(GetDlgItem(hwnd, ID_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)"3 Minutes");
		SendMessage(GetDlgItem(hwnd, ID_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)"5 Minutes");
		//SendMessage(GetDlgItem(hwnd, ID_COMBOBOX), CB_SETCURSEL, 0, 0); //Sets first value
		SendMessage(GetDlgItem(hwnd, ID_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)"7 Minutes");


		CreateWindow(
			"ComboBox",
			"Overwrite Or Append",
			WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,
			430,
			200,
			150,
			200,
			hwnd,
			(HMENU)ID_BOXAOO,
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);

		SendMessage(GetDlgItem(hwnd, ID_BOXAOO), CB_ADDSTRING, 0, (LPARAM)"Overwrite");
		SendMessage(GetDlgItem(hwnd, ID_BOXAOO), CB_ADDSTRING, 0, (LPARAM)"Append");


		hMenu = CreateMenu();
		hSubMenu = CreatePopupMenu();
		hAboutMenu = CreatePopupMenu();
		hEditMenu = CreatePopupMenu();

		//File Menu
		AppendMenu(hSubMenu, MF_STRING, IDM_EXIT, "E&xit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

		//About Menu
		AppendMenu(hAboutMenu, MF_STRING, ID_ABOUT, "A&bout");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hAboutMenu, "&Help");
		SetMenu(hwnd, hMenu);

		if (config.getMillisecondBool() == true)
		{
			SetTimer(hwnd, ID_TIMER, 1, NULL);
		}
		else
		{
			SetTimer(hwnd, ID_TIMER, 1000, NULL); //sets timer for every second
		}

		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
			case IDM_EXIT: //Exit called
				DestroyWindow(hwnd);
				break;
			case ID_ABOUT: //About is called
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, About);
				break;

			case ID_MILLISECONDS:
				millisecondEnable = "millisecondEnable";
				config.setConfig(millisecondEnable);
				config.readConfig();
				if (enableMilliesecond == false)
				{
					enableMilliesecond = true;
					SetTimer(hwnd, ID_TIMER, 1, NULL);
				}
				else if(enableMilliesecond == true)
				{
					enableMilliesecond = false;
					InvalidateRect(hwnd, NULL, TRUE);
					SetTimer(hwnd, ID_TIMER, 1000, NULL); //sets timer for every second
				}
				break;
			case ID_START:
				startTimer = true;

				if (stopTimer)
				{
					stopTimer = false;

				}
				clearLap = false;
				InvalidateRect(hwnd,  NULL, FALSE);
				break;
			case ID_STOP:
				if (startTimer)
				{
					startTimer = false;

				}
				stopTimer = true;
				clearLap = false;
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			case ID_ELOGGING:
				loggingEnable = "loggingEnable";
				
				config.readConfig();
				config.setConfig(loggingEnable);
				config.readConfig();
				break;

			case ID_COMBOBOX:
				if (HIWORD(wparam) == CBN_SELCHANGE)
				{
					char selected[255] = { 0 };
					int index = SendMessage((HWND)lparam, CB_GETCURSEL, 0, 0);
					SendMessage((HWND) lparam, CB_GETLBTEXT, index, (LPARAM) selected);
					MessageBox(0, selected, "Information", MB_OK | MB_SYSTEMMODAL);

					config.setLoggingOptions(selected);
					cout << endl;
					
				}
				break;
			case ID_CLEARLAP:
				clearLap = true;
				InvalidateRect(hwnd, NULL, TRUE); //clear the text
				break;
			case ID_BOXAOO:
				if (HIWORD(wparam) == CBN_SELCHANGE)
				{
					char selected[255] = { 0 };
					int index = SendMessage((HWND) lparam, CB_GETCURSEL, 0, 0);
					SendMessage((HWND)lparam, CB_GETLBTEXT, index, (LPARAM)selected);
					MessageBox(0, selected, "Information", MB_OK | MB_SYSTEMMODAL);

					config.setLoggingOptions(selected);
				}
				break;
			default:
				return DefWindowProc(hwnd, uMsg, wparam, lparam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//Display date and time
		clocking.DisplayTime(hdc);
		clocking.DisplayDate(hdc);
		
		//Switch to drawtext and create a rectangle for this
		//Stopwatch like feature, to record start and stop time
		if (startTimer == true && stopTimer == false && clearLap == false)
		{
			SetRect(&startTimeRect, 115, 175,115, 175);
			//the time is still movign which screws up duration
			//figure out how to get static time
			saveStartTime = "Start Time: " + saveStart + " Date: " + clocking.getDate();
			saveStart = saveStartTime;
			DrawText(hdc, saveStartTime.c_str(), -1, &startTimeRect, DT_LEFT | DT_NOCLIP);
		}
		else if (stopTimer == true && startTimer == false && clearLap == false)
		{
			SetRect(&stopTimeRect, 115, 200, 115, 175);
			saveStopTime = "Stop Time: " + saveStop + " Date: " + clocking.getDate();
			saveStop = saveStopTime;
			DrawText(hdc, saveStopTime.c_str(), -1, &stopTimeRect, DT_LEFT | DT_NOCLIP);
			//clocking.differenceBetweenStartAndStop(hdc, saveStart, saveStop);
		}

		SetRect(&loggingRateMsg, 430, 105, 150, 50);
		DrawText(hdc, "Logging Rate", -1, &loggingRateMsg, DT_LEFT | DT_NOCLIP);

		SetRect(&OverOrAppend, 430, 177, 150, 50);
		DrawText(hdc, "Overwrite or Append Log File", -1, &OverOrAppend, DT_LEFT | DT_NOCLIP);

		//SetRect(&loggingRateMsg, 430, 105, 150, 50);
		//DrawText(hdc, "Logging Rate", -1, &loggingRateMsg, DT_LEFT | DT_NOCLIP);
		EndPaint(hwnd, &ps);
		if (config.getLoggingValue() == true)
		{
			LoggingEnabled logEnable;

			//write to log
			logEnable.writeToLog(clocking.getTime(), clocking.getDate());
		}
		break;
	case WM_TIMER:
		InvalidateRect(hwnd, &clocking.getRect(), FALSE); //Update the Rectangle and DrawText, setTimer calls this every second, updates the whole window
		break;
	
		
	case WM_DESTROY:
		DeleteObject(whiteBrush);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DeleteObject(whiteBrush);
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wparam, lparam);
	}

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	//Initialize global variables
	hInst = hInstance;


	config.readConfig(); //read configuration
	if (config.getLoggingValue() == true)
	{
		LoggingEnabled logEnable; //check if logging is enabled
	}

	if (config.getMillisecondBool() == true)
	{
		enableMilliesecond = true;
	}

	if (!MyRegisterClass(hInstance)) // check
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	if (!initWindow(hInstance, nCmdShow)) {
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(ID_PROJECT));

	//Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}



INT_PTR CALLBACK About(HWND hDlg, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wparam) == IDOK || LOWORD(wparam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wparam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}