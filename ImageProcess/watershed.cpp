﻿// watershed.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
//Image Watershed Segmentation
//This is the implementation of the algorithm based on immersion model. 
// ===========================================================================
// =====     Module: Watershed.cpp
// ===== -------------------------------------------------------------- ======
// =====     Version 01   Date: 04/21/2003
// ===== -------------------------------------------------------------- ======
// ===========================================================================


#include <queue>
#include <vector>
#include <windows.h>
using namespace std;

/*====================================================================
函数名            ：    Watershed
功能            ：    用标记-分水岭算法对输入图像进行分割
算法实现        ：    无
输入参数说明    ：    OriginalImage --输入图像（灰度图，0~255）
                     SeedImage   --标记图像（二值图，0-非标记，1-标记）
                     LabelImage   --输出图像（1-第一个分割区域，2-第二个分割区域，...）
                     row       --图像行数
                     col       --图像列数
返回值说明        ：    无    
====================================================================*/
void Watershed(const int **OriginalImage, char** SeedImage, int **LabelImage, int row, int col)
{
	

	//标记区域标识号，从1开始
	int Num = 0;
	int i, j;

	//保存每个队列种子个数的数组
	vector<int*> SeedCounts;
	//临时种子队列
	queue<POINT> que;
	//保存所有标记区域种子队列的数组
	vector<queue<POINT>* > qu;

	int* array;
	queue<POINT> *uu;
	POINT temp;

	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			LabelImage[j] = 0;


	int m, n, k = 0;
	int up, down, right, left, upleft, upright, downleft, downright;

	//预处理,提取区分每个标记区域，并初始化每个标记的种子队列
	//种子是指标记区域边缘的点，他们可以在水位上升时向外淹没（或者说生长）
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			//如果找到一个标记区域
			if (SeedImage[i][j] == 1)
			{
				//区域的标识号加一
				Num++;
				//分配数组并初始化为零
				array = new int[256];
				ZeroMemory(array, 256 * sizeof(int));
				//
				SeedCounts.push_back(array);
				//分配本标记的优先队列
				uu = new queue<POINT>[256];
				//加入到队列数组中
				qu.push_back(uu);
				//当前点放入本标记区域的临时种子队列中
				temp.x = i;
				temp.y = j;
				que.push(temp);
				//当前点标记为已处理
				LabelImage[i][j] = Num;
				SeedImage[i][j] = 127;

				//让种子队列中的种子进行生长直到所有的种子都生长完毕
				while (!que.empty())
				{
					up = down = right = left = 0;
					upleft = upright = downleft = downright = 0;
					//队列中取出一个种子
					temp = que.front();
					m = temp.x;
					n = temp.y;
					que.pop();

					if (m > 0)
					{
						//上方若为可生长点则加为新种子
						if (SeedImage[m - 1][n] == 1)
						{
							temp.x = m - 1;
							temp.y = n;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m - 1][n] = Num;
							SeedImage[m - 1][n] = 127;
						}
						else//否则上方为不可生长
						{
							up = 1;
						}
					}
					if (m > 0 && n > 0)
					{
						if (SeedImage[m - 1][n - 1] == 1)//左上方若为可生长点则加为新种子
						{
							temp.x = m - 1;
							temp.y = n - 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m - 1][n - 1] = Num;
							SeedImage[m - 1][n - 1] = 127;
						}
						else//否则左上方为不可生长
						{
							upleft = 1;
						}
					}

					if (m < row - 1)
					{
						if (SeedImage[m + 1][n] == 1)//下方若为可生长点则加为新种子
						{
							temp.x = m + 1;
							temp.y = n;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m + 1][n] = Num;
							SeedImage[m + 1][n] = 127;
						}
						else//否则下方为不可生长
						{
							down = 1;
						}
					}
					if (m < (row - 1) && n < (col - 1))
					{
						if (SeedImage[m + 1][n + 1] == 1)//下方若为可生长点则加为新种子
						{
							temp.x = m + 1;
							temp.y = n + 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m + 1][n + 1] = Num;
							SeedImage[m + 1][n + 1] = 127;
						}
						else//否则下方为不可生长
						{
							downright = 1;
						}
					}

					if (n < col - 1)
					{
						if (SeedImage[m][n + 1] == 1)//右方若为可生长点则加为新种子
						{
							temp.x = m;
							temp.y = n + 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m][n + 1] = Num;
							SeedImage[m][n + 1] = 127;
						}
						else//否则右方为不可生长
						{
							right = 1;
						}
					}
					if (m > 0 && n < (col - 1))
					{
						if (SeedImage[m - 1][n + 1] == 1)//右上方若为可生长点则加为新种子
						{
							temp.x = m - 1;
							temp.y = n + 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m - 1][n + 1] = Num;
							SeedImage[m - 1][n + 1] = 127;
						}
						else//否则右上方为不可生长
						{
							upright = 1;
						}
					}

					if (n > 0)
					{
						if (SeedImage[m][n - 1] == 1)//左方若为可生长点则加为新种子
						{
							temp.x = m;
							temp.y = n - 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m][n - 1] = Num;
							SeedImage[m][n - 1] = 127;
						}
						else//否则左方为不可生长
						{
							left = 1;
						}
					}
					if (m < (row - 1) && n>0)
					{
						if (SeedImage[m + 1][n - 1] == 1)//左下方若为可生长点则加为新种子
						{
							temp.x = m + 1;
							temp.y = n - 1;
							que.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m + 1][n - 1] = Num;
							SeedImage[m + 1][n - 1] = 127;
						}
						else//否则左方为不可生长
						{
							downleft = 1;
						}
					}

					//上下左右只要有一点不可生长，那么本点为初始种子队列中的一个
					if (up || down || right || left ||
						upleft || downleft || upright || downright)
					{
						temp.x = m;
						temp.y = n;
						qu[Num - 1][OriginalImage[m][n]].push(temp);
						SeedCounts[Num - 1][OriginalImage[m][n]]++;
					}

				}//while结束
			}
		}
	}

	bool actives;//在某一水位处，所有标记的种子生长完的标志
	int WaterLevel;

	//淹没过程开始，水位从零开始上升
	for (WaterLevel = 0; WaterLevel < 256; WaterLevel++)
	{
		actives = true;
		while (actives)
		{
			actives = false;
			//依次处理每个标记区域
			for (i = 0; i < Num; i++)
			{
				if (!qu[i][WaterLevel].empty())
				{
					actives = true;
					while (SeedCounts[WaterLevel] > 0)
					{
						SeedCounts[WaterLevel]--;
						temp = qu[i][WaterLevel].front();
						qu[i][WaterLevel].pop();
						m = temp.x;
						n = temp.y;//当前种子的坐标
						if (m > 0)
						{
							if (!LabelImage[m - 1][n])//上方若未处理
							{
								temp.x = m - 1;
								temp.y = n;
								LabelImage[m - 1][n] = i + 1;//上方点标记为已淹没区域

								if (OriginalImage[m - 1][n] <= WaterLevel)//上方若为可生长点则加入当前队列
								{
									qu[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m-1][n]级队列
								{
									qu[i][OriginalImage[m - 1][n]].push(temp);
									SeedCounts[OriginalImage[m - 1][n]]++;
								}
							}
						}

						if (m < row - 1)
						{
							if (!LabelImage[m + 1][n])//下方若未处理
							{
								temp.x = m + 1;
								temp.y = n;
								LabelImage[m + 1][n] = i + 1;//下方点标记为已淹没区域

								if (OriginalImage[m + 1][n] <= WaterLevel)//下方若为可生长点则加入当前队列
								{
									qu[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m+1][n]级队列
								{
									qu[i][OriginalImage[m + 1][n]].push(temp);
									SeedCounts[OriginalImage[m + 1][n]]++;
								}
							}
						}

						if (n < col - 1)
						{
							if (!LabelImage[m][n + 1])//右边若未处理
							{
								temp.x = m;
								temp.y = n + 1;
								LabelImage[m][n + 1] = i + 1;//右边点标记为已淹没区域

								if (OriginalImage[m][n + 1] <= WaterLevel)//右边若为可生长点则加入当前队列
								{
									qu[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m][n+1]级队列
								{
									qu[i][OriginalImage[m][n + 1]].push(temp);
									SeedCounts[OriginalImage[m][n + 1]]++;
								}
							}
						}

						if (n > 0)
						{
							if (!LabelImage[m][n - 1])//左边若未处理
							{
								temp.x = m;
								temp.y = n - 1;
								LabelImage[m][n - 1] = i + 1;//左边点标记为已淹没区域

								if (OriginalImage[m][n - 1] <= WaterLevel)//左边若为可生长点则加入当前队列
								{
									qu[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m][n-1]级队列
								{
									qu[i][OriginalImage[m][n - 1]].push(temp);
									SeedCounts[OriginalImage[m][n - 1]]++;
								}
							}
						}
					}//while循环结束
					SeedCounts[i][WaterLevel] = qu[i][WaterLevel].size();
				}//if结束
			}//for循环结束
		}//while循环结束
	}//for循环结束
	while (!qu.empty())
	{
		uu = qu.back();
		delete[] uu;
		qu.pop_back();
	}
	while (!SeedCounts.empty())
	{
		array = SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
int main() {
	unsigned char** ori_image = NULL;
	ori_image = new unsigned char*[m_imageH];
	for (i = 0; i < m_imageH; i++)
		ori_image[i] = new unsigned char[m_imageW];
}