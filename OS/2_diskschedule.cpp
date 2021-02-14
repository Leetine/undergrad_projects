// 2_diskschedule.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<math.h>
#define N 14

void main()
{
	//初始化
	int track[N], track2[N + 1];//开辟两个数组空间
	int i;
	track[0] = 30;
	track[1] = 50;
	track[2] = 100;
	track[3] = 180;
	track[4] = 20;
	track[5] = 90;
	track[6] = 150;
	track[7] = 70;
	track[8] = 80 ;
	track[9] = 10 ;
	track[10] =160 ;
	track[11] = 120;
	track[12] = 40;
	track[13] = 110;
	for (i = 0; i < N; i++){
		track2[i] = track[i];
	}

	//先来先服务
	printf("先来先服务：");

	for (i = 0; i < N; i++){
		printf("%d ", track[i]);        //按照请求到达的顺序服务
	}

	double s = abs(90 - track[0]);      //总移动道数s初始化为起点和90之间的距离

	for (i = 0; i <= N - 2; i++){                   //从第一个服务的点开始，直到倒数第二个点
		s += abs(track[i] - track[i + 1]);          //将每次的移动道数累加
	}

	printf("\n平均移动磁道数：%0.1lf\n\n", s / N);  //计算平均移动道数



	//最短寻道优先
	printf("最短寻道优先：");
	s = 0;       //复原总移动道数
	int temp[N];
	track2[N] = 90;
	int k = N;   //当前磁道
	int flag;    //标志
	int min;     //最小差值
	int t;       //最小差值的数组下标
	int j;

	for (j = 0; j < N; j++){
	flag = 0;
	for (i = 0; i < N; i++){
		if ((i != k) && flag == 0 && track2[i] != -1)//如果请求尚未处理完且磁道没有被访问过
		{
			min = abs(track2[i] - track2[k]);        
			t = i;
			flag++;
		}
		if ((i != k) && (track2[i] != -1) && (abs(track2[i] - track2[k]) < min))
		{
			min = abs(track2[i] - track2[k]);       //更新当前请求和剩余请求的最小差值
			t = i;
		}
	}
	s += min;             //磁头移动，移动道数累加
	temp[j] = t;          //记录最小差值的数组下标
	track2[k] = -1;       //该磁道已访问过
	k = t;                //磁头指向移动后的磁道
	}

	for (i = 0; i < N; i++) {
		printf("%d  ", track[temp[i]]);
	}
		
	printf("\n平均移动磁道数：%0.1lf\n\n", s / N);


	//电梯调度方法
	s = 0;
	printf("电梯调度：");
	for (i = 0; i < N - 1; i++)
	{
		for (j = 0; j < N - i - 1; j++)
		{
			if (track[j] > track[j + 1])
			{
				t = track[j]; 
				track[j] = track[j + 1];
				track[j + 1] = t;				//升序排序
			}
		}
	}
	
	for (i = 0; i < N; i++)
	{
		if (track[i] > 90)
			break;
	}
	
	for (j = i; j < N; j++)
		printf("%d  ", track[j]);              //题目中给出磁道号增大的为前进方向

	for (j = i - 1; j >= 0; j--)
		printf("%d  ", track[j]);              //服务完最远请求后再反向 

	s = (track[N - 1] - 90) + (track[N - 1] - track[0]);

	printf("\n平均移动磁道数：%0.1lf", s / N);

	printf("\n");

}
  