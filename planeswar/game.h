#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#ifdef __cplusplus
extern C
#endif


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







//enime:
void				updatePlaneInfo(BOOL, UINT);				//���µл���Ϣ
void				checkPlaneInfo();							//���л���Ϣ
BOOL				checkPlanePos();							//���ɻ�λ�ã��ж��Ƿ��зɻ�������Ļ�ײ�


#ifdef __cplusplus
extern C
#endif

#endif