#pragma once
#include <assert.h>
#include <mmsystem.h>

#include "resource.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

//定义窗口尺寸
#define WNDWIDTH	395
#define WNDHEIGHT	595

//bmp数量
#define BMPCOUNT	  8


//声明结构体
typedef enum {
	WELCOME,
	RUNING,
	OVER,
}GAMESTATUS;//游戏所处状态

typedef struct {
	GAMESTATUS	g_status;
	int			score;
}GAME;

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HBITMAP hbBmp[BMPCOUNT];						// 储存位图ID的数组
int uiBmpName[BMPCOUNT] = {						// 储存位图句柄的数组
	IDB_BackColor,
	IDB_BIG,
	IDB_EXIT,
	IDB_GAMEOVER,
	IDB_LOGO,
	IDB_MIDDLE,
	IDB_RESTART,
	IDB_START,
};
GAME s_game;
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void				drawGameUI(HDC);


