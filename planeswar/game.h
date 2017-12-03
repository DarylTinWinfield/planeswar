/**********************************************
				Editor:Joezeo
		  E-mail:joezeo@outlook.com

**********************************************/
#ifndef __GAME_H__
#define __GAME_H__

#include "windows.h"
#include "resource.h"
#include "assert.h"
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")

#ifdef __cplusplus
extern "C" {
#endif

//���崰�ڳߴ�
#define WNDWIDTH		380
#define WNDHEIGHT		550

#define BMPCOUNT		39		//bmp����
#define TIMER			1		//timer ID
#define MAXPLANENUM		100		//�������ɷɻ�����


#define WM_START (WM_USER + 1)			//�Զ��忪ʼ�¼�
#define WM_PLAYBACKSOUND (WM_USER + 2)	//�Զ��岥�ű��������¼�

//�����ṹ��
typedef enum {
	WELCOME,
	RUNING,
	OVER,
}GAMESTATUS;//��Ϸ����״̬

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
	UINT			g_backGround_couter;//���ڵ��ڱ������������ٶ�
	UINT			g_bullet_counter;	//���������ӵ������ٶ�
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

typedef struct bullet {
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



//init/free:
VOID			//��ʼ�����б���	
init_items(LPARAM, HWND, HBITMAP**, BULLET_LINK*, MYPLANE**, PLANE**, GAME*, HBITMAP*);

VOID			//��Ϸ����ʱ�ͷ���Դ
free_items(HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*);						

static VOID		//���¿�ʼ��Ϸ���������
restart_game(HWND, BULLET_LINK*, MYPLANE*, GAME*);							


//scence:
VOID			//�ֳ�����������	
draw_senceUI(HDC, HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);	

static VOID		//������
draw_background(HDC, HDC, HBITMAP*, BITMAP, GAME*, HBITMAP*);					

static VOID		//������һ��WELCOME
draw_senceI(HDC, HDC, HBITMAP*, BITMAP);										

static VOID		//����������RUNING
draw_senceII(HDC, HDC, HBITMAP*, BITMAP, BULLET_LINK*, MYPLANE*, PLANE*, GAME*);

static VOID		//����������OVER
draw_senceIII(HDC, HDC, HBITMAP*, BITMAP, GAME*);								


//message event:
VOID			//��ʱ���¼�	
proc_timer(HWND, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);		

VOID			//�����������¼�
proc_lButtondown(HWND, LPARAM, BULLET_LINK*, MYPLANE*, GAME*, HBITMAP*);

static BOOL		//��⿪ʼ��ť�Ƿ񱻰���
check_button_start(POINT, HBITMAP*);					

static BOOL		//������¿�ʼ��ť�Ƿ񱻰���
check_button_restart(POINT, HBITMAP*);					

static BOOL		//����˳���ť�Ƿ񱻰���
check_button_exit(POINT, HBITMAP*);						

static BOOL		//�����ͣ/��ʼ��ť�Ƿ񱻰���
check_button_pause_play(POINT, HBITMAP*);				

static BOOL		//������ֿ��ذ�ť�Ƿ񱻰���
check_button_sound_on_off(POINT, HBITMAP*);				


//enime:
VOID			//���µл���Ϣ
update_plane_info(BOOL, UINT, PLANE*, GAME*);	

static VOID		//���л���Ϣ
check_plane_info(PLANE*, GAME*);				

static VOID		//���ɻ�λ�ã��ж��Ƿ��зɻ�������Ļ�ײ������ǣ�update_plane_info
check_plane_pos(PLANE*, GAME*);					

static VOID		//�ͷ�planes��Դ
free_planes(PLANE*);							


//sound:
VOID			//���ű�������
play_backsound(GAME*);							


//control myplane:
VOID			//�ƶ������ɻ������������ң��ƶ�
move_myplane(WPARAM, HWND, MYPLANE*, GAME*);	

static BOOL		//���л�����ҷɻ��Ƿ���ײ
check_myplane_crash(MYPLANE*, PLANE*, GAME*);	

static VOID		//�ͷ�myplane��Դ
free_myplane(MYPLANE*);							


//bullet:
static VOID		//�����ӵ�
create_bullet(BULLET_LINK*, MYPLANE*, HBITMAP*);

static VOID		//�����ӵ�
draw_bullet(BULLET_LINK*, HDC, HDC);			

static VOID		//�ƶ��ӵ�
move_bullet(BULLET_LINK*);						

static BULLET*	//����ӵ��Ƿ����
check_bullet_pos(BULLET_LINK*);					

static BULLET*	//����ӵ��Ƿ���ел�
check_bullet_info(BULLET_LINK*, PLANE*, GAME*);	

static VOID		//�ͷ��ӵ���Դ	
free_bullet(BULLET_LINK*, BULLET*);				

static VOID		//��Ϸ���¿�ʼʱ���ͷ������ӵ���Դ
free_allBullet(BULLET_LINK*);					


#ifdef __cplusplus
}
#endif

#endif // __GAME_H__
