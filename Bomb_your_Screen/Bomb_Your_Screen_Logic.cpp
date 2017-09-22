#include "StdAfx.h"
#include "Bomb_Your_Screen_Logic.h"

#pragma comment(lib,"msimg32.lib")


Bomb_Your_Screen_Logic::Bomb_Your_Screen_Logic(HWND hwnd)
{
	m_hwnd = hwnd;
	preloadImageBomb();
}


Bomb_Your_Screen_Logic::~Bomb_Your_Screen_Logic(void)
{
}

/************************************************************************/
/* 函数功能：预加载图片                                                 */
/************************************************************************/
void Bomb_Your_Screen_Logic::preloadImageBomb()
{
	WCHAR filename[100];
	for (int i = 0; i < 4; i++)
	{
		wsprintf(filename,L"..\\Image\\heart%d.bmp",i+1);
		HBITMAP temp_bitmap = (HBITMAP)LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		Image_Buffer_Bomb.push_back(temp_bitmap);
	}	
}


/************************************************************************/
/*函数功能：对当前屏幕截屏并绘制到窗口DC,并将窗口放大到全屏             */
/************************************************************************/
void Bomb_Your_Screen_Logic::CatchScreen()
{
	// 屏幕的DC  
	HDC hdcScreen = GetDC(NULL);  
	// 本窗口的DC  
	HDC hdcWindow = GetDC(m_hwnd);  
	// 屏幕的宽度  
	int scrWidth = GetSystemMetrics(SM_CXSCREEN);  
	// 屏幕的高度  
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(m_hwnd,0,0,scrWidth,scrHeight,FALSE);
	// 窗口的客户区域  
	RECT rectClient;  
	GetClientRect(m_hwnd, &rectClient);  
	// 使用StretchBlt进行复制  
	StretchBlt(hdcWindow,0,0,rectClient.right,rectClient.bottom,hdcScreen,0,0,scrWidth,scrHeight,SRCCOPY);  
	// 释放DC  
	ReleaseDC(NULL, hdcScreen);  
	ReleaseDC(m_hwnd, hdcWindow);
}
/************************************************************************/
/* 函数功能：根据索引绘制爆炸的图像                                     */
/************************************************************************/
void Bomb_Your_Screen_Logic::BombScreen(int picture_index,tagPOINT pt_mouse)
{
	HDC hdcWindow = GetDC(m_hwnd);
	HDC hdc_mem = CreateCompatibleDC(hdcWindow);
	RECT rect_window;
	GetClientRect(m_hwnd,&rect_window);
	int width_window = rect_window.right - rect_window.left;
	int height_window = rect_window.bottom - rect_window.top;
	HBITMAP hbitmap_bomb = Image_Buffer_Bomb[picture_index];
	/*
	WCHAR filename[100];
	wsprintf(filename,L"..\\Image\\bomb%d.bmp",picture_index+1);
	HBITMAP hbitmap_bomb =  (HBITMAP)LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	*/
	SelectObject(hdc_mem,hbitmap_bomb);
	BITMAP bmp_info;
	GetObject(hbitmap_bomb,sizeof(BITMAP),&bmp_info);
	
	//求实际绘制位置
	tagPOINT ptPos;
	ptPos.x = pt_mouse.x - bmp_info.bmWidth/2;
	ptPos.y = pt_mouse.y - bmp_info.bmHeight/2;
	TransparentBlt(hdcWindow,ptPos.x,ptPos.y,bmp_info.bmWidth,bmp_info.bmHeight,hdc_mem,0,0,bmp_info.bmWidth,bmp_info.bmHeight,RGB(255,255,255));
	DeleteObject(hdc_mem);
	//DeleteObject(hbitmap_bomb);
}

/************************************************************************/
/* 函数功能，发现任务管理器窗口并将其关闭                               */
/************************************************************************/
void* Bomb_Your_Screen_Logic::FindAndCloseTaskManagerWindow(void* input)
{
	POINT pos = {0};
	RECT TaskWindowRect = {0};
	while(true)
	{
		HWND hwnd_TaskManager = FindWindowW(L"TaskManagerWindow",L"任务管理器");	
		GetCursorPos(&pos);
		GetWindowRect(hwnd_TaskManager,&TaskWindowRect);		
		if(pos.x > TaskWindowRect.left && pos.x < TaskWindowRect.right)
			if(pos.y > TaskWindowRect.top && pos.y < TaskWindowRect.bottom)
			{
				MoveWindow(hwnd_TaskManager,pos.x,pos.y,600,600,true);
				//CloseWindow(hwnd_TaskManager);				
			};

	};
	return 0;
}

/************************************************************************/
/* 函数功能，开启监视任务管理器的线程                                   */
/************************************************************************/
int Bomb_Your_Screen_Logic::TaskManagerCtrlThreadCreate()
{
	pthread_t TaskManagerCtrlThread;

	if(pthread_create(&TaskManagerCtrlThread,NULL,FindAndCloseTaskManagerWindow,(void*)this))
	{
		return -1;
	}
	return 0;
}