#include"acllib.h"
#include<time.h>
#include<malloc.h>
#include<stdio.h>
const int winWidth = 960, winHeight = 660;
ACL_Image tomimg;
ACL_Image jerryimg;
ACL_Sound sound;
ACL_Image dogimg;
ACL_Image heartimg;
ACL_Image beginimg;
ACL_Image endimg;
const int maxNum = 2000;
const int maxJerryNum = 20;
const int maxDogNum = 20;
const int maxHeartNum = 20;
int jerryNum = 0;
int dogNum = 0;
int heartNum = 0;
int preJerryNum = 0;
int preDogNum = 0;
int preHeartNum = 0;
int flag = 0;
void timerEvent(int id);//定时器
void keyEvent(int key, int e);//键盘
void initgame();//初始化函数
int moveid = 0, movetimerInterval = 30;
int createid = 1, createtimerInterval = 1000;
int dogid1 = 2, dogtimerInterval1 = 5000;
int dogid2 = 3, dogtimerInterval2 = 30;
int heartid1 = 4, hearttimerInterval1 = 30;
int heartid2 = 5, hearttimerInterval2 = 8000;
int score1 = 0, score2 = 8;
void beginpaint();//开始界面
void Paint();//游戏界面
void endpaint();//结束界面
struct sprite//精灵
{
	int x, y;
	int width, height;
	int distx,disty;
};
typedef struct rect
{
	int x, y;
	int w, h;
}rect;
int collision(rect r1, rect r2);//检测碰撞
void change(int score);//改变Tom猫的大小
struct sprite tomsprite, *jerrysprite[maxNum]={NULL},*dogsprite[maxNum]={NULL},*heartsprite[maxNum]={NULL};
int Setup()
{
	initWindow("My first game", DEFAULT, DEFAULT, winWidth, winHeight);
	loadImage("tom.bmp", &tomimg);
	loadImage("jerry.bmp", &jerryimg);
	loadSound("tada.wav", &sound);
	loadImage("dog.bmp", &dogimg);
	loadImage("heart.bmp", &heartimg);
	loadImage("begin.bmp", &beginimg);
	loadImage("end.bmp", &endimg);
	srand((unsigned)time(NULL));
	initgame();
	registerTimerEvent(timerEvent);
	registerKeyboardEvent(keyEvent);
	beginpaint();
	return 0;
}
void initgame()
{
	memset(jerrysprite, NULL, sizeof(jerrysprite));
	memset(dogsprite, NULL, sizeof(dogsprite));
	memset(heartsprite, NULL, sizeof(heartsprite));
	score1 = 0;
	score2 = 8;
	jerryNum = 0;
	dogNum = 0;
	heartNum = 0;
	preJerryNum = 0;
	preDogNum = 0;
	preHeartNum = 0;
	jerrysprite[jerryNum] = (struct sprite*)malloc(sizeof(struct sprite));
	jerrysprite[jerryNum]->distx = jerrysprite[jerryNum]->disty = 3;
	jerrysprite[jerryNum]->height = 50;
	jerrysprite[jerryNum]->width = 50;
	jerrysprite[jerryNum]->x = rand() % (winWidth - jerrysprite[jerryNum]->width + 1);
	jerrysprite[jerryNum]->y = rand() % (winHeight - jerrysprite[jerryNum]->height + 1);
	tomsprite.width = 100;
	tomsprite.height = 100;
	tomsprite.distx = tomsprite.disty = 5;
	tomsprite.x = rand() % (winWidth - tomsprite.width + 1);
	tomsprite.y = rand() % (winHeight - tomsprite.height + 1);
}
void keyEvent(int key, int e)
{
	if (e != )return;
	if (key == 0x52)
	{
		initgame();
		flag = 1;
		Paint();
		startTimer(moveid, movetimerInterval);
		startTimer(createid, createtimerInterval);
		startTimer(dogid1, dogtimerInterval1);
		startTimer(dogid2, dogtimerInterval2);
		startTimer(heartid1, hearttimerInterval1);
		startTimer(heartid2, hearttimerInterval2);
	}
	else if (flag == 1)
	{
		switch (key)
		{
		case VK_UP:
			tomsprite.y -= tomsprite.disty;
			if (tomsprite.y <= 0)tomsprite.y = 0;
			break;
		case VK_DOWN:
			tomsprite.y += tomsprite.disty;
			if (tomsprite.y >= winHeight - tomsprite.height)tomsprite.y = winHeight - tomsprite.height;
			break;
		case VK_RIGHT:
			tomsprite.x += tomsprite.distx;
			if (tomsprite.x >= winWidth - tomsprite.width)tomsprite.x = winWidth - tomsprite.width;
			break;
		case VK_LEFT:
			tomsprite.x -= tomsprite.distx;
			if (tomsprite.x <= 0)tomsprite.x = 0;
			break;
		default:break;
		}
		rect r1, r2, r3, r4;
		r1.x = tomsprite.x;
		r1.y = tomsprite.y;
		r1.w = tomsprite.width;
		r1.h = tomsprite.height;
		for (int i = 0; i <= jerryNum; ++i)
		{
			if (jerrysprite[i])
			{
				r2.x = jerrysprite[i]->x;
				r2.y = jerrysprite[i]->y;
				r2.w = jerrysprite[i]->width;
				r2.h = jerrysprite[i]->height;
				int c = collision(r1, r2);
				if (c)
				{
					playSound(sound, 0);
					score1++;
					change(score1);
					if (score1 >= 10)
					{
						flag = 0;
						endpaint();
						cancelTimer(moveid);
						cancelTimer(createid);
						cancelTimer(dogid1);
						cancelTimer(dogid2);
						cancelTimer(heartid1);
						cancelTimer(heartid2);
						return;
					}
					preJerryNum--;
					delete(jerrysprite[i]);
					jerrysprite[i] = NULL;
				}
			}
		}
		for (int i = 1; i <= dogNum; ++i)
		{
			if (dogsprite[i])
			{
				r3.x = dogsprite[i]->x;
				r3.y = dogsprite[i]->y;
				r3.w = dogsprite[i]->width;
				r3.h = dogsprite[i]->height;
				int c = collision(r1, r3);
				if (c)
				{
					playSound(sound, 0);
					score2--;
					if (score1 >= 3)
						score1 -= 3;
					else score1 = 0;
					change(score1);
					if (score2 <= 0)
					{
						flag = 0;
						endpaint();
						cancelTimer(moveid);
						cancelTimer(createid);
						cancelTimer(dogid1);
						cancelTimer(dogid2);
						cancelTimer(heartid1);
						cancelTimer(heartid2);
						return;

					}
					preDogNum--;
					delete(dogsprite[i]);
					dogsprite[i] = NULL;
				}
			}
		}
		for (int i = 1; i <= heartNum; ++i)
		{
			if (heartsprite[i])
			{
				r4.x = heartsprite[i]->x;
				r4.y = heartsprite[i]->y;
				r4.w = heartsprite[i]->width;
				r4.h = heartsprite[i]->height;
				int c = collision(r1, r4);
				if (c)
				{
					playSound(sound, 0);
					score2++;
					change(score1);
					preHeartNum--;
					delete(heartsprite[i]);
					heartsprite[i] = NULL;
				}
			}
		}
		Paint();
	}
}
void timerEvent(int id)
{
	int a;
	if (id == 0)
	{
		for (a = 0; a <= jerryNum; a++)
		{
			if (jerrysprite[a])
			{
				if (jerrysprite[a]->x >= winWidth - jerrysprite[a]->width || jerrysprite[a]->x <= 0) { jerrysprite[a]->distx *= -1; }
				if (jerrysprite[a]->y >= winHeight - jerrysprite[a]->height || jerrysprite[a]->y <= 0) { jerrysprite[a]->disty *= -1; }
				jerrysprite[a]->x += jerrysprite[a]->distx;
				jerrysprite[a]->y += jerrysprite[a]->disty;
			}
		}
		//Paint();
	}
	if (id == 1)
	{
		if (preJerryNum < maxJerryNum) { jerryNum++; preJerryNum++; }
		if (preJerryNum < maxJerryNum && jerrysprite[jerryNum] == NULL)
		{
			jerrysprite[jerryNum] = (struct sprite*)malloc(sizeof(struct sprite));
			jerrysprite[jerryNum]->width = 50;
			jerrysprite[jerryNum]->height = 50;
			jerrysprite[jerryNum]->distx = jerrysprite[jerryNum]->disty = 3;
			jerrysprite[jerryNum]->x = rand() % (winWidth - jerrysprite[jerryNum]->width+1);
			jerrysprite[jerryNum]->y = rand() % (winHeight - jerrysprite[jerryNum]->height+1);
		}
	}
	if (id == 2)
	{
		if (preDogNum < maxDogNum) { dogNum++; preDogNum++; }
		if (preDogNum < maxDogNum && dogsprite[dogNum] == NULL)
		{
			dogsprite[dogNum] = (struct sprite*)malloc(sizeof(struct sprite));
			dogsprite[dogNum]->width = 100;
			dogsprite[dogNum]->height = 100;
			dogsprite[dogNum]->distx = dogsprite[dogNum]->disty = 2;
			dogsprite[dogNum]->x = rand() % (winWidth - dogsprite[dogNum]->width+1);
			dogsprite[dogNum]->y = rand() % (winHeight - dogsprite[dogNum]->height+1);
		}
	}
	if (id == 3)
	{
		for (a = 1; a <= dogNum; a++)
		{
			if (dogsprite[a])
			{
				if (dogsprite[a]->x >= winWidth - dogsprite[a]->width || dogsprite[a]->x <= 0) { dogsprite[a]->distx *= -1; }
				if (dogsprite[a]->y >= winHeight - dogsprite[a]->height || dogsprite[a]->y <= 0) { dogsprite[a]->disty *= -1; }
				dogsprite[a]->x += dogsprite[a]->distx;
				dogsprite[a]->y += dogsprite[a]->disty;
			}
		}
		//Paint();
	}
	if (id == 4)
	{
		for (a = 1; a <= heartNum; a++)
		{
			if (heartsprite[a])
			{
				if (heartsprite[a]->x >= winWidth - heartsprite[a]->width || heartsprite[a]->x <= 0) { heartsprite[a]->distx *= -1; }
				if (heartsprite[a]->y >= winHeight - heartsprite[a]->height || heartsprite[a]->y <= 0) { heartsprite[a]->disty *= -1; }
				heartsprite[a]->x += heartsprite[a]->distx;
				heartsprite[a]->y += heartsprite[a]->disty;
			}
		}
		//Paint();
	}
	if (id == 5)
	{
		if (preHeartNum < maxHeartNum) {
			heartNum++; preHeartNum++;
		}
		if (preHeartNum < maxNum && heartsprite[heartNum] == NULL)
		{
			heartsprite[heartNum] = (struct sprite*)malloc(sizeof(struct sprite));
			heartsprite[heartNum]->width = 50;
			heartsprite[heartNum]->height = 50;
			heartsprite[heartNum]->distx = heartsprite[heartNum]->disty = 1;
			heartsprite[heartNum]->x = rand() % (winWidth - heartsprite[heartNum]->width + 1);
			heartsprite[heartNum]->y = rand() % (winHeight - heartsprite[heartNum]->height + 1);
		}
	}
	rect r1, r2,r3,r4;
	r1.x = tomsprite.x;
	r1.y = tomsprite.y;
	r1.w = tomsprite.width;
	r1.h = tomsprite.height;
	for (int i = 0; i <= jerryNum; ++i)
	{
		if (jerrysprite[i])
		{
			r2.x = jerrysprite[i]->x;
			r2.y = jerrysprite[i]->y;
			r2.w = jerrysprite[i]->width;
			r2.h = jerrysprite[i]->height;
			int c = collision(r1, r2);
			if (c)
			{
				playSound(sound, 0);
				score1++;
				change(score1);
				if (score1 >= 10)
				{
					flag = 0;
					endpaint();
					cancelTimer(moveid);
					cancelTimer(createid);
					cancelTimer(dogid1);
					cancelTimer(dogid2);
					cancelTimer(heartid1);
					cancelTimer(heartid2);
					return;
				}
				preJerryNum--;
				delete(jerrysprite[i]);
				jerrysprite[i] = NULL;
			}
		}
	}
	for (int i = 1; i <= dogNum; ++i)
	{
		if (dogsprite[i])
		{
			r3.x = dogsprite[i]->x;
			r3.y = dogsprite[i]->y;
			r3.w = dogsprite[i]->width;
			r3.h = dogsprite[i]->height;
			int c = collision(r1, r3);
			if (c)
			{
				playSound(sound, 0);
				score2--;
				if (score1 >= 3)
					score1 -= 3;
				else score1 = 0;
				change(score1);
				if (score2 <= 0)
				{
					flag = 0;
					endpaint();
					cancelTimer(moveid);
					cancelTimer(createid);
					cancelTimer(dogid1);
					cancelTimer(dogid2);
					cancelTimer(heartid1);
					cancelTimer(heartid2);
					return;

				}
				preDogNum--;
				delete(dogsprite[i]);
				dogsprite[i] = NULL;
			}
		}
	}
	for (int i = 1; i <= heartNum; ++i)
	{
		if (heartsprite[i])
		{
			r4.x = heartsprite[i]->x;
			r4.y = heartsprite[i]->y;
			r4.w = heartsprite[i]->width;
			r4.h = heartsprite[i]->height;
			int c = collision(r1, r4);
			if (c)
			{
				playSound(sound, 0);
				score2++;
				change(score1);
				preHeartNum--;
				delete(heartsprite[i]);
				heartsprite[i] = NULL;
			}
		}
	}
	Paint();
}
void beginpaint()
{
	beginPaint();
	clearDevice();
	putImageScale(&beginimg, 0, 0, winWidth, winHeight);
	setTextSize(60);
	setTextColor(BLUE);
	setTextBkColor(RGB(64,224,208));
	setTextFont("楷体");
	paintText(300, 10, "按R开始游戏");
	endPaint();
}
void Paint()
{
	beginPaint();
	//putImage(&img, 10, 10);
	clearDevice();
	putImageTransparent(&tomimg, tomsprite.x, tomsprite.y, tomsprite.width, tomsprite.height,ACL_Color WHITE);
	//putImageScale(&jerryimg, jerry_x, jerry_y, jerryWidth, jerryHeight);
	for (int a = 0; a <= jerryNum; a++)
	{
		if (jerrysprite[a])
		{
			putImageTransparent(&jerryimg, jerrysprite[a]->x, jerrysprite[a]->y, jerrysprite[a]->width, jerrysprite[a]->height,ACL_Color WHITE);
		}
	}
	for (int a = 1; a <= dogNum; a++)
	{
		if (dogsprite[a])
		{
			putImageTransparent(&dogimg, dogsprite[a]->x, dogsprite[a]->y, dogsprite[a]->width, dogsprite[a]->height,ACL_Color WHITE);
		}
	}
	for (int a = 1; a <= heartNum; a++)
	{
		if (heartsprite[a])
		{
			putImageTransparent(&heartimg, heartsprite[a]->x, heartsprite[a]->y, heartsprite[a]->width, heartsprite[a]->height,ACL_Color WHITE);
		}
	}
	char str1[50],str2[50];
	sprintf_s(str1, "score:%d", score1);
	sprintf_s(str2, "HP:%d", score2);
	setTextSize(30);
	setTextBkColor(RGB(255, 255, 255));
	paintText(10, 10, str1);
	paintText(200, 10, str2);
	endPaint();
}
void endpaint()
{
	beginPaint();
	clearDevice();
	putImageScale(&endimg, 0, 0, winWidth, winHeight);
	setTextBkColor(RGB(255, 255, 255));
	paintText(360, 10, "按R重新开始游戏");
	endPaint();
}
int collision(rect r1, rect r2) {
	int c = 1;
	if (r1.x < r2.x && r1.x + r1.w >r2.x) {
		if (r1.y > r2.y && r1.y < r2.y + r2.h)return c;
		if (r1.y <r2.y && r1.y + r1.h >r2.y)return c;
	}
	else {
		if (r1.x > r2.x && r2.x + r2.w > r1.x)
		{
			if (r1.y > r2.y && r1.y < r2.y + r2.h)return c;
			if (r1.y <r2.y && r1.y + r1.h >r2.y)return c;
		}
	}
	c = 0;
	return c;
}
void change(int score)
{
	int d = score / 8;
	tomsprite.height = 100 + d * 20;
	tomsprite.width = 100 + d * 20;
}
