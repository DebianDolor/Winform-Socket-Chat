#include "stdafx.h"
#include "Server.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVER));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVER));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SERVER);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	  CW_USEDEFAULT, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
	  return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			char buf[4096] = {};
			if (!gServerObj.isConnected())
			{
				MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
				getch();
				return 1;
			}
			gServerObj.setHWND(hWnd);
			AfxBeginThread(listenServerThread, 0);
			LoadData();
			hTextBox = CreateWindow(L"edit", L"", WS_VISIBLE | WS_VSCROLL | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY, 0, 0, 480, 500, hWnd, 0, hInst, 0);
			break;
		}
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				switch (wmId)
				{
					case IDM_ABOUT:
						DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
						break;
					case IDM_EXIT:
						DestroyWindow(hWnd);
						break;
					case ID_RESTORE:
						RestoreData();
						break;
					case ID_USER_CONNECT:
					{
						WCHAR buffer[10000];
						GetWindowText(hTextBox, buffer, 10000);
						if (buffer == NULL)
							break;
					
						wcscat_s(buffer, L"\r\n");
						wcscat_s(buffer, L"1 user has just connected to server");
						SetWindowText(hTextBox, buffer);

						SendMessageA(hTextBox, EM_SETSEL, 0, -1);
						SendMessageA(hTextBox, EM_SETSEL, -1, -1);
						SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);

						break;
					}
					case ID_USER_LEAVE:
					{
						WCHAR buffer[10000];
						GetWindowText(hTextBox, buffer, 10000);
						if (buffer == NULL)
							break;
					
						wcscat_s(buffer, L"\r\n");
						wcscat_s(buffer, L"1 user has just disconnected to server");
						SetWindowText(hTextBox, buffer);

						SendMessageA(hTextBox, EM_SETSEL, 0, -1);
						SendMessageA(hTextBox, EM_SETSEL, -1, -1);
						SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);

						break;
					}
					default:
						return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			{
				RestoreData();
				PostQuitMessage(0);
				break;
			}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}


void RestoreData()
{
	std::wofstream fos(L"userdata.ini");
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	fos.imbue(loc);
	if (!fos.is_open())
		return;

	for (auto user : gServerObj.getUser())
	{
		fos << user->username << L";";
		fos << user->password << L"\n";
	}
	fos.close();
}

void LoadData()
{
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	std::wifstream fin(L"userdata.ini");
	fin.imbue(loc);
	WCHAR buffer[200];
	WCHAR username[100];
	WCHAR password[100];

	int i = 0;

	while (!fin.eof())
	{
		fin.getline(buffer, 200);
		if (buffer[0] == NULL)
			break;
		
		int len = (int)wcslen(buffer);
		int j, k;
		for (j = 0; j < len; j++)
		{
			if (buffer[j] == L';')
			{
				j++;
				break;
			}
			username[j] = buffer[j];
		}
		username[j - 1] = NULL;

		for (k = j; k < len; k++)
			password[k - j] = buffer[k];
		
		password[k - j] = NULL;
		auto user = new User;
		user->username = username;
		user->password = password;
		gServerObj.addUser(user);
	}

	fin.close();
}