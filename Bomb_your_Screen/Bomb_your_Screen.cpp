// Bomb_your_Screen.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Bomb_your_Screen.h"
#include "Bomb_Your_Screen_Logic.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


/************************************************************************/
/* ���Ժ���                                                                     */
/************************************************************************/
void* TestFun(void* args)
{
	int a = 0;
	return 0;
}



/************************************************************************/
/* ȫ�ֱ���                                                             */
/************************************************************************/
//�Զ������
Bomb_Your_Screen_Logic* bomb_your_screen = NULL;
//���̹��ӵ�ȫ�ֱ���
HINSTANCE g_Instance;   // Handler of current instance  
HHOOK     g_Hook;       // Handler of hook 
BOOL SetHook();  
BOOL UnSetHook();
// ���̹��ӵĻص�
static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);  



/************************************************************************/
/* �������ܣ����������Ϣ                                               */
/************************************************************************/ 
void __cdecl odprintf(const char* fmt, ...)  
{  
	char buf[4096], *p = buf;  
	va_list args;
	va_start(args, fmt);  
	p += vsnprintf_s(p, sizeof(buf), _TRUNCATE, fmt, args);  
	va_end(args); 
	while ( p > buf  &&  isspace(p[-1]) )  
		*--p = '\0';  
	*p++ = '\r';  
	*p++ = '\n';  
	*p   = '\0'; 
	OutputDebugStringA(buf);    //output as ANSI string //OutputDebugString  
}  
/************************************************************************/
/* �������ܣ����̹��ӵĻص�������������̵���Ϣ                         */
/************************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)  
{  
	KBDLLHOOKSTRUCT* pkh = (KBDLLHOOKSTRUCT*)lParam;  
	if (GetAsyncKeyState(VK_CONTROL)&&GetAsyncKeyState(VK_MENU)&&GetAsyncKeyState(VK_DELETE))
	{
		int a = 0;
	}


	//HC_ACTION: wParam ��lParam���������˼��̰�����Ϣ  
	if (nCode == HC_ACTION)   
	{ 
		
		if (::GetAsyncKeyState('1') & 0x8000)  
		{  
			odprintf("111111111111111111");  
		}  
		//�жϺ�������ʱָ���������״̬  
		//BOOL bCtrlKey =   ::GetAsyncKeyState(VK_CONTROL) & 0x8000;
		BOOL bCtrlKey = ::GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1); 		
		 



		
		if ((pkh->vkCode == VK_ESCAPE && bCtrlKey) ||                    // Ctrl+Esc ��ʼ�˵�  
			(pkh->vkCode == VK_TAB    && pkh->flags & LLKHF_ALTDOWN) ||       // Alt+TAB ��Ļ�л��˵�  
			(pkh->vkCode == VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN) ||       // Alt+Esc ��Ļ�л�   
			(pkh->vkCode == VK_LWIN   || pkh->vkCode==VK_RWIN )||             // ����Windows��  
			 (GetAsyncKeyState('.') & 0x8000 && pkh->flags &LLKHF_ALTDOWN && bCtrlKey)                
			)     
		{         
			odprintf("ok...i'am come here!");       //������������, ��Ϊ  
			return 1;   
		}  
		

	}  

	//return 1;
	// Call next hook in chain  
	return ::CallNextHookEx(g_Hook, nCode, wParam, lParam);  
}  


BOOL SetHook()  
{  
	if (g_Instance || g_Hook)       // Already hooked!  
		return TRUE;  


	g_Instance = GetModuleHandle(NULL);  
	g_Hook     = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, g_Instance, 0);  
	if (!g_Hook)  
	{  
		odprintf("SetHook error, error code=%ld", ::GetLastError());    //error code  
		return FALSE;  
	}  


	return TRUE;                                // Hook has been created correctly  
}  

/************************************************************************/
/* �������ܣ�ȡ������                                                   */
/************************************************************************/
BOOL UnSetHook()  
{  
	if (g_Hook) {                               // Check if hook handler is valid  
		::UnhookWindowsHookEx(g_Hook);          // Unhook is done here  
		g_Hook = NULL;                          // Remove hook handler to avoid to use it again  
	}  


	return TRUE;                                // Hook has been removed  
}  







int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	odprintf("Test of KeyboardProc, starting...");  


	if (!SetHook())  
		return -1;  
 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOMB_YOUR_SCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOMB_YOUR_SCREEN));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	UnSetHook();  
	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOMB_YOUR_SCREEN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOMB_YOUR_SCREEN);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
   UINT widthOfdesk = GetSystemMetrics(SM_CXSCREEN);
   UINT heightOfdesk = GetSystemMetrics(SM_CYSCREEN);
   hWnd = CreateWindow(szWindowClass, szTitle,WS_POPUP,
      100,100, 0, 0, NULL, NULL, hInstance, NULL);
   ShowWindow(hWnd,SW_HIDE);
   ::SetMenu(hWnd,NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd); 
   bomb_your_screen = new Bomb_Your_Screen_Logic(hWnd);
   bomb_your_screen->CatchScreen();
   //����һ�����̣߳����������Ƿ������������
   bomb_your_screen->TaskManagerCtrlThreadCreate();
   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		//��ȡ���λ��
		tagPOINT pt_mouse;
		GetCursorPos(&pt_mouse);
		//�������ת������Ļ����
		ScreenToClient(hWnd,&pt_mouse);
		for (int i = 0; i < 4; i++)
		{
			bomb_your_screen->BombScreen(i,pt_mouse);
			Sleep(40);
		}		
		break;
	case WM_RBUTTONUP:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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
