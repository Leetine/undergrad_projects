// 动态内存分配.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<string>
#include<map>
using namespace std;

typedef struct node
//自由主存队列结点
{
	int size;
	int index;
	node *next;
}pd;

const int fixed_size = 2;//已分配主存块最前面两个单元依次表示flag,size
const int MAX_size = 512;//m_rid主存资源信息块宏定义上限
int memory[MAX_size];//m_rid主存资源信息块
const int small = 1;

typedef struct
//待处理进程结点 
{
	string name;
	int resource;
	int index;
	bool flag;//表示该进程是否还在主存
}process;
process Process[20];
int num_process;//记录已处理进程数

pd *head = new pd;//自由主存队列头结点

void printmenu()
//输出菜单函数
{
	cout << "**********************************" << endl;
	cout << "        1.首次适应算法" << endl;
	cout << "        2.最佳适应算法" << endl;
	cout << "        3.最坏适应算法" << endl;
	cout << "        4.退出" << endl;
	cout << "**********************************" << endl;
}

int getmd(int choice, int size)
//分配一个主存块，返回在主存资源信息块的下标(除去flag,size)
{
	int size1 = size + fixed_size;
	pd *pre = head;
	pd *cur = head->next;
	while (cur != NULL)
	{
		if (cur->size >= size1)
			break;
		else
		{
			pre = cur;
			cur = cur->next;
		}
	}
	if (cur == NULL)
	{
		return -1;
	}
	pd *new_p = new pd;
	new_p->size = cur->size - size1;	
	new_p->index = cur->index;
	memory[new_p->size] = 1;
	memory[cur->size + 1] = size;
	cout << cur->size + 1 << "  " << memory[cur->size + 1] << endl;
	if (1 == choice)//首次适应算法
	{
		new_p->next = cur->next;
		pre->next = new_p;
	}
	else if (2 == choice || 3 == choice)//最佳适应算法
	{
		pre->next = cur->next;
		pd *tpre = head;
		pd *tcur = head->next;
		while (tcur != NULL)
		{
			if (2 == choice)
			{
				if (new_p->size <= tcur->size)
					break;
			}
			else if (3 == choice)
			{
				if (new_p->size >= tcur->size)
					break;
			}
			tpre = tcur;
			tcur = tcur->next;
		}
		tpre->next = new_p;
		new_p->next = tcur;

	}
	//	}
	return 
		cur->index + cur->size - size1 + fixed_size;
}


void relmb(int choice, int index)
//回收一主存块，返回在主存资源信息块的下标
{
	int new_hfree = index - 2;
	int new_rfree = index + memory[index - 1];
	pd *pre = head;
	pd *cur = head->next;
	while (cur != NULL)//需回收区在自由区上游
	{
		if (cur->index + cur->size + 2 == new_hfree)
		{
			cur->size += memory[index - 1];
			if (1 == choice)
				return;
			else if (2 == choice || 3 == choice)
			{
				pre->next = cur->next;
				pd *tpre = head;
				pd *tcur = head->next;
				while (tcur != NULL)
				{
					if (2 == choice)
					{
						if (cur->size <= tcur->size)
							break;
					}
					else if (3 == choice)
					{
						if (cur->size >= tcur->size)
							break;
					}
					tpre = tcur;
					tcur = tcur->next;
				}
				tpre->next = cur;
				cur->next = tcur;
				return;
			}
		}
		pre = cur;
		cur = cur->next;
	}

	pre = head;
	cur = head->next;
	while (cur != NULL)//需回收区在自由区下游
	{
		if (cur->index - 2 == new_rfree)
		{
			cur->index = new_hfree;
			cur->size += memory[index - 1];
			if (1 == choice)
				return;
			else if (2 == choice || 3 == choice)
			{
				pre->next = cur->next;
				pd *tpre = head;
				pd *tcur = head->next;
				while (tcur != NULL)
				{
					if (2 == choice)
					{
						if (cur->size <= tcur->size)
							break;
					}
					else if (3 == choice)
					{
						if (cur->size >= tcur->size)
							break;
					}
					tpre = tcur;
					tcur = tcur->next;
				}
				tpre->next = cur;
				cur->next = tcur;
				return;
			}
		}
		pre = cur;
		cur = cur->next;
	}

	//若不直接相连，则插入到适当位置
	pd *p = new pd;
	p->index = new_hfree;
	cout << index << "   " << memory[index - 1] << "  " << memory[349] << endl;
	p->size = memory[index - 1];
	if (1 == choice)
	{
		pre = head;
		cur = head->next;
		while (cur != NULL)
		{
			if (new_hfree<cur->index)
				break;
			pre = cur;
			cur = cur->next;
		}
		if (cur == NULL)
		{
			pre->next = p;
			p->next = NULL;
			return;
		}
		p->next = cur;
		pre->next = p;
	}
	else if (2 == choice || 3 == choice)
	{
		pd *tpre = head;
		pd *tcur = head->next;
		while (tcur != NULL)
		{
			if (2 == choice)
			{
				if (p->size <= tcur->size)
					break;
			}
			else if (3 == choice)
			{
				if (p->size >= tcur->size)
					break;
			}
			tpre = tcur;
			tcur = tcur->next;
		}
		tpre->next = p;
		p->next = tcur;
	}
}


void adaption(int cho)
//根据参数选择放置策略
{
	int choice;
	num_process = 1;
	map<string, int>Map;//用来关联进程名与下标
	while (1)
	{
		do//知道选择正确的为止
		{
			if (1 == cho)
				printf("=============首次适应算法=======================\n");
			else if (2 == cho)
				printf("=============最佳适应算法=======================\n");
			else if (3 == cho)
				printf("=============最坏适应算法=======================\n");

			printf("        1.申请\n");
			printf("        2.完成\n");
			printf("        3.退出\n");
			printf("请输入您的选项[ ]\b\b");
			scanf_s("%d", &choice);
		} while (choice != 1 && choice != 2 && choice != 3);
		if (1 == choice)
		{
			printf("请输入需申请主存的进程名");
			cin >> Process[num_process].name;
			printf("请输入该进程所需主存大小");
			scanf_s("%d", &Process[num_process].resource);
			Process[num_process].flag = true;
			Map[Process[num_process].name] = num_process;

			Process[num_process].index = getmd(cho, Process[num_process].resource);//进行试探性分配
			if (-1 == Process[num_process].index)
			{
				cout << "进程" << Process[num_process].name << "无法得到" << Process[num_process].resource << "的主存资源!" << endl;
				continue;
			}
			pd *aa = head->next;
			while (aa != NULL)
			{
				printf("index=%d  sie=%d\n", aa->index, aa->size);
				aa = aa->next;
			}
			num_process++;
		}
		else if (2 == choice)
		{
			string name;
			int ind;
			bool flag1 = false;
			do//保证进程名有效
			{
				if (flag1)
				{
					if (0 == ind)
						printf("输入完成的进程名在主存资源块中无法找到，请重新输入!\n");
					else if (-1 == ind)
						printf("输入完成的进程名已经退出了主存区，无法再次退出，请重新输入!\n");
				}
				flag1 = true;
				printf("请输入完成的进程名");
				cin >> name;
				ind = Map[name];
				if (ind != 0 && !Process[ind].flag)//表明该进程已经退出了主存区
					ind = -1;
			} while (ind == 0 || ind == -1);

			getchar();
			cout << Process[ind].index << endl;
			relmb(cho, Process[ind].index);
			Process[ind].flag = false;
			printf("Release Success!\n");
			pd *aa = head->next;
			while (aa != NULL)
			{
				printf("index=%d  sie=%d\n", aa->index, aa->size);
				aa = aa->next;
			}
		}
		else if (3 == choice)
			return;
	}
}


void initiate()
//初始化自由主存队列
{
	head->next = new pd;
	pd *p = head->next;
	p->next = NULL;
	p->index = 0;
	p->size = MAX_size;
}


void menu()
//根据菜单提示选择相应的子功能函数
{
	int choice;
	bool flag = false;
	do//直到输入正确的为止
	{
		printmenu();
		cout << "请输入您的选择[ ]\b\b";
		cin >> choice;
		getchar();
		if (flag)
			cout << "你的选项不存在!请重新输入!" << endl;
		flag = true;
	} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);
	initiate();
	if (choice != 4)
		adaption(choice);
	else if (4 == choice)
		exit(0);
}


int main()
{
	while (1)
	{
		menu();
	}
	return 0;
}