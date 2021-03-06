// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<malloc.h>
#define MaxSize 13   //电梯所能承载的最大人数
#define N 13
typedef enum {
	Up, 
	Down,
	Free
} StateofElevator; //电梯的状态

typedef struct    
 {
	int NowFloor;         //乘客所在楼层
	int GoFloor;          //乘客要去楼层
	int Patience;         //乘客的最大容忍等待时间
	int Direction;        //方向
	int InorOut;          //乘客是否已出电梯
	int Leave;            //乘客是否离去
	int WaitedTime;       //乘客已经等待的时间
}Person;

typedef struct  
{
	int Floor;                //所在楼层
	StateofElevator State;    //运行方向状态
	int NumberofPerson;       //装载乘客人数
}Elevator;

Person Passenger[9][N];  //全局结构体二维数组保存整栋楼的乘客的相关信息

Person PassengerInElevator[MaxSize];   //在电梯中的乘客的相关信息存储在结构体一维数组中

int b[9];            //每层楼的人数
int InCount[9];      //每层楼进入电梯的实际人数
int OutCount[9];     //每层楼走出电梯的实际人数
int tclock = 0;      //电梯运行时的全局软时钟
int InTime[9];       //每层楼乘客进入电梯的时间
int OutTime[9];      //每层楼乘客出电梯的时间

int In(int y, int x, StateofElevator State1)      //乘客进入电梯,每次乘客进入电梯后将该层的乘客重新排序( y表示电梯当前人数；x表示楼层)
{
	int m = y;           //保存电梯当前人数
	int p, q, l, v = 0, et;
	int d = 1;
	if (State1 == Up)       
		d = 1;
	if (State1 == Down)
		d = 0;
	for (p = 0; p<b[x]; p++)
	{
		if (Passenger[x][p].Direction == d && Passenger[x][p].Leave == 0)
			//该x层乘客p的方向与电梯方向一致且未离开，若电梯未满，则进电梯
		{
			if (m == MaxSize)
			{
				printf("电梯现在已经超载，请您等待片刻\n");
				break;                 //该楼层的其他乘客等待一段时间
			}
			m++;                        //电梯内人数增加
			for (q = 0; q<MaxSize; q++)
			{
				if (PassengerInElevator[q].InorOut == 0)
					//如果电梯内乘客q已出电梯，则乘客p入电梯
				{
					PassengerInElevator[q] = Passenger[x][p];
					//乘客正式进入电梯
					PassengerInElevator[q].InorOut = 1;
					//乘客已经进入电梯
					PassengerInElevator[q].Leave = 1;
					PassengerInElevator[q].WaitedTime = 0;
					PassengerInElevator[q].Patience = 0;
					Passenger[x][p].Direction = -2;
					Passenger[x][p].NowFloor = -2;
					Passenger[x][p].InorOut = -2;
					Passenger[x][p].Leave = -2;					
					Passenger[x][p].WaitedTime = 0;
					Passenger[x][p].Patience = 0;
					//进了电梯表明乘客即可到达目的地从而离开系统
					break;
				}
			}
		}
	}
	InCount[x] = m - y;                //每层楼进入电梯的实际人数
									   /*将本楼层的乘客信息重新排列，将该层剩余乘客信息放入全局结构体二维数组*/
	Person *temp = (Person *)malloc((b[x] - InCount[x]) * sizeof(Person));  //临时数组

	for (et = 0; et<(b[x] - InCount[x]); et++)//电梯外剩余乘客 
	{
		Passenger[x][et] = temp[et];
	}
	b[x] = b[x] - InCount[x];
	InTime[x] = InCount[x] * 25;
	tclock += InTime[x];
	printf("在%dt时刻第%d楼层有%d个人进入电梯\n", tclock, x, (m - y));
	//printf("目标楼层\n", ) 
	return m;
}

int Out(int floor, int number)    //乘客出电梯，每次乘客出电梯将全局数组中的乘客信息重新修改，floor表示楼层，number表示电梯内人数
{
	int i;
	int k = number;//电梯内剩余的人 
	for (i = 0; i<MaxSize; i++)
	{
		if (PassengerInElevator[i].GoFloor == floor && PassengerInElevator[i].InorOut == 1 && PassengerInElevator[i].Leave == 1)
		{
			k--;
			PassengerInElevator[i].InorOut = 0;      //到站的乘客出电梯

		}
	}
	OutCount[floor] = number - k;
	OutTime[floor] = OutCount[floor] * 25;
	tclock += OutTime[floor];
	printf("在%dt时刻第%d楼层有%d个人已经到达\n", tclock, floor, OutCount[floor]);
	return k;
}

void Open()        
{
	tclock += 20;
	printf("电梯的门正在打开，请您稍候\n");
	printf("在%dt时刻电梯的门已经打开\n", tclock);
}

void Close()     
{
	tclock += 20;
	printf("电梯的门正在关闭，请您稍候\n");
	printf("在%dt时刻电梯的门已经关闭\n", tclock);
}

int Go(Elevator *ele)           //电梯正在运行
{
	Elevator *e = ele;
	if (e->State == Up)
	{
		printf("电梯正在向上运行\n");
		printf("\n");
		e->Floor++;
	}
	if (e->State == Down)
	{
		e->Floor--;
		printf("电梯正在向下运行\n");
		printf("\n");
	}
	tclock += 10;
	return (e->Floor);
}

int Initialize()            //系统最开始的初始化
{
	int i, j, k, m, n, s, c, u, v = 0;
	srand(time(0));                 //获取时间种子
	for (i = 0; i <= 8; i++)        //每层楼的人数随机初始化
		b[i] = rand() % N;
	for (m = 0; m<b[0]; m++)      //最底层楼的乘客信息初始化
		Passenger[0][m].Direction = 1; 
	    Passenger[0][m].GoFloor = 1 + rand() % 7;
	for (c = 0; c<b[8]; c++)      //最高层的乘客信息初始化
		Passenger[8][c].Direction = 0;
	    Passenger[8][c].GoFloor = -1 + rand() % 8;
	for (n = 1; n <= 7; n++)        //每层楼每个乘客方向初始化
		for (s = 0; s<b[n]; s++)
			Passenger[n][s].Direction = rand() % 2;
	for (j = 0; j <= 8; j++)    //每层楼的乘客的相关信息随机初始化
		for (k = 0; k<b[j]; k++)
		{
			Passenger[j][k].NowFloor = j;
			Passenger[j][k].Patience = 100 + rand() % 100;
			Passenger[j][k].InorOut = 0;               //电梯外
			Passenger[j][k].Leave = 0;                 //未离开
			Passenger[j][k].WaitedTime = 0;
			if (Passenger[j][k].Direction == 1)
				Passenger[j][k].GoFloor = (j + 1) + rand() % (8 - j);
			if (Passenger[j][k].Direction == 0 && j != 1)
				Passenger[j][k].GoFloor = rand() % (j - 1);
			if (Passenger[j][k].Direction == 0 && j == 1)
				Passenger[j][k].GoFloor = 0;
		}
	for (u = 0; u <= 8; u++)      // 遍历整栋楼，有人则返回1，没人返回0；
		if (b[u] == 0)
			v++;
	if (v == 9)
		return 0;
	else
		return 1;
}

int Test()             // 考查整栋楼有无请求，无返回0，有返回1；
{
	int cs, cf = 0;
	for (cs = 0; cs<9; cs++)
		if (b[cs] == 0)
			cf++;
	if (cf == 9)
		return 0;
	else
		return 1;
}

void OnFree()
{
	printf("电梯现在在第一楼!电梯正处于空闲状态\n");
}

void Run(Elevator *left)
{
	Elevator *ts = left;
	int floor = ts->Floor;             //保存电梯当前的所在楼层数
	int number = ts->NumberofPerson;   //保存电梯当前的人数
	int ps, ms, ns, bs = 0, ls = 0, pt, yt;
	int loop = 0;
	//循环次数
	while (loop != 3)
	{
		if (Test() == 0)                //考查整栋楼有无请求 ，无返回0，有返回1;
		{
			OnFree();                   //输出电梯现在在第一楼!电梯正处于空闲状态!       
		}
		else
		{
			bs = 0;
			printf("在%dt时刻电梯正在第%d层\n", tclock, floor);
			for (ns = 0; ns<number; ns++)
				if (PassengerInElevator[ns].GoFloor != floor)
					bs++;
			if (b[floor] == 0 && bs == number)    //电梯外没人，没人下
			{
				//电梯不开门，继续上升或下降
				if (number != 0)    //电梯里有人
				{
					if (PassengerInElevator[0].Direction == 1 && floor<8)
						//往上
					{
						ts->State = Up;
						floor = Go(ts);      //电梯移动
						if (floor == 9)
						{
							printf("在%dt时刻电梯正在%d楼层\n", tclock, floor);
							Open();
							number = Out(floor, number);             //乘客出电梯
							OutTime[floor] = OutCount[floor] * 25;   //乘客出电梯的耗时
							ts->State = Down;
							number = In(number, floor, ts->State);
							InTime[floor] = InCount[floor] * 25;      //乘客进入电梯的耗时
							Close();							
							for (ps = 0; ps <= 9; ps++)//重新计算每层楼的正在等待的乘客的需等待时间
								for (ms = 0; ms<b[ps]; ms++)
								{
									if (Passenger[ps][ms].InorOut == 0 && Passenger[ps][ms].Leave == 0)
										Passenger[ps][ms].WaitedTime += (50 + OutTime[floor] + InTime[floor]);
								}
							if (number == 0)
								floor = Go(ts);
						}
					}
					if (PassengerInElevator[0].Direction == 0 && floor>0)  //往下
					{
						ts->State = Down;
						floor = Go(ts);
						if (floor == 0)
						{
							printf("在%dt时刻电梯正在%d楼层\n", tclock, floor);
							Open();
							number = Out(floor, number);  //乘客出电梯
							OutTime[floor] = OutCount[floor] * 25;   //乘客出电梯的耗时
							ts->State = Up;
							number = In(number, floor, ts->State);
							InTime[floor] = InCount[floor] * 25;  //乘客进入电梯的耗时
							Close();							
							for (ps = 0; ps <= 9; ps++)///*重新计算每层楼的正在等待的乘客的需等待时间
								for (ms = 0; ms<b[ps]; ms++)
								{
									if (Passenger[ps][ms].InorOut == 0 && Passenger[ps][ms].Leave == 0)
										Passenger[ps][ms].WaitedTime += (50 + OutTime[floor] + InTime[floor]);
								}
							floor = Go(ts);
						}
					}
				}
				else   //电梯里没有人
				{
					if (ts->State == Up)
					{
						if (floor == 8)
							ts->State = Down;
						else
							floor = Go(ts);
					}
					if (ts->State == Down)
					{
						if (floor == 0)
							ts->State = Up;
						else
							floor = Go(ts);
					}
				}
				for (pt = 0; pt <= 9; pt++)
					for (yt = 0; yt<b[pt]; yt++)
					{
						if (Passenger[pt][yt].InorOut == 0 && Passenger[pt][yt].Leave == 0)
							Passenger[pt][yt].WaitedTime += 10;
					}
			}
			else
			{
				Open();
				number = Out(floor, number);     //乘客出电梯
				OutTime[floor] = OutCount[floor] * 25; //乘客出电梯的耗时
				number = In(number, floor, ts->State);
				InTime[floor] = InCount[floor] * 25;  //乘客进入电梯的耗时
				Close();
				/*重新计算每层楼的正在等待的乘客的需等待时间*/
				for (ps = 0; ps <= 9; ps++)
					for (ms = 0; ms<b[ps]; ms++)
					{
						if (Passenger[ps][ms].InorOut == 0 && Passenger[ps][ms].Leave == 0)		
							Passenger[ps][ms].WaitedTime += (50 + OutTime[floor] + InTime[floor]);
					}
				/*服务完本层，电梯正式运行*/	
				if (PassengerInElevator[0].Direction == 1 && floor<9)//向上
				{
					ts->State = Up;
					floor = Go(ts);
					if (floor == 9)
					{
						printf("在%dt时刻电梯正在%d楼层\n", tclock, floor);
						Open();
						number = Out(floor, number);   //乘客出电梯
						OutTime[floor] = OutCount[floor] * 25;     //乘客出电梯的耗时
						ts->State = Down;
						number = In(number, floor, ts->State);
						InTime[floor] = InCount[floor] * 25; //乘客进入电梯的耗时
						Close();
						/*重新计算每层楼的正在等待的乘客的需等待时间*/
						for (ps = 0; ps <= 9; ps++)
							for (ms = 0; ms<b[ps]; ms++)
							{
								if (Passenger[ps][ms].InorOut == 0 && Passenger[ps][ms].Leave == 0)
									Passenger[ps][ms].WaitedTime += (50 + OutTime[floor] + InTime[floor]);
							}
						if (number == 0)
							floor = Go(ts);
					}
				}
				if (PassengerInElevator[0].Direction == 0 && floor>0)
				{
					ts->State = Down;
					floor = Go(ts);
					if (floor == 0)
					{
						printf("在%dt时刻电梯正在%d楼层\n", tclock, floor);
						Open();
						number = Out(floor, number);   //乘客出电梯
						OutTime[floor] = OutCount[floor] * 25; //乘客出电梯的耗时
						ts->State = Up;
						number = In(number, floor, ts->State);
						InTime[floor] = InCount[floor] * 25;  //乘客进入电梯的耗时  
						Close();
						/*重新计算每层楼的正在等待的乘客的需等待时间*/
						for (ps = 0; ps <= 9; ps++)
							for (ms = 0; ms<b[ps]; ms++)
							{
								if (Passenger[ps][ms].InorOut == 0 && Passenger[ps][ms].Leave == 0)
									Passenger[ps][ms].WaitedTime += (50 + OutTime[floor] + InTime[floor]);
							}
						floor = Go(ts);
					}
				}
			}
		}
		//乘客向电梯请求
		loop++;
	}
}

int main()
{
	int number, floor, state;
	Elevator left;
	left.State = Free;                   //电梯初始化
	left.Floor = 1;
	left.NumberofPerson = 1;
	for (number = 1; number<MaxSize; number++)                 //电梯内乘客信息初始化
	{
		PassengerInElevator[number].InorOut = 0;
		PassengerInElevator[number].Direction = -1;
		PassengerInElevator[number].NowFloor = -1;
		PassengerInElevator[number].Leave = 0;
		PassengerInElevator[number].GoFloor = int(rand);
		PassengerInElevator[number].WaitedTime = 0;
		PassengerInElevator[number].Patience = 0;
	}
	for (floor = 1; floor <= 9; floor++)
	{
		InCount[floor] = 0;
		OutCount[floor] = 0;
		InTime[floor] = 0;
		OutTime[floor] = 0;
	}
	state = Initialize();
	if (state == 0)
		OnFree();
	else                      // 判断电梯最初运行方向
	{
		if (b[1] == 0)           // 1楼没人，而整栋楼有人
		{
			left.State = Up;
			Run(&left);
		}
		else
		{
			if (Passenger[1][0].Direction == 1)
			{
				left.State = Up;
				Run(&left);
			}
			if (Passenger[1][0].Direction == 0)
			{
				left.State = Down;
				Run(&left);
			}
		}
	}
}
