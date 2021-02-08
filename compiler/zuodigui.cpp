// zuodigui.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<iostream>
#include<string>
#include<fstream>
#include<string.h>
using namespace std;

//产生式
struct Production
{
	char L; //产生式左部
	string R; //产生式右部
	char Rfirst; //产生式右部的第一个字符
	int length; //产生式右部的长度
	bool JudgeOutput = true;
};

Production production[100];//产生式集

char vn[10]; //非终结符集
int vnnum = 0;
int pnum; //产生式的个数

//消除左递归
void left_recursion()
{
	for (int i = 0; i < vnnum; i++)  //消除间接左递归
	{
		for (int j = 0; j < i; j++)
		{
			for (int n = 0; n < pnum; n++)
			{
				if (vn[i] == production[n].L)
				{
					if (production[n].R[0] == vn[j])
					{
						for (int m = 0; m < pnum; m++)
						{
							if (production[m].L == vn[j])
							{
								string temp;
								temp = production[n].R.substr(1, production[n].length);
								production[n].JudgeOutput = false;
								production[pnum].L = production[n].L;
								production[pnum].R = production[m].R + temp;
								production[pnum].Rfirst = production[pnum].R[0];
								production[pnum].length = production[pnum].R.length();
								pnum++;
							}
						}
					}
				}
			}
		}
	}
	cout << "消除间接左递归后:" << endl;
	for (int i = 0; i < pnum; i++)
	{
		if (production[i].JudgeOutput)
			cout << production[i].L << "->" << production[i].R << endl;
	}
	/*消除直接左递归*/
	char temps, remove;
	bool Judge = false;
	for (int i = 0; i < pnum; i++)
	{
		if (production[i].L == production[i].R[0])
		{
			string temp;
			temp = production[i].R.substr(1, production[i].length);
			cout << "已有的非终结符：";
			for (int j = 0; j < vnnum; j++)
			{
				cout << vn[j] << " ";
			}
			cout << "请输入一个新的非终结符" << endl;
			cin >> temps;
			remove = production[i].L;
			production[i].L = temps;
			production[i].R = temp + temps;
			production[i].Rfirst = production[i].R[0];
			production[i].length = production[i].R.length();
			for (int j = 0; j < pnum; j++)
			{
				if (production[j].L == remove)
				{
					production[j].R += temps;
					production[j].Rfirst = production[j].R[0];
					production[j].length = production[j].R.length();
				}
			}
			Judge = true;
		}
		else
			continue;
	}
	if (Judge)
	{
		production[pnum].L = temps;
		production[pnum].R = "@";
		pnum++;
	}
	cout << "消除直接左递归后:" << endl;
	for (int i = 0; i < pnum; i++)
	{
		if (production[i].JudgeOutput)
			cout << production[i].L << "->" << production[i].R << endl;
	}
}

//读取数据
void read_data()
{
	cout << "请输入产生式的条数：" << endl;
	cin >> pnum;
	fstream readfile("G~.txt", ios::in);
	for (int i = 0; i < pnum; i++)
	{
		readfile >> production[i].L >> production[i].R;
		production[i].Rfirst = production[i].R[0];
		production[i].length = production[i].R.length();
	}

	//非终结符
	vn[vnnum++] = production[0].L;
	for (int n = 1; n < pnum; n++)
	{
		if (vn[vnnum - 1] != production[n].L)
			vn[vnnum++] = production[n].L;
		else
			continue;
	}

	cout << "消除左递归前的文法为：" << endl;
	for (int k = 0; k < pnum; k++) {
		cout << production[k].L << "->" << production[k].R << endl;
	}
}

int main()
{
	read_data();
	left_recursion();
	system("pause");
	return 0;
}