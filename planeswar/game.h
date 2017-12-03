#pragma once
#include "windows.h"
#include "resource.h"
#include "assert.h"
#include <mmsystem.h>
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")
#ifndef __GAME_H__
#define __GAME_H__

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
VOID		init_items(LPARAM, HWND, HBITMAP**, BULLET_LINK*, MYPLANE**, PLANE**, GAME*, HBITMAP*);//��ʼ��ȫ�ֱ���
VOID		free_items(HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*);						//��Ϸ����ʱ�ͷ���Դ
VOID		restart_game(HWND, BULLET_LINK*, MYPLANE*, GAME*);							//���¿�ʼ��Ϸ���������

//scence:
VOID		draw_senceUI(HDC, HBITMAP*, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);	//�ֳ�����������
VOID		draw_background(HDC, HDC, HBITMAP*, BITMAP, GAME*, HBITMAP*);					//������
VOID		draw_senceI(HDC, HDC, HBITMAP*, BITMAP);										//������һ��WELCOME
VOID		draw_senceII(HDC, HDC, HBITMAP*, BITMAP, BULLET_LINK*, MYPLANE*, PLANE*, GAME*);//����������RUNING
VOID		draw_senceIII(HDC, HDC, HBITMAP*, BITMAP, GAME*);								//����������OVER

//message event:
VOID		proc_lButtondown(HWND, LPARAM, BULLET_LINK*, MYPLANE*, GAME*, HBITMAP*);//�����������¼�
BOOL		check_button_start(POINT, HBITMAP*);					//��⿪ʼ��ť�Ƿ񱻰���
BOOL		check_button_restart(POINT, HBITMAP*);					//������¿�ʼ��ť�Ƿ񱻰���
BOOL		check_button_exit(POINT, HBITMAP*);						//����˳���ť�Ƿ񱻰���
BOOL		check_button_pause_play(POINT, HBITMAP*);				//�����ͣ/��ʼ��ť�Ƿ񱻰���
BOOL		check_button_sound_on_off(POINT, HBITMAP*);				//������ֿ��ذ�ť�Ƿ񱻰���
VOID		proc_timer(HWND, BULLET_LINK*, MYPLANE*, PLANE*, GAME*, HBITMAP*);//��ʱ���¼�

//enime:
VOID		update_plane_info(BOOL, UINT, PLANE*, GAME*);	//���µл���Ϣ
VOID		check_plane_info(PLANE*, GAME*);				//���л���Ϣ
VOID		check_plane_pos(PLANE*, GAME*);					//���ɻ�λ�ã��ж��Ƿ��зɻ�������Ļ�ײ������ǣ�update_plane_info
VOID		free_planes(PLANE*);							//�ͷ�planes��Դ

//sound:
VOID		play_backsound(GAME*);							//���ű�������

//control myplane:
VOID		move_myplane(WPARAM, HWND, MYPLANE*, GAME*);	//�ƶ������ɻ������������ң��ƶ�
BOOL		check_myplane_crash(MYPLANE*, PLANE*, GAME*);	//���л�����ҷɻ��Ƿ���ײ
VOID		free_myplane(MYPLANE*);							//�ͷ�myplane��Դ

//bullet:
VOID		create_bullet(BULLET_LINK*, MYPLANE*, HBITMAP*);//�����ӵ�
VOID		draw_bullet(BULLET_LINK*, HDC, HDC);			//�����ӵ�
VOID		move_bullet(BULLET_LINK*);						//�ƶ��ӵ�
BULLET*		check_bullet_pos(BULLET_LINK*);					//����ӵ��Ƿ����
BULLET*		check_bullet_info(BULLET_LINK*, PLANE*, GAME*);	//����ӵ��Ƿ���ел�
VOID		free_bullet(BULLET_LINK*, BULLET*);				//�ͷ��ӵ���Դ
VOID		free_allBullet(BULLET_LINK*);					//��Ϸ���¿�ʼʱ���ͷ������ӵ���Դ
#ifdef __cplusplus
}
#endif

#endif // __GAME_H__
