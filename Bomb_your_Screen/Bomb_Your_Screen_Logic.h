#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <pthread.h>
using namespace std;
class Bomb_Your_Screen_Logic
{
public:
	Bomb_Your_Screen_Logic(HWND hwnd):m_hwnd(hwnd){};
	virtual ~Bomb_Your_Screen_Logic(void);
	void CatchScreen();
	void BombScreen(int picture_index,tagPOINT pt_mouse);
	void preloadImageBomb();
	static void* FindAndCloseTaskManagerWindow(void* input);
	int TaskManagerCtrlThreadCreate();
private:	
	HWND m_hwnd;  //主窗口DC
	vector<HBITMAP> Image_Buffer_Bomb; //预加载图片缓存区
};

