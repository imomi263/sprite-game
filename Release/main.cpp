#include <acllib.h>
#include<time.h>
#include<malloc.h>
#include<stdio.h>
#include<string.h>
#include<cmath>
using namespace std;
ACL_Image begin;
ACL_Image end;
ACL_Image tom1;
ACL_Image tom2;
ACL_Image jerry;
ACL_Image dog;
#define null NULL
const int Width = 1000, Height =700;
const int MAX = 20;
int flag = 0; //flag=1 normal game flag=2 tom crazy flag=3 dog come
int jerrynum = 0;
int tomnum = 0;
int dognum = 0;
/*
	由于dog一开始是没有的所以dog是先赋值在dognum++;所以dognum循环的时候为dognum-1;
	jerrynum是从0开始的一开始就有一个值，所以循环时就是为jerrynum
*/
int score = 0;
int jerryid = 1;
int jerryInterval = 10;
int tomid = 2;
int tomInterval = 5;
int dogid = 3;
int dogInterval = 13;
void timerEvent(int id);//定时器
void keyEvent(int key, int e);//键盘
void init();//初始化函数
void beginpaint();
void endpaint();
void judge();
void Paint();
int jerrytime = 0;
// https://zouzhongliang.com/index.php/2019/04/04/acllib%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C/
class Sprite {
public:
	Sprite() {
		x = y = width = height = distx = disty = 0;
	}
	Sprite(int a1,int a2,int a3,int a4,int a5,int a6) {
		x = a1; y = a2; width = a3; height = a4;
		distx = a5; disty = a6;
	}
	int x;
	int y;
	int width, height;
	int distx, disty;
	int movex;
	int movey;
	virtual void move() {

	}
	Sprite operator&=(Sprite& a) {
		this->x = a.x;
		this->y = a.y;
		this->distx = a.x;
		this->disty = a.y;
		this->height = a.height;
		this->width = a.width;
		return *this;
	}
};
class DogSprite  :public Sprite{
public:
	/*int width = 50;
	int height = 50;
	int distx = 5;
	int disty = 5;*/
	void move() {
		if (x + movex * distx + width >= Width)
		{
			x = x - movex * distx;
			movex = movex * (-1);
		}
		else if (x + movex * distx <= 0) {
			x = x - movex * distx;
			movex = movex * (-1);
		}
		if (y + movey * disty + height >= Height)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
		if (y + movey * disty <= 0)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
	}
};
class JerrySprite :public Sprite {
public:
	JerrySprite() {
		 this->width = 100;
		height = 100;
		 distx = 5;
		 disty = 5;
	}
	/*int width = 100;
	int height = 100;
	int distx = 5;
	int disty = 5;*/
	// 这里可以写遇到的困难，new出来的对象多态之间的x区分不好
	void move() {
		if (x + movex * distx + width >= Width)
		{
			x = x - movex * distx;
			movex = movex * (-1);
		}
		else if (x + movex * distx <= 0) {
			x = x - movex * distx;
			movex = movex * (-1);
		}
		else x = x + movex * distx;
		if (y + movey * disty + height >= Height)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
		else if (y + movey * disty <= 0)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
		else y = y + movey * disty;
		Paint();
	}
};
class TomSprite :public Sprite {
public:
	int width = 100;
	int height = 100;
	int distx = 5;
	int disty = 5;
	void move() {
		if (x + movex * distx + width >= Width)
		{
			x = x - movex * distx;
			movex = movex * (-1);
		}
		else if (x + movex * distx <= 0) {
			x = x - movex * distx;
			movex = movex * (-1);
		}
		if (y + movey * disty + height >= Height)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
		if (y + movey * disty <= 0)
		{
			y = y - movey * disty;
			movey = movey * (-1);
		}
	}
};
void add_dog();
void add_jerry();
bool collision(Sprite A, Sprite B);
Sprite* jerrysprite[MAX],  *dogsprite[MAX];
TomSprite tomsprite;
int Setup()
{
	initWindow("Tom and Jerry", DEFAULT, DEFAULT, Width, Height);
	loadImage("jerry1.jpg", &jerry);
	loadImage("tom1.bmp", &tom1);
	loadImage("tom2.bmp", &tom2);
	loadImage("dog.bmp", &dog);
	loadImage("begin.bmp", &begin);
	loadImage("end.bmp", &end);
//	loadImage("jerry.bmp", &jerry);  dont know why load fails
	srand((unsigned)time(NULL));
	init();
	registerTimerEvent(timerEvent);
	registerKeyboardEvent(keyEvent);
	beginpaint();
	return 0;
}
void init() {
	memset(jerrysprite, NULL, sizeof(jerrysprite));
	memset(dogsprite, NULL, sizeof(dogsprite));
	jerrynum = tomnum = dognum = score=0;
	jerrysprite[jerrynum] = new JerrySprite;
	jerrysprite[jerrynum]->x = rand() % (Width - jerrysprite[jerrynum]->width + 2);
	jerrysprite[jerrynum]->y = rand() % (Height - jerrysprite[jerrynum]->height + 2);
	jerrysprite[jerrynum]->movex = rand() % 3 - 1;
	jerrysprite[jerrynum]->movey = rand() % 3 - 1;
	tomsprite.width = 100;
	tomsprite.height = 100;
	tomsprite.distx = tomsprite.disty = 15;
	tomsprite.x = rand() % (Width - tomsprite.width + 1);
	tomsprite.y = rand() % (Height - tomsprite.height + 1);
	tomsprite.movex = rand() % 3 - 1;
	tomsprite.movey = rand() % 3 - 1;
	if (jerrysprite[jerrynum]->movex == 0 && jerrysprite[jerrynum]->movey == 0) {
		jerrysprite[jerrynum]->movex = 1;
		jerrysprite[jerrynum]->movex = 1;
	}
}
void timerEvent(int id)//定时器
{
	judge();
	if (flag == 0)return;
	if (id == 1) {
		if (jerrynum == 20) {
			jerrytime = 0;
		}
		if (jerrytime == 100) {
			add_jerry();
			jerrytime = 0;
		}
		else {
			jerrytime++;
		}
		for (int i = 0; i <= jerrynum; i++) {
			if (jerrysprite[i]) {
				jerrysprite[i]->move();
			}
		}
	}
	if (id == 2) {
		tomsprite.move();
	}
	if (id == 3) {
		for (int i = 0; i <= dognum; i++) {
			if (dogsprite[i]) {
				dogsprite[i]->move();
			}
		}
	}
	judge();
}
void keyEvent(int key, int event) {
	if (key == 82&&flag==0)
	{
		init();
		flag = 1;
		Paint();
		startTimer(jerryid, jerryInterval);
		startTimer(tomid, tomInterval);
		//startTimer(dogid, dogInterval);
	}
	if (flag == 1) {
		switch (key)
		{
			case 82:
				flag = 1;
				break;
			case VK_UP:
				tomsprite.y -= tomsprite.disty;
				if (tomsprite.y <= 0)tomsprite.y = 0;
				break;
			case VK_DOWN:
				tomsprite.y += tomsprite.disty;
				if (tomsprite.y >= Height - tomsprite.height)tomsprite.y = Height - tomsprite.height;
				break;
			case VK_RIGHT:
				tomsprite.x += tomsprite.distx;
				if (tomsprite.x >= Width - tomsprite.width)tomsprite.x = Width - tomsprite.width;
				break;
			case VK_LEFT:
				tomsprite.x -= tomsprite.distx;
				if (tomsprite.x <= 0)tomsprite.x = 0;
				break;
			case 80:// P  add dog

				add_dog();
				Paint();
				break;
			case 81:// Q 
				Paint();
				break;
			default:break;
		}
		judge();
	}
}

void Paint()
{
	beginPaint();
	//putImage(&img, 10, 10);
	clearDevice();
	putImageTransparent(&tom1, tomsprite.x, tomsprite.y, tomsprite.width, tomsprite.height, ACL_Color WHITE);
	//putImageScale(&jerryimg, jerry_x, jerry_y, jerryWidth, jerryHeight);
	for (int a = 0; a <= jerrynum; a++)
	{
		if (jerrysprite[a])
		{
			putImageTransparent(&jerry, jerrysprite[a]->x, jerrysprite[a]->y, jerrysprite[a]->width, jerrysprite[a]->height, ACL_Color WHITE);
		}
	}
	for (int a = 0; a <= dognum; a++)
	{
		if (dogsprite[a])
		{
			putImageTransparent(&dog, dogsprite[a]->x, dogsprite[a]->y, dogsprite[a]->width, dogsprite[a]->height, ACL_Color WHITE);
		}
	}
	char str1[50];// str2[50];
	sprintf_s(str1, "score:%d", score);
	setTextSize(30);
	setTextBkColor(RGB(255, 255, 255));
	paintText(10, 10, str1);
	//paintText(200, 10, str2);
	endPaint();
}
void beginpaint() {
	beginPaint();
	clearDevice();
	putImageScale(&begin, 0, 0, Width, Height);
	setTextSize(60);
	setTextColor(BLUE);
	setTextBkColor(RGB(64, 224, 208));
	setTextFont("楷体");
	paintText(300, 10, "按R开始游戏");
	endPaint();
}
void endpaint()
{
	beginPaint();
	clearDevice();
	putImageScale(&end, 0, 0, Width, Height);
	setTextBkColor(RGB(255, 255, 255));
	paintText(360, 10, "按R重新开始游戏");
	endPaint();
}
bool collision(Sprite A, Sprite B) {
	int x1 = A.x + A.width / 2;
	int y1 = A.y + A.height / 2;
	int x2 = B.x + B.width / 2;
	int y2 = B.y + B.height / 2;
	int r1 = min(A.height, A.width);
	int r2 = min(B.height, B.width);
	if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < r1 + r2)return true;
	else return false;
}
void judge() {
	for (int i = 0; i < jerrynum; i++) {
		if (jerrysprite[i]) {
			if (collision(*jerrysprite[i], tomsprite)) {
				score = score + 1;
				delete jerrysprite[i];
				jerrysprite[i] = jerrysprite[jerrynum];
				jerrysprite[jerrynum] = NULL;
				jerrynum--;
				Paint();
			}
		}
	}
	
	for (int i = 0; i < dognum; i++) {
		if (dogsprite[i]) {
			if (collision(*dogsprite[i], tomsprite)) {
				endpaint();
				flag = 0;
			}
		}
	}
	Paint();
}
void add_dog() {
	
	
	dogsprite[dognum] = new DogSprite();
	dogsprite[dognum]->movex = rand() % 3 - 1;
	dogsprite[dognum]->movey = rand() % 3 - 1;
	dogsprite[jerrynum]->x = rand() % (Width - jerrysprite[jerrynum]->width + 2);
	dogsprite[jerrynum]->y = rand() % (Height - jerrysprite[jerrynum]->height + 2);
	if (dogsprite[dognum]->movex == 0 && dogsprite[dognum]->movey==0) {
		dogsprite[dognum]->movex = 1;
		dogsprite[dognum]->movey = 1;
	}
	dognum++;
}
void add_jerry() {
	jerrynum++;
	jerrysprite[jerrynum] = new JerrySprite;
	jerrysprite[jerrynum]->x = rand() % (Width - jerrysprite[jerrynum]->width + 2);
	jerrysprite[jerrynum]->y = rand() % (Height - jerrysprite[jerrynum]->height + 2);
	jerrysprite[jerrynum]->movex = rand() % 3 - 1;
	jerrysprite[jerrynum]->movey = rand() % 3 - 1;
	if (jerrysprite[jerrynum]->movex == 0 && jerrysprite[jerrynum]->movey == 0) {
		jerrysprite[jerrynum]->movex = 1;
		jerrysprite[jerrynum]->movey = 1;
	}
}