// Bomb_your_Screen.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Bomb_your_Screen.h"
#include "Bomb_Your_Screen_Logic.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


/************************************************************************/
/* 测试函数                                                                     */
/************************************************************************/
void* TestFun(void* args)
{
	int a = 0;
	return 0;
}



/************************************************************************/
/* 全局变量                                                             */
/************************************************************************/
//自定义的类
Bomb_Your_Screen_Logic* bomb_your_screen = NULL;
//键盘钩子的全局变量
HINSTANCE g_Instance;   // Handler of current instance  
HHOOK     g_Hook;       // Handler of hook 
BOOL SetHook();  
BOOL UnSetHook();
// 键盘钩子的回调
static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);  



/************************************************************************/
/* 函数功能：输出调试信息                                               */
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
/* 函数功能：键盘钩子的回调函数，处理键盘的消息                         */
/************************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)  
{  
	KBDLLHOOKSTRUCT* pkh = (KBDLLHOOKSTRUCT*)lParam;  
	if (GetAsyncKeyState(VK_CONTROL)&&GetAsyncKeyState(VK_MENU)&&GetAsyncKeyState(VK_DELETE))
	{
		int a = 0;
	}


	//HC_ACTION: wParam 和lParam参数包含了键盘按键消息  
	if (nCode == HC_ACTION)   
	{ 
		
		if (::GetAsyncKeyState('1') & 0x8000)  
		{  
			odprintf("111111111111111111");  
		}  
		//判断函数调用时指定虚拟键的状态  
		//BOOL bCtrlKey =   ::GetAsyncKeyState(VK_CONTROL) & 0x8000;
		BOOL bCtrlKey = ::GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1); 		
		 



		
		if ((pkh->vkCode == VK_ESCAPE && bCtrlKey) ||                    // Ctrl+Esc 开始菜单  
			(pkh->vkCode == VK_TAB    && pkh->flags & LLKHF_ALTDOWN) ||       // Alt+TAB 屏幕切换菜单  
			(pkh->vkCode == VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN) ||       // Alt+Esc 屏幕切换   
			(pkh->vkCode == VK_LWIN   || pkh->vkCode==VK_RWIN )||             // 左右Windows键  
			 (GetAsyncKeyState('.') & 0x8000 && pkh->flags &LLKHF_ALTDOWN && bCtrlKey)                
			)     
		{         
			odprintf("ok...i'am come here!");       //这里会出现两次, 因为  
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
/* 函数功能：取消钩子                                                   */
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
 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOMB_YOUR_SCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOMB_YOUR_SCREEN));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中
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
   //设置一个子线程，用来监视是否开启任务管理器
   bomb_your_screen->TaskManagerCtrlThreadCreate();
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择:
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
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		//获取鼠标位置
		tagPOINT pt_mouse;
		GetCursorPos(&pt_mouse);
		//鼠标坐标转换到屏幕坐标
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

// “关于”框的消息处理程序。
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
