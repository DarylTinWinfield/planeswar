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
	   WNDHEIGHT + 35, 
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
	static UINT timeCounter = 0;
    switch (message)
    {
	case WM_CREATE: {
		initgame(lParam);
	}
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		drawSenceUI(hdc);
		EndPaint(hWnd, &ps);
	}
        break;
	case WM_LBUTTONDOWN: {
		lButtondownProc(hWnd,lParam);
	}
		break;
	case WM_START: {
		SetTimer(hWnd, TIMER, 50, NULL);
		updatePlaneInfo(TRUE, 0);
		//InvalidateRect(hWnd, NULL, TRUE);
		SendMessage(hWnd, WM_PAINT, 0, 0);
	}
		break;
	case WM_TIMER: {
		timerProc(hWnd);
	}
		break;
	case WM_ERASEBKGND://窗口必须重绘时产生消息，return 0 不擦除背景，return 非 0 擦除背景
		return FALSE;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//	自定义函数
//
//  声明在planeswar.h头文件中
//

//init:
VOID initgame(LPARAM lParam) {
	BITMAP bmp;
	for (int i = 0; i < BMPCOUNT; i++) {
		hbBmp[i] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(uiBmpName[i]));
	}

	s_game.g_status = WELCOME;
	s_game.g_score = 0;
	s_game.g_bigAdd = FALSE;
	s_game.g_plNum = 15;

	GetObject(hbBmp[1], sizeof(BITMAP), &bmp);
	typeinfo[BIG].t_maxHP = 3;
	typeinfo[BIG].t_speed = 1;
	typeinfo[BIG].t_size.cx = bmp.bmWidth;
	typeinfo[BIG].t_size.cy = bmp.bmHeight / 4;	// 裁剪，一张 bmp 图内有四张小图

	GetObject(hbBmp[5], sizeof(BITMAP), &bmp);
	typeinfo[MIDDLE].t_maxHP = 2;
	typeinfo[MIDDLE].t_speed = 2;
	typeinfo[MIDDLE].t_size.cx = bmp.bmWidth;
	typeinfo[MIDDLE].t_size.cy = bmp.bmHeight / 3;	// 裁剪，一张 bmp 图内有三张小图

	GetObject(hbBmp[7], sizeof(BITMAP), &bmp);
	typeinfo[SMALL].t_maxHP = 1;
	typeinfo[SMALL].t_speed = 3;
	typeinfo[SMALL].t_size.cx = bmp.bmWidth;
	typeinfo[SMALL].t_size.cy = bmp.bmHeight / 2;	// 裁剪，一张 bmp 图内有两张小图

}

//sence:
VOID drawSenceUI(HDC hdc) {
	HBITMAP hBmpMem;
	HDC hdcMem, hdcTmp;
	static BITMAP bmp;
	
	// 双缓冲法绘图，解决重绘时闪烁的问题 
	// 创建与hDC环境相关的设备兼容的位图内存画布 
	hBmpMem = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	// 创建与hdc相兼容的内存dc 
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBmpMem);
	// hDcTmp是另一个临时的内存dc，用来储存部件，如背景，飞机，按钮等等 
	hdcTmp = CreateCompatibleDC(hdc);

	//画出背景图
	drawbackground(hdcMem, hdcTmp, hbBmp);

	switch (s_game.g_status) {
	case WELCOME:
		//TODO:welcome场景下的界面绘制
		drawSenceI(hdcMem, hdcTmp, hbBmp, bmp);
		break;
	case RUNING:
		//TODO:runing场景下的界面绘制
		drawSenceII(hdcMem, hdcTmp, hbBmp, bmp);	
		break;
	case OVER:
		//TODO:over场景下的界面绘制
		drawSenceIII(hdcMem, hdcTmp, hbBmp, bmp);
		break;
	}
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcMem, 0, 0, SRCCOPY);
	
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
	DeleteDC(hdcTmp);
}
VOID drawbackground(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp) {
	SelectObject(hdcTmp, hbBmp[0]);
	BitBlt(hdcMem, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcTmp, 0, 0, SRCCOPY);
}
VOID drawSenceI(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp, BITMAP bmp) {
	//画logo
	SelectObject(hdcTmp, hbBmp[4]);
	GetObject(hbBmp[4], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 35, 150, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//画start button
	SelectObject(hdcTmp, hbBmp[8]);
	GetObject(hbBmp[8], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 125, 380, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
}
VOID drawSenceII(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp, BITMAP bmp) {
	for (UINT i = 0; i < s_game.g_plNum; i++) {
		switch (planes[i].p_type) {
		case BIG:
			SelectObject(hdcTmp, hbBmp[1]);
			break;
		case MIDDLE:
			SelectObject(hdcTmp, hbBmp[5]);
			break;
		case SMALL:
			SelectObject(hdcTmp, hbBmp[7]);
			break;
		default:
			assert(0);
		}
		TransparentBlt(hdcMem, 
			planes[i].p_point.x, planes[i].p_point.y, 
			planes[i].p_size.cx, planes[i].p_size.cy,
			hdcTmp, 0, (planes[i].p_hitCount) * (planes[i].p_size.cy),
			planes[i].p_size.cx, planes[i].p_size.cy,
			RGB(255, 255, 255));	
	}
	
}
VOID drawSenceIII(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp, BITMAP bmp) {
	LOGFONT lf;
	HFONT	hf;
	RECT	rt;
	TCHAR	strScore[10];
	//画restart button
	SelectObject(hdcTmp, hbBmp[6]);
	GetObject(hbBmp[6], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 95, 320, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//画exit button
	SelectObject(hdcTmp, hbBmp[2]);
	GetObject(hbBmp[2], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 95, 380, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//画分数框
	SelectObject(hdcTmp, hbBmp[3]);
	GetObject(hbBmp[3], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 35, 70, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//画分数
	lf.lfHeight = 30;
	lf.lfWidth = 30;
	lf.lfEscapement = 0;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = CHINESEBIG5_CHARSET;

	hf = CreateFontIndirect(&lf);
	SelectObject(hdcMem, hf);

	rt.left = 75;
	rt.top = 140;
	rt.right = 300;
	rt.bottom = 260;

	_itot_s(s_game.g_score, strScore, 10);
	SetBkMode(hdcMem, TRANSPARENT);
	DrawText(hdcMem, strScore, -1, &rt, DT_CENTER);
}

//message event:
VOID lButtondownProc(HWND hwnd, LPARAM lParam) {
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	switch (s_game.g_status) {
	case WELCOME:
		// TODO:welcome场景下的鼠标事件
		if (checkbutton_start(ptMouse)) {
			s_game.g_status = RUNING;
			SendMessage(hwnd, WM_START, 0, 0);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case RUNING:
		// TODO:runing场景下的鼠标事件
		buttondown_hit(ptMouse);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case OVER:
		// TODO:over场景下的鼠标事件
		if (checkbutton_restart(ptMouse)) {
			s_game.g_score = 0;
			s_game.g_status = RUNING;
			SendMessage(hwnd, WM_START, 0, 0);
		}
		if (checkbutton_exit(ptMouse)) {
			SendMessage(hwnd, WM_DESTROY, 0, 0);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
}
VOID buttondown_hit(POINT ptMouse) {
	RECT rect;
	PLANETYPE type;
	for (UINT i = 0; i < s_game.g_plNum; i++) {
		type = planes[i].p_type;

		rect.left = planes[i].p_point.x;
		rect.top = planes[i].p_point.y;
		rect.right = rect.left + planes[i].p_size.cx;
		rect.bottom = rect.top + planes[i].p_size.cy;

		if (PtInRect(&rect, ptMouse)) {
			planes[i].p_hitCount++;

			if (planes[i].p_hitCount > planes[i].p_maxHP) {
				planes[i].p_hitCount = planes[i].p_maxHP;
			}

			PlaySound(
				MAKEINTRESOURCE(IDR_WAVE1),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC);

		}
	}
}
BOOL checkbutton_start(POINT ptMouse) {
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[8], sizeof(BITMAP), &bmp);
	rect.left = 125;
	rect.top = 380;
	rect.right = bmp.bmWidth + rect.left;
	rect.bottom = bmp.bmHeight + rect.top;
	return PtInRect(&rect, ptMouse);
}
BOOL checkbutton_restart(POINT ptMouse) {
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[6], sizeof(BITMAP), &bmp);
	rect.left = 95;
	rect.top = 320;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return PtInRect(&rect, ptMouse);
}
BOOL checkbutton_exit(POINT ptMouse) {
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[2], sizeof(BITMAP), &bmp);
	rect.left = 95;
	rect.top = 380;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return PtInRect(&rect, ptMouse);
}
VOID timerProc(HWND hwnd) {
	checkPlaneInfo();
	if (checkPlanePos()) {
		s_game.g_status = OVER;
		KillTimer(hwnd, TIMER);
	}
	for (UINT i = 0; i < s_game.g_plNum; i++) {
		planes[i].p_point.y += planes[i].p_speed;
	}
	InvalidateRect(hwnd, NULL, TRUE);	
}

//enime:
VOID updatePlaneInfo(BOOL bReset, UINT pIndex) {//bRest：是否是初次更新飞机信息
	UINT begin, end;

	if (bReset) {
		begin = 0;
		end = s_game.g_plNum;
	}
	else {
		begin = pIndex;
		end = pIndex + 1;
	}

	srand(GetTickCount());
	for ( UINT i = begin; i < end; i++) {
		UINT idex;
		idex = rand() % 3;
		while (idex == 0 && s_game.g_bigAdd) {
			idex = rand() % 3;
		}
		if (idex == 0 && !s_game.g_bigAdd) {
			s_game.g_bigAdd = TRUE;
		}
		switch (idex) {
		case BIG: {
			//srand(GetTickCount());
			planes[i].p_maxHP = typeinfo[BIG].t_maxHP;
			planes[i].p_speed = typeinfo[BIG].t_speed;
			planes[i].p_size = typeinfo[BIG].t_size;
			planes[i].p_hitCount = 0;
			planes[i].p_type = BIG;
			(planes[i].p_point).x = rand() % (WNDWIDTH - (planes[i].p_size).cx);
			(planes[i].p_point).y = -(rand() % 110 + 100);//- (plane->p_size).cy - rand() % ((plane->p_size).cy * (4 - idex));
		}
			 break;
		case MIDDLE: {
			//srand(GetTickCount());
			planes[i].p_maxHP = typeinfo[MIDDLE].t_maxHP;
			planes[i].p_speed = typeinfo[MIDDLE].t_speed;
			planes[i].p_size = typeinfo[MIDDLE].t_size;
			planes[i].p_hitCount = 0;
			planes[i].p_type = MIDDLE;
			(planes[i].p_point).x = rand() % (WNDWIDTH - (planes[i].p_size).cx);
			(planes[i].p_point).y = -(rand() % 350 + 80);//-(plane->p_size).cy - rand() % ((plane->p_size).cy * (4 - idex));

		}
			break;
		case SMALL: {
			//srand(GetTickCount());
			planes[i].p_maxHP = typeinfo[SMALL].t_maxHP;
			planes[i].p_speed = typeinfo[SMALL].t_speed;
			planes[i].p_size = typeinfo[SMALL].t_size;
			planes[i].p_hitCount = 0;
			planes[i].p_type = SMALL;
			(planes[i].p_point).x = rand() % (WNDWIDTH - (planes[i].p_size).cx);
			(planes[i].p_point).y = -(rand() % 470 + 30);//-(plane->p_size).cy - rand() % ((plane->p_size).cy * (4 - idex));
		}
			break;
		}
	}
}
VOID checkPlaneInfo() {//检查飞机信息，如果飞机被击数等于最大生命，updatePlaneInfo
	for (UINT i = 0; i < s_game.g_plNum; i++) {
		if (planes[i].p_hitCount == planes[i].p_maxHP) {
			if (planes[i].p_type == BIG) {
				s_game.g_bigAdd = FALSE;
			}
			switch (planes[i].p_type) {
			case BIG:
				s_game.g_score += 5;
				break;
			case MIDDLE:
				s_game.g_score += 3;
				break;
			case SMALL:
				s_game.g_score += 1;
				break;
			}
			updatePlaneInfo(FALSE, i);
		}
	}
}
BOOL checkPlanePos() {
	for (UINT i = 0; i < s_game.g_plNum; i++) {
		if (planes[i].p_point.y > WNDHEIGHT - 35) {
			return TRUE;
		}
	}
	return FALSE;
}