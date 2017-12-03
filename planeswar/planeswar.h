#pragma once
#include <assert.h>
#include <mmsystem.h>
#include <time.h>
#include "resource.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING	100

//定义窗口尺寸
#define WNDWIDTH		380
#define WNDHEIGHT		550

#define BMPCOUNT		39		//bmp数量
#define TIMER			1		//timer ID
#define MAXPLANENUM		100		//最大可容纳飞机数量

#define WM_START (WM_USER + 1)	//自定义开始事件
#define WM_PLAYBACKSOUND (WM_USER + 2)	//自定义播放背景音乐事件

//声明结构体
typedef enum {
	WELCOME,
	RUNING,
	OVER,
}GAMESTATUS;//游戏所处状态

typedef enum {
	BIG = 0,
	MIDDLE = 1,
	SMALL = 2,
}PLANETYPE;

typedef struct {
	GAMESTATUS		g_status;
	UINT			g_score;
	UINT			g_plNum;
	BOOL			g_bigAdd;
	UINT			g_pause_status;
	UINT			g_sound_status;
	UINT			g_backGround_status;
	UINT			g_backGround_couter;//用于调节背景动画播放速度
	UINT			g_bullet_counter;	//辅助调节子弹生成速度
}GAME;

typedef struct {
	PLANETYPE		p_type;
	SIZE			p_size;
	POINT			p_point;
	UINT			p_speed;
	UINT			p_maxHP;
	UINT			p_hitCount;
}PLANE;

typedef struct {
	UINT			t_speed;
	UINT			t_maxHP;
	SIZE			t_size;
}TYPEINFO;

typedef struct {
	HBITMAP hbmpPlane;
	UINT	moveSpeed;
	UINT	fireSpeed;
	POINT	postion;
	SIZE	size;
	UINT	hitCounter;
	UINT	maxHp;
}MYPLANE;

typedef struct bullet{
	HBITMAP hbmp;
	UINT	move_speed;
	POINT	bullet_pos;
	SIZE	bullet_size;
	struct bullet *next;
	struct bullet *pre;
}BULLET;

typedef struct {
	BULLET *head;
	BULLET *tail;
}BULLET_LINK;

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
int uiBmpName[BMPCOUNT] = {						// 储存位图ID的数组
	IDB_BG1,		//0
	IDB_BIG,		//1
	IDB_EXIT,		//2
	IDB_GAMEOVER,	//3
	IDB_LOGO,		//4
	IDB_MIDDLE,		//5
	IDB_RESTART,	//6
	IDB_SMALL,		//7
	IDB_START,		//8
	IDB_PLAYORPAUSE,//9
	IDB_MYPLANE,	//10
	IDB_MUSICON,	//11
	IDB_MUSICOFF,	//12
	IDB_BG1,		//13
	IDB_BG1,		//14
	IDB_BG2,		//15
	IDB_BG3,		//16
	IDB_BG4,		//17
	IDB_BG5,		//18
	IDB_BG6,		//19
	IDB_BG7,		//20
	IDB_BG8,		//21
	IDB_BG9,		//22
	IDB_BG10,		//23
	IDB_BG11,		//24
	IDB_BG12,		//25
	IDB_BG13,		//26
	IDB_BG14,		//27
	IDB_BG15,		//28
	IDB_BG16,		//29
	IDB_BG17,		//30
	IDB_BG18,		//31
	IDB_BG19,		//32
	IDB_BG20,		//33
	IDB_BG21,		//34
	IDB_BG22,		//35
	IDB_BG23,		//36
	IDB_BG24,		//37
	IDB_BULLET,		//38
};
TYPEINFO typeinfo[3];//big_info, middle_info, small_info;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//init/free:
VOID		init_items(LPARAM, HWND, HBITMAP**, BULLET_LINK*, MYPLANE**, PLANE**, GAME*, HBITMAP*);//初始化全局变量
VOID		free_items(HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*);						//游戏结束时释放资源
VOID		restart_game(HWND, BULLET_LINK*, MYPLANE*, GAME*);							//重新开始游戏，重设变量

//scence:
VOID		draw_senceUI(HDC, HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);	//分场景画出界面
VOID		draw_background(HDC, HDC, HBITMAP*, BITMAP, GAME*, HBITMAP*);					//画背景
VOID		draw_senceI(HDC, HDC, HBITMAP*, BITMAP);										//画场景一：WELCOME
VOID		draw_senceII(HDC, HDC, HBITMAP*, BITMAP, BULLET_LINK*, MYPLANE*, PLANE*, GAME*);//画场景二：RUNING
VOID		draw_senceIII(HDC, HDC, HBITMAP*, BITMAP, GAME*);								//画场景三：OVER

//message event:
VOID		proc_lButtondown(HWND, LPARAM, BULLET_LINK*, MYPLANE*, GAME*, HBITMAP*);//鼠标左键按下事件
BOOL		check_button_start(POINT, HBITMAP*);					//检测开始按钮是否被按下
BOOL		check_button_restart(POINT, HBITMAP*);					//检测重新开始按钮是否被按下
BOOL		check_button_exit(POINT, HBITMAP*);						//检测退出按钮是否被按下
BOOL		check_button_pause_play(POINT, HBITMAP*);				//检测暂停/开始按钮是否被按下
BOOL		check_button_sound_on_off(POINT, HBITMAP*);				//检测音乐开关按钮是否被按下
VOID		proc_timer(HWND, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);//计时器事件

//enime:
VOID		update_plane_info(BOOL, UINT, PLANE*, GAME*);	//跟新敌机信息
VOID		check_plane_info(PLANE*, GAME*);				//检查敌机信息
VOID		check_plane_pos(PLANE*, GAME*);					//检查飞机位置，判断是否有飞机到达屏幕底部，如是，update_plane_info
VOID		free_planes(PLANE*);							//释放planes资源

//sound:
VOID		play_backsound(GAME*);							//播放背景音乐

//control myplane:
VOID		move_myplane(WPARAM, HWND, MYPLANE*, GAME*);	//移动己方飞机，用上下左右，移动
BOOL		check_myplane_crash(MYPLANE*, PLANE*, GAME*);	//检查敌机和玩家飞机是否相撞
VOID		free_myplane(MYPLANE*);							//释放myplane资源

//bullet:
VOID		create_bullet(BULLET_LINK*, MYPLANE*, HBITMAP*);//生成子弹
VOID		draw_bullet(BULLET_LINK*, HDC, HDC);			//画出子弹
VOID		move_bullet(BULLET_LINK*);						//移动子弹
BULLET*		check_bullet_pos(BULLET_LINK*);					//检查子弹是否出界
BULLET*		check_bullet_info(BULLET_LINK*, PLANE*, GAME*);	//检查子弹是否击中敌机
VOID		free_bullet(BULLET_LINK*, BULLET*);				//释放子弹资源
VOID		free_allBullet(BULLET_LINK*);					//游戏重新开始时，释放所有子弹资源