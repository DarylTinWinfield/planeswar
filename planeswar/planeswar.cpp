//planeswar.cpp: 定义应用程序的入口点。
//
#include "stdafx.h"
#include "planeswar.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PLANESWAR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PLANESWAR));

    MSG msg;

    // 主消息循环: 
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

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BIGICON));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MYCURSOR));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // 将实例句柄存储在全局变量中
   int cxScreen = GetSystemMetrics(SM_CXSCREEN);
   int cyScreen = GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowW(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX,
       (cxScreen - WNDWIDTH) / 2, 
	   (cyScreen - WNDHEIGHT - 50) / 2, 
	   WNDWIDTH, 
	   WNDHEIGHT, 
	   nullptr, 
	   nullptr, 
	   hInstance, 
	   nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		for (int i = 0; i < BMPCOUNT; i++) {
			hbBmp[i] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(uiBmpName[i]));
		}
		s_game.g_status = WELCOME;
		s_game.score = 0;
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			drawGameUI(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void drawGameUI(HDC hdc) {
	HBITMAP hBmpMem;
	HDC hDcMem,hDcTmp;
	BITMAP bmp;

	/* 双缓冲法绘图，解决重绘时闪烁的问题 */
	/* 创建与hDC环境相关的设备兼容的位图 */
	hBmpMem = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	/* 创建与hdc相兼容的内存dc */
	hDcMem = CreateCompatibleDC(hdc);
	SelectObject(hDcMem, hBmpMem);
	/* hDcTmp是另一个临时的内存dc，用来储存部件，如背景，飞机，按钮等等 */
	hDcTmp = CreateCompatibleDC(hdc);

	//画出背景图
	SelectObject(hDcTmp, hbBmp[0]);
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT,
		hDcTmp, 0, 0, SRCCOPY);

	switch (s_game.g_status) {
	case WELCOME:
		//画logo
		SelectObject(hDcTmp, hbBmp[4]);
		GetObject(hbBmp[4], sizeof(BITMAP), &bmp);
		TransparentBlt(hdc, 35, 150, bmp.bmWidth, bmp.bmHeight, 
			hDcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255,255,255));
		//画button
		SelectObject(hDcTmp, hbBmp[7]);
		GetObject(hbBmp[7], sizeof(BITMAP), &bmp);
		TransparentBlt(hdc, 125, 380, bmp.bmWidth, bmp.bmHeight,
			hDcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		break;
	}
}
