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
/* �������ܣ�Ԥ����ͼƬ                                                 */
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
/*�������ܣ��Ե�ǰ��Ļ���������Ƶ�����DC,�������ڷŴ�ȫ��             */
/************************************************************************/
void Bomb_Your_Screen_Logic::CatchScreen()
{
	// ��Ļ��DC  
	HDC hdcScreen = GetDC(NULL);  
	// �����ڵ�DC  
	HDC hdcWindow = GetDC(m_hwnd);  
	// ��Ļ�Ŀ��  
	int scrWidth = GetSystemMetrics(SM_CXSCREEN);  
	// ��Ļ�ĸ߶�  
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(m_hwnd,0,0,scrWidth,scrHeight,FALSE);
	// ���ڵĿͻ�����  
	RECT rectClient;  
	GetClientRect(m_hwnd, &rectClient);  
	// ʹ��StretchBlt���и���  
	StretchBlt(hdcWindow,0,0,rectClient.right,rectClient.bottom,hdcScreen,0,0,scrWidth,scrHeight,SRCCOPY);  
	// �ͷ�DC  
	ReleaseDC(NULL, hdcScreen);  
	ReleaseDC(m_hwnd, hdcWindow);
}
/************************************************************************/
/* �������ܣ������������Ʊ�ը��ͼ��                                     */
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
	
	//��ʵ�ʻ���λ��
	tagPOINT ptPos;
	ptPos.x = pt_mouse.x - bmp_info.bmWidth/2;
	ptPos.y = pt_mouse.y - bmp_info.bmHeight/2;
	TransparentBlt(hdcWindow,ptPos.x,ptPos.y,bmp_info.bmWidth,bmp_info.bmHeight,hdc_mem,0,0,bmp_info.bmWidth,bmp_info.bmHeight,RGB(255,255,255));
	DeleteObject(hdc_mem);
	//DeleteObject(hbitmap_bomb);
}

/************************************************************************/
/* �������ܣ�����������������ڲ�����ر�                               */
/************************************************************************/
void* Bomb_Your_Screen_Logic::FindAndCloseTaskManagerWindow(void* input)
{
	POINT pos = {0};
	RECT TaskWindowRect = {0};
	while(true)
	{
		HWND hwnd_TaskManager = FindWindowW(L"TaskManagerWindow",L"���������");	
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
/* �������ܣ���������������������߳�                                   */
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