#include<stdio.h>
#include<graphics.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define PI 3.1415926548
#define NUM 13//烟花种类数
struct FIRE {//烟花
	int r;//当前爆炸半径
	int max_r;//爆炸中心距离边缘最大半径
	int x, y;//爆炸中心在窗口的坐标
	int cx, cy;//爆炸中心相对图片左上角坐标
	int width, height;//图片
	int xy[240][240];//图片像素点
	bool boom;//是否绽放
	bool draw;//输出像素点
	DWORD t1, t2, dt;
}fire[NUM];
struct JET {
	int x, y;
	int hx, hy;
	int height;
	bool shoot;//是否发射
	DWORD t1, t2, dt;
	IMAGE img[2];
	byte n : 1;//图片下标
}jet[NUM];
void welcome() {
		settextcolor(YELLOW);
		for (int i = 0; i < 50; i++) {
			int x = 600 + int(180 * sin(PI * 2 * i / 60));
			int y = 200 + int(180 * cos(PI * 2 * i / 60));
			cleardevice();
			settextstyle(i, 0, "隶书");
			outtextxy(x - 80, y, "熊出没");
			outtextxy(x - 10, y + 100, "--雪岭熊风");
			Sleep(25);
		}
		Sleep(100);
		IMAGE amg[4];
		loadimage(&amg[1], "./团子001(1).jpg");
		loadimage(&amg[0], "./熊004(1).jpg");
		loadimage(&amg[2], "./熊出没雪.jpg");
		//loadimage(&amg[3], "./熊出没岭.jpg");
		putimage(0, 0, &amg[1]);
		Sleep(1000);
		putimage(500, 0, &amg[0]);
		Sleep(1000);
		putimage(700, 300, &amg[2]);
		//putimage(300, 300, &amg[3]);
		Sleep(1000);
		settextstyle(25, 0, "楷体");
		outtextxy(20, 300, "浩瀚星空里");
		Sleep(1000);
		outtextxy(20, 330, "只剩你的背影");
		Sleep(1000);
		outtextxy(20, 360, "银河已凝结成冰");
		Sleep(1000);
		outtextxy(20, 390, "只剩你的背影");
		Sleep(1000);
		outtextxy(20, 420, "记忆划过泪滴");
		Sleep(1000);
		outtextxy(20, 450, "想象能够回到过去");
		Sleep(1000);
		outtextxy(20, 480, "终会存在我心底");
		Sleep(1000);
		outtextxy(100, 510, "——你从未离去");
		Sleep(25);
	}
	
void Init(int i) {
	int r[13] = { 120,143,144,154,137,120,139,140,150,155,152,120,168 };
	int x[13] = { 120,106,129,130,103,120,123,130,120,101,98,103,99 };
	int y[13] = { 120,104,138,128,110,107,117,105,114,100,99,104,99 };
	fire[i].x = 0;
	fire[i].y= 0;
	fire[i].width = 240;
	fire[i].height = 240;
	fire[i].max_r = r[i];
	fire[i].cx = x[i];
	fire[i].cy = y[i];
	fire[i].boom = false;
	fire[i].dt = 5;//绽放间隔
	fire[i].t1 = timeGetTime();
	fire[i].r = 0;
	jet[i].x = -240;
	jet[i].y = -240;
	jet[i].hx = -240;
	jet[i].hy = -240;
	jet[i].height = 0;
	jet[i].t1 = timeGetTime();
	jet[i].dt = rand() % 10;
	jet[i].n = 0;
	jet[i].shoot = false;
}
void Load() {
	IMAGE fm, gm;
	loadimage(&fm, "flower(1).jpg", 3120, 240);
	for (int i = 0; i < 13; i++) {
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		SetWorkingImage(&gm);
		for(int a=0;a<240;a++)
			for (int b = 0; b < 240; b++) {
				fire[i].xy[a][b] = getpixel(a, b);
			}
	}
	IMAGE sm;
	loadimage(&sm, "shoot(1)(1).jpg", 200, 50);
	for (int i = 0; i < 13; i++) {
		SetWorkingImage(&sm);
		int n = rand() % 5;
		getimage(&jet[i].img[0], n * 20, 0, 20, 50);//暗
		getimage(&jet[i].img[1], (n+5) * 20, 0, 20, 50);//明
	}
	SetWorkingImage();
}
void Chose(DWORD& t1) {
	DWORD t2 = timeGetTime();
	if (t2 - t1 > 100){
		int n = rand() % 20;
		if (n < 13 && jet[n].shoot == false && fire[n].boom == false) {
			jet[n].x = rand() % 1200;
			jet[n].y = rand() % 100 + 600;
			jet[n].hx = jet[n].x;
			jet[n].hy = rand() % 400;
			jet[n].height = jet[n].y - jet[n].hy;
			jet[n].shoot = true;
			putimage(jet[n].x, jet[n].y, &jet[n].img[jet[n].n], SRCINVERT);
		}
		t1 = t2;
	}
}
void Shoot() {
	for (int i = 0; i < 13; i++) {
		jet[i].t2 = timeGetTime();
		if (jet[i].t2 - jet[i].t1 > jet[i].dt && jet[i].shoot == true) {
			putimage(jet[i].x, jet[i].y, &jet[i].img[jet[i].n], SRCINVERT);
			if (jet[i].y > jet[i].hy) {
				jet[i].n++;
				jet[i].y -= 5;
			}
			putimage(jet[i].x, jet[i].y, &jet[i].img[jet[i].n], SRCINVERT);
			if ((jet[i].y - jet[i].hy) * 4 < jet[i].height)
				jet[i].dt = rand() % 4 + 10;
			if (jet[i].y <= jet[i].hy) {
				putimage(jet[i].x, jet[i].y, &jet[i].img[jet[i].n], SRCINVERT);
				fire[i].x = jet[i].hx + 10;
				fire[i].y = jet[i].hy;
				fire[i].boom = true;
				jet[i].shoot = false;
			}
			jet[i].t1 = jet[i].t2;
		}
	}
}
void Show(DWORD* pmem) {
	int drt[16] = { 5,5,5,5,6,6,6,7,7,8,9,10,30,20,55,55 };
	for (int i = 0; i < 13; i++) {
		fire[i].t2 = timeGetTime();
		if (fire[i].t2 - fire[i].t1 > fire[i].dt && fire[i].boom == true) {
			if (fire[i].r < fire[i].max_r) {
				fire[i].r++;
				fire[i].dt = drt[fire[i].r / 10];
				fire[i].draw = true;
			}
			if (fire[i].r >= fire[i].max_r - 1) {
				fire[i].draw = false;
				Init(i);
			}
			fire[i].t1 = fire[i].t2;
		}
		if (fire[i].draw) {
			for (double a = 0; a <= 6.28; a += 0.01) {
				int x1 = (int)(fire[i].cx + fire[i].r * cos(a));
				int y1 = (int)(fire[i].cy - fire[i].r * sin(a));
				if (x1 > 0 && x1 < fire[i].width && y1>0 && y1 < fire[i].height) {
					int b = fire[i].xy[x1][y1] & 0xff;
					int g = (fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (fire[i].xy[x1][y1] >> 16);
					int xx = (int)(fire[i].x + fire[i].r * cos(a));
					int yy = (int)(fire[i].y - fire[i].r * sin(a));
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && yy > 0 && xx < 1200 && yy < 800)
						pmem[yy * 1200 + xx] = BGR(fire[i].xy[x1][y1]);
				}
			}
			fire[i].draw = false;
		}
	}
}
void Style(DWORD& st1) {
	DWORD st2 = timeGetTime();
	if (st2 - st1 > 20000) {
		int x[13] = { 68,37,37,98,89,56,2,87,76,52,76,73,36};
		int y[13] = { 43,76,42,31,32,14,34,13,21,46,24,21,14 };
		for (int i = 0; i < NUM; i++) {
			jet[i].x = x[i] * 10;
			jet[i].y = (y[i] + 75) * 10;
			jet[i].hx = jet[i].x;
			jet[i].hy = y[i] * 10;
			jet[i].height = jet[i].y - jet[i].hy;
			jet[i].shoot = true;
			jet[i].dt = 7;
			putimage(jet[i].x, jet[i].y, &jet[i].img[jet[i].n], SRCINVERT);
			fire[i].x = jet[i].x + 10;
			fire[i].y = jet[i].hy;
			fire[i].boom = false;
			fire[i].r = 0;
		}
		st1 = st2;
	}
}
void main() {
	initgraph(1200, 800);
	srand(time(0));
	mciSendString("open 你从未离去.mp3", 0, 0, 0);
	mciSendString("play 你从未离去.mp3", 0, 0, 0);
	welcome();
	getchar();
	DWORD t1 = timeGetTime();//筛选烟花计时
	DWORD st1 = timeGetTime();//播放烟花计时
	DWORD* pmem = GetImageBuffer();//获取窗口显存指针
	for (int i = 0; i < NUM; i++) {
		Init(i);
	}
	Load();
	BeginBatchDraw();
	while (!_kbhit()) {
		Sleep(10);
		for (int i = 0; i < 1000; i++) {
			for (int j = 0; j < 2; j++) {
				int px1 = rand() % 1200;
				int py1 = rand() % 800;
				if (py1 < 799) 
					pmem[py1 * 1200 + px1] = pmem[py1 * 1200 + px1 + 1] = BLACK;//对显存赋值擦除像素点
			}
		}
		Chose(t1);
		Shoot();
		Show(pmem);
		Style(st1);
		FlushBatchDraw();
	}
	
}