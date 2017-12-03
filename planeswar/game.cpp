#include "game.h"
#include "stdafx.h"
#include <stdlib.h>
int uiBmpName[BMPCOUNT] = {						// ����λͼID������
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
TYPEINFO typeinfo[3];			//�������ͷɻ�����Ϣ

//	�Զ��庯��
//
//  ������planeswar.hͷ�ļ���
//

/*
init/freeģ�飺
*/
VOID
init_items(LPARAM lParam, HWND hwnd, HBITMAP **backGrounds, BULLET_LINK *bullet_link,
	MYPLANE **myplane, PLANE **planes, GAME *s_game, HBITMAP *hbBmp) {
	// ��ʼ����Ϸ�ı���...
	BITMAP bmp;
	UINT flag = 0;
	for (int i = 0; i < BMPCOUNT; i++) {
		hbBmp[i] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(uiBmpName[i]));
	}
	*backGrounds = (HBITMAP *)malloc(24 * sizeof(HBITMAP));
	if (!backGrounds) exit(-2);
	for (int i = 14; i < 38; i++) {
		*(*(backGrounds)+flag) = hbBmp[i];
		flag++;
	}

	*myplane = (MYPLANE *)malloc(sizeof(MYPLANE));
	GetObject(hbBmp[10], sizeof(BITMAP), &bmp);
	((*myplane)->size).cx = bmp.bmWidth;
	((*myplane)->size).cy = bmp.bmHeight;
	((*myplane)->postion).x = WNDWIDTH / 2 - bmp.bmWidth / 2;
	((*myplane)->postion).y = WNDHEIGHT - 100;
	(*myplane)->moveSpeed = 5;
	(*myplane)->fireSpeed = 10;
	(*myplane)->hbmpPlane = hbBmp[10];
	(*myplane)->hitCounter = 0;
	(*myplane)->maxHp = 5;

	s_game->g_status = WELCOME;
	s_game->g_score = 0;
	s_game->g_bigAdd = FALSE;
	s_game->g_plNum = 10;
	s_game->g_pause_status = 0;//0Ϊδ��ͣ
	s_game->g_sound_status = 1;//0Ϊ�ر�����
	s_game->g_backGround_status = 0;//��ǰ���Ƶڼ��ű���ͼ
	s_game->g_backGround_couter = 0;
	s_game->g_bullet_counter = 0;

	GetObject(hbBmp[1], sizeof(BITMAP), &bmp);
	typeinfo[BIG].t_maxHP = 3;
	typeinfo[BIG].t_speed = 1;
	typeinfo[BIG].t_size.cx = bmp.bmWidth;
	typeinfo[BIG].t_size.cy = bmp.bmHeight / 4;	// �ü���һ�� bmp ͼ��������Сͼ

	GetObject(hbBmp[5], sizeof(BITMAP), &bmp);
	typeinfo[MIDDLE].t_maxHP = 2;
	typeinfo[MIDDLE].t_speed = 2;
	typeinfo[MIDDLE].t_size.cx = bmp.bmWidth;
	typeinfo[MIDDLE].t_size.cy = bmp.bmHeight / 3;	// �ü���һ�� bmp ͼ��������Сͼ

	GetObject(hbBmp[7], sizeof(BITMAP), &bmp);
	typeinfo[SMALL].t_maxHP = 1;
	typeinfo[SMALL].t_speed = 3;
	typeinfo[SMALL].t_size.cx = bmp.bmWidth;
	typeinfo[SMALL].t_size.cy = bmp.bmHeight / 2;	// �ü���һ�� bmp ͼ��������Сͼ

	bullet_link->head = NULL;
	bullet_link->tail = NULL;

	*planes = (PLANE *)malloc(MAXPLANENUM * sizeof(PLANE));

	SendMessage(hwnd, WM_PLAYBACKSOUND, 0, 0);

}
VOID
free_items(HBITMAP *backGrouds, BULLET_LINK *bullet_link, MYPLANE *myplane, PLANE *planes) {
	free(backGrouds);
	backGrouds = NULL;
	free_myplane(myplane);
	free_planes(planes);
	free_allBullet(bullet_link);
}
VOID
restart_game(HWND hwnd, BULLET_LINK *bullet_link, MYPLANE *myplane, GAME *s_game) {
	s_game->g_score = 0;
	s_game->g_status = RUNING;

	myplane->hitCounter = 0;
	(myplane->postion).x = WNDWIDTH / 2 - (myplane->size).cx / 2;
	(myplane->postion).y = WNDHEIGHT - 100;

	free_allBullet(bullet_link);
	SendMessage(hwnd, NULL, 0, 0);
}

/*
senceģ�飺
*/
VOID
draw_senceUI(HDC hdc, HBITMAP *backGrouds, BULLET_LINK *bullet_link,
	MYPLANE *myplane, PLANE *planes, GAME *s_game, HBITMAP *hbBmp) {// �ֲ�ͬ�ĳ�����������ͬ�Ľ��� //
	HBITMAP hBmpMem;
	HDC hdcMem, hdcTmp;
	static BITMAP bmp;

	// ˫���巨��ͼ������ػ�ʱ��˸������ 
	// ������hDC������ص��豸���ݵ�λͼ�ڴ滭�� 
	hBmpMem = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	// ������hdc����ݵ��ڴ�dc 
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBmpMem);
	// hDcTmp����һ����ʱ���ڴ�dc���������沿�����米�����ɻ�����ť�ȵ� 
	hdcTmp = CreateCompatibleDC(hdc);

	//��������ͼ
	draw_background(hdcMem, hdcTmp, backGrouds, bmp, s_game, hbBmp);

	switch (s_game->g_status) {
	case WELCOME:
		//TODO:welcome�����µĽ������
		draw_senceI(hdcMem, hdcTmp, hbBmp, bmp);
		break;
	case RUNING:
		//TODO:runing�����µĽ������
		draw_senceII(hdcMem, hdcTmp, hbBmp, bmp, bullet_link, myplane, planes, s_game);
		break;
	case OVER:
		//TODO:over�����µĽ������
		draw_senceIII(hdcMem, hdcTmp, hbBmp, bmp, s_game);
		break;
	}
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcMem, 0, 0, SRCCOPY);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
	DeleteDC(hdcTmp);
}
VOID
draw_background(HDC hdcMem, HDC hdcTmp, HBITMAP *backGrounds,
	BITMAP bmp, GAME *s_game, HBITMAP *hbBmp) {// ����������������������Ҫ�������� //
											   //������
	SelectObject(hdcTmp, *(backGrounds + s_game->g_backGround_status));
	BitBlt(hdcMem, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcTmp, 0, 0, SRCCOPY);
	if (s_game->g_status == RUNING) {
		s_game->g_backGround_couter++;
		if (s_game->g_backGround_couter == 2) {
			s_game->g_backGround_couter = 0;
			s_game->g_backGround_status++;
		}
	}
	if (s_game->g_backGround_status == 24) {
		s_game->g_backGround_status = 0;
	}
	//���������ֿ��ذ�ť
	int tmp = 11;
	if (s_game->g_sound_status == 0)
		tmp = 12;
	SelectObject(hdcTmp, hbBmp[tmp]);
	GetObject(hbBmp[tmp], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 330, 0, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

}
VOID
draw_senceI(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp, BITMAP bmp) {// ������һ������logo����ʼ��ť //
																 //��logo
	SelectObject(hdcTmp, hbBmp[4]);
	GetObject(hbBmp[4], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 35, 150, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//��start button
	SelectObject(hdcTmp, hbBmp[8]);
	GetObject(hbBmp[8], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 125, 380, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
}
VOID
draw_senceII(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp,
	BITMAP bmp, BULLET_LINK *bullet_link, MYPLANE *myplane, PLANE *planes, GAME *s_game) {// ���������������л��������ɻ�����ʼ/��ͣ��ť //
	for (UINT i = 0; i < s_game->g_plNum; i++) {
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
	//���ҵķɻ�
	SelectObject(hdcTmp, myplane->hbmpPlane);
	TransparentBlt(hdcMem,
		(myplane->postion).x, (myplane->postion).y,
		(myplane->size).cx, (myplane->size).cy,
		hdcTmp, 0, 0,
		(myplane->size).cx, (myplane->size).cy, RGB(255, 255, 255));

	//��play/pause button
	SelectObject(hdcTmp, hbBmp[9]);
	GetObject(hbBmp[9], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight / 2,
		hdcTmp, 0, s_game->g_pause_status * bmp.bmHeight / 2, bmp.bmWidth, bmp.bmHeight / 2, RGB(255, 255, 255));

	//���ӵ�
	draw_bullet(bullet_link, hdcTmp, hdcMem);
}
VOID
draw_senceIII(HDC hdcMem, HDC hdcTmp, HBITMAP *hbBmp, BITMAP bmp, GAME *s_game) {// �������������������򣬷������˳���ť�����¿�ʼ��ť //
	LOGFONT lf;
	HFONT	hf;
	RECT	rt;
	TCHAR	strScore[10];
	//��restart button
	SelectObject(hdcTmp, hbBmp[6]);
	GetObject(hbBmp[6], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 95, 320, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//��exit button
	SelectObject(hdcTmp, hbBmp[2]);
	GetObject(hbBmp[2], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 95, 380, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//��������
	SelectObject(hdcTmp, hbBmp[3]);
	GetObject(hbBmp[3], sizeof(BITMAP), &bmp);
	TransparentBlt(hdcMem, 35, 70, bmp.bmWidth, bmp.bmHeight,
		hdcTmp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	//������
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

	_itot_s(s_game->g_score, strScore, 10, 0);
	SetBkMode(hdcMem, TRANSPARENT);
	DrawText(hdcMem, strScore, -1, &rt, DT_CENTER);
}

/*
message eventģ�飺
*/
VOID
proc_lButtondown(HWND hwnd, LPARAM lParam, BULLET_LINK *bullet_link,
	MYPLANE *myplane, GAME *s_game, HBITMAP *hbBmp) {// �����������¼����ڲ�ͬ�ĳ����в�ͬ������ //
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	if (check_button_sound_on_off(ptMouse, hbBmp)) {
		if (s_game->g_sound_status) {
			s_game->g_sound_status = 0;
		}
		else {
			s_game->g_sound_status = 1;
		}
		InvalidateRect(hwnd, NULL, TRUE);
	}
	switch (s_game->g_status) {
	case WELCOME:
		// TODO:welcome�����µ�����¼�
		if (check_button_start(ptMouse, hbBmp)) {
			s_game->g_status = RUNING;
			SendMessage(hwnd, WM_START, 0, 0);
		}
		break;
	case RUNING:
		// TODO:runing�����µ�����¼�
		if (check_button_pause_play(ptMouse, hbBmp)) {
			if (s_game->g_pause_status) {
				SetTimer(hwnd, TIMER, 50, NULL);
				s_game->g_pause_status = 0;
			}
			else {
				KillTimer(hwnd, TIMER);
				s_game->g_pause_status = 1;
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case OVER:
		// TODO:over�����µ�����¼�
		if (check_button_restart(ptMouse, hbBmp)) {
			restart_game(hwnd, bullet_link, myplane, s_game);
		}
		if (check_button_exit(ptMouse, hbBmp)) {
			SendMessage(hwnd, WM_DESTROY, 0, 0);
		}
		break;
	}
}
BOOL
check_button_start(POINT ptMouse, HBITMAP *hbBmp) {// ��⿪ʼ��ť�Ƿ񱻰��£����ǣ�����TRUE //
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[8], sizeof(BITMAP), &bmp);
	rect.left = 125;
	rect.top = 380;
	rect.right = bmp.bmWidth + rect.left;
	rect.bottom = bmp.bmHeight + rect.top;
	return PtInRect(&rect, ptMouse);
}
BOOL
check_button_restart(POINT ptMouse, HBITMAP *hbBmp) {// ������¿�ʼ��ť�Ƿ񱻰��£����ǣ�����TRUE //
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[6], sizeof(BITMAP), &bmp);
	rect.left = 95;
	rect.top = 320;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return PtInRect(&rect, ptMouse);
}
BOOL
check_button_exit(POINT ptMouse, HBITMAP *hbBmp) {// ����˳���ť�Ƿ񱻰��£����ǣ�����TRUE //
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[2], sizeof(BITMAP), &bmp);
	rect.left = 95;
	rect.top = 380;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return PtInRect(&rect, ptMouse);
}
BOOL
check_button_pause_play(POINT ptMouse, HBITMAP *hbBmp) {// �����ͣ/��ʼ��ť�����Ƿ񱻰��£����ǣ�����TRUE //
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[9], sizeof(BITMAP), &bmp);
	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight / 2;
	return PtInRect(&rect, ptMouse);
}
BOOL
check_button_sound_on_off(POINT ptMouse, HBITMAP *hbBmp) {// ���������ť�Ƿ񱻰��� //
	RECT rect;
	BITMAP bmp;
	GetObject(hbBmp[11], sizeof(BITMAP), &bmp);
	rect.left = 330;
	rect.top = 0;
	rect.right = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return PtInRect(&rect, ptMouse);
}
VOID
proc_timer(HWND hwnd, BULLET_LINK *bullet_link, MYPLANE *myplane,
	PLANE *planes, GAME *s_game, HBITMAP *hbBmp) {// Timer��ʱ�����������¼���
												  //ÿ��λʱ����ִ��һ��check_plane_info��check_plane_pos��
												  //�ƶ��л����ػ���Ļ 
												  //�л�ģ�飺
	check_plane_info(planes, s_game);
	check_plane_pos(planes, s_game);
	if (check_myplane_crash(myplane, planes, s_game)) {
		myplane->hitCounter++;
	}
	if (myplane->hitCounter == myplane->maxHp) {
		s_game->g_status = OVER;
		SendMessage(hwnd, WM_PLAYBACKSOUND, 0, 0);
		KillTimer(hwnd, TIMER);
	}
	for (UINT i = 0; i < s_game->g_plNum; i++) {
		planes[i].p_point.y += planes[i].p_speed;
	}
	//�ӵ�ģ�飺
	s_game->g_bullet_counter++;
	if ((s_game->g_bullet_counter == myplane->fireSpeed || bullet_link->head == NULL)
		&& (myplane->postion.y >= 5)) {// �����ӵ����������ӵ�Ϊ�� || ÿfirespeed��λʱ�䣩
		create_bullet(bullet_link, myplane, hbBmp);
		s_game->g_bullet_counter = 0;
	}
	move_bullet(bullet_link);
	BULLET *tmp1 = check_bullet_pos(bullet_link);
	BULLET *tmp2 = check_bullet_info(bullet_link, planes, s_game);
	if (tmp1 != NULL) {
		free_bullet(bullet_link, tmp1);
	}
	if (tmp2 != NULL) {
		free_bullet(bullet_link, tmp2);
	}

	InvalidateRect(hwnd, NULL, TRUE);
}

/*
enimeģ��
*/
VOID
update_plane_info(BOOL bReset, UINT pIndex, PLANE *planes, GAME *s_game) { // ���·ɻ�����Ϣ	bRest���Ƿ��ǳ��θ��·ɻ���Ϣ pIndex����Ҫ���·ɻ���Ϣ�ķɻ����±� //
	UINT begin, end;

	if (bReset) {
		begin = 0;
		end = s_game->g_plNum;
	}
	else {
		begin = pIndex;
		end = pIndex + 1;
	}

	srand(GetTickCount());
	for (UINT i = begin; i < end; i++) {
		UINT idex;
		idex = rand() % 3;
		while (idex == 0 && s_game->g_bigAdd) {
			idex = rand() % 3;
		}
		if (idex == 0 && !s_game->g_bigAdd) {
			s_game->g_bigAdd = TRUE;
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
VOID
check_plane_info(PLANE *planes, GAME *s_game) { // ���ɻ���Ϣ������ɻ��������������������update_plane_info //
	for (UINT i = 0; i < s_game->g_plNum; i++) {
		if (planes[i].p_hitCount == planes[i].p_maxHP) {
			if (planes[i].p_type == BIG) {
				s_game->g_bigAdd = FALSE;
			}
			switch (planes[i].p_type) {
			case BIG:
				s_game->g_score += 5;
				break;
			case MIDDLE:
				s_game->g_score += 3;
				break;
			case SMALL:
				s_game->g_score += 1;
				break;
			}
			update_plane_info(FALSE, i, planes, s_game);
		}
	}
}
VOID
check_plane_pos(PLANE *planes, GAME *s_game) {// ���ɻ�λ�ã�����ɻ�������Ļ�ײ���update_plane_info //
	for (UINT i = 0; i < s_game->g_plNum; i++) {
		if (planes[i].p_point.y > WNDHEIGHT - 15) {
			if (planes[i].p_type == BIG)
				s_game->g_bigAdd = FALSE;
			update_plane_info(FALSE, i, planes, s_game);
		}
	}
}
VOID
free_planes(PLANE *planes) {// �ͷ�planes��Դ //
	free(planes);
}

/*
soundģ�飺
*/
VOID
play_backsound(GAME *s_game) {// ���ű������� //
	switch (s_game->g_status) {
	case WELCOME:
		if (s_game->g_sound_status) {
			PlaySound(
				MAKEINTRESOURCE(IDR_WELSOUND),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC | SND_LOOP);
		}
		break;
	case RUNING: {
		if (s_game->g_sound_status) {
			PlaySound(
				MAKEINTRESOURCE(IDR_RUNSOUND),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC | SND_LOOP);
		}
	}
				 break;
	case OVER: {
		if (s_game->g_sound_status) {
			PlaySound(
				MAKEINTRESOURCE(IDR_OVERS),
				GetModuleHandle(NULL),
				SND_RESOURCE | SND_ASYNC | SND_LOOP);
		}
	}
			   break;
	}
}

/*
myplaneģ�飺
*/
VOID
move_myplane(WPARAM wParam, HWND hWnd, MYPLANE *myplane, GAME *s_game) {// ��ҿ��Ƽ����ɻ��ƶ� //
	if (s_game->g_pause_status) {
		return;
	}
	switch (wParam) {
	case VK_UP:
		if (myplane->postion.y < 0) return;
		myplane->postion.y -= myplane->moveSpeed;
		break;
	case VK_DOWN:
		if (myplane->postion.y > WNDHEIGHT - 90) return;
		myplane->postion.y += myplane->moveSpeed;
		break;
	case VK_LEFT:
		if (myplane->postion.x < 0) return;
		myplane->postion.x -= myplane->moveSpeed;
		break;
	case VK_RIGHT:
		if (myplane->postion.x > WNDWIDTH - myplane->size.cx - 15) return;
		myplane->postion.x += myplane->moveSpeed;
		break;
	}
	//SendMessage(hWnd, WM_PAINT, 0, 0);
}
BOOL
check_myplane_crash(MYPLANE *myplane, PLANE *planes, GAME *s_game) {// ���л�����ҷɻ��Ƿ���ײ //
	POINT pt;
	RECT rect;
	for (UINT i = 0; i < s_game->g_plNum; i++) {
		pt.x = planes[i].p_point.x + planes[i].p_size.cx / 2;
		pt.y = planes[i].p_point.y + planes[i].p_size.cy;
		rect.left = myplane->postion.x;
		rect.top = myplane->postion.y;
		rect.right = rect.left + myplane->size.cx;
		rect.bottom = rect.top + myplane->size.cy;
		if (PtInRect(&rect, pt)) {
			planes[i].p_hitCount = planes[i].p_maxHP;
			return TRUE;
		}
	}
	return FALSE;
}
VOID
free_myplane(MYPLANE *myplane) {
	free(myplane);
	myplane = NULL;
}

/*
bulletģ�飺
*/
VOID
create_bullet(BULLET_LINK *bullet_link, MYPLANE *myplane, HBITMAP *hbBmp) {// �����ӵ� //
	BITMAP bmp;
	BULLET *newBullet = (BULLET *)malloc(sizeof(BULLET));
	if (!newBullet)	exit(-2);
	GetObject(hbBmp[38], sizeof(BITMAP), &bmp);
	(newBullet->bullet_size).cx = bmp.bmWidth;
	(newBullet->bullet_size).cy = bmp.bmHeight;
	(newBullet->bullet_pos).x = myplane->postion.x + myplane->size.cx / 2 - 5;
	(newBullet->bullet_pos).y = myplane->postion.y - 19;
	newBullet->move_speed = 4;
	newBullet->next = NULL;
	newBullet->pre = NULL;
	newBullet->hbmp = hbBmp[38];
	if (bullet_link->head == NULL) {
		bullet_link->head = newBullet;
		bullet_link->tail = newBullet;
		return;
	}
	(bullet_link->tail)->next = newBullet;
	newBullet->pre = bullet_link->tail;
	bullet_link->tail = newBullet;
}
VOID
draw_bullet(BULLET_LINK *bullet_link, HDC hdcTmp, HDC hdcMem) {// �����ӵ� //
	BULLET *p = bullet_link->head;
	while (p != NULL) {
		SelectObject(hdcTmp, p->hbmp);
		TransparentBlt(hdcMem,
			(p->bullet_pos).x, (p->bullet_pos).y,
			(p->bullet_size).cx, (p->bullet_size).cy,
			hdcTmp, 0, 0,
			(p->bullet_size).cx, (p->bullet_size).cy, RGB(255, 255, 255));
		p = p->next;
	}
}
VOID
move_bullet(BULLET_LINK *bullet_link) {// �ӵ��ƶ� //
	BULLET *p = bullet_link->head;
	while (p != NULL) {
		(p->bullet_pos).y -= p->move_speed;
		p = p->next;
	}
}
BULLET*
check_bullet_pos(BULLET_LINK *bullet_link) {// ��Ϊ��headλ���ӵ�һ��������ǰ�棬�ʼ��headλ���ɣ����緵�س���ɻ��ĵ�ַ //
	if (bullet_link->head == NULL) {
		return NULL;
	}
	if (((bullet_link->head)->bullet_pos).y < -150) {
		return bullet_link->head;
	}
	return NULL;
}
BULLET*
check_bullet_info(BULLET_LINK *bullet_link, PLANE *planes, GAME *s_game) {// �ж��ӵ��Ƿ���ел� ���ھͲ���ֻ���headλ���ӵ��� //
	BULLET *p = bullet_link->head;
	RECT rect;
	POINT point;
	while (p != NULL) {
		point.x = (p->bullet_pos).x + (p->bullet_size).cx / 2;
		point.y = (p->bullet_pos).y;
		for (UINT i = 0; i < s_game->g_plNum; i++) {
			rect.left = planes[i].p_point.x;
			rect.top = planes[i].p_point.y;
			rect.right = rect.left + planes[i].p_size.cx;
			rect.bottom = rect.top + planes[i].p_size.cy;

			if (PtInRect(&rect, point) && planes[i].p_point.y > -110) {
				planes[i].p_hitCount++;
				if (planes[i].p_hitCount > planes[i].p_maxHP) {
					planes[i].p_hitCount = planes[i].p_maxHP;
				}
				return p;
			}
		}
		p = p->next;
	}
	return NULL;
}
VOID
free_bullet(BULLET_LINK *bullet_link, BULLET *bul) {
	if (bullet_link->head == bullet_link->tail) {
		bullet_link->head = NULL;
		bullet_link->tail = NULL;
		free(bul);
		bul = NULL;
		return;
	}
	if (bul == bullet_link->head) {
		bullet_link->head = bul->next;
		if (bullet_link->head != NULL)
			(bullet_link->head)->pre = NULL;
		free(bul);
		bul = NULL;
		return;
	}
	if (bul == bullet_link->tail) {
		bullet_link->tail = bul->pre;
		if (bullet_link->tail != NULL)
			(bullet_link->tail)->next = NULL;
		free(bul);
		bul = NULL;
		return;
	}
	assert(bul->pre);
	if (bul->pre != NULL)
		(bul->pre)->next = bul->next;
	if (bul->next != NULL)
		(bul->next)->pre = bul->pre;
	free(bul);
	bul = NULL;
}
VOID
free_allBullet(BULLET_LINK *bullet_link) {// ���¿�ʼ��Ϸʱ���ͷ������ӵ���Դ //
	BULLET *p = bullet_link->head;
	BULLET *tmp = NULL;
	while (p != NULL) {
		tmp = p->next;
		free(p);
		p = tmp;
	}
	bullet_link->head = NULL;
	bullet_link->tail = NULL;
}