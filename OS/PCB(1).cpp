// PCB(1).cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<string.h>
#include<iostream>
using namespace std;
typedef struct  pcb_tag {
	char name[10];
	int need;//需要运行的时间
	int turn;//周转时间
	struct pcb_tag *next;
}PCB;
PCB *RQ1,*RQ2;

int clock = 0;//时钟
int cputime = 7;
int main()
{
	char name1[5][10] = { "p1","p2","p3","p4","p5" };
	char name2[5][10] = { "p6","p7","p8","p9","p10" };
	int need1[5] = { 16,11,14,13,15 };
	int turn1[5] = { 6,5,4,3,2 };
	int need2[5] = { 21,18,10,7,14};
	int turn2[5] = { 1,2,3,4,5 };
	PCB *p, *q, *r = NULL;

	//生成链表RQ1
	RQ1 = (PCB*)malloc(sizeof(PCB));
	strcpy_s(RQ1->name, name1[0]);
	RQ1->need = need1[0];
	RQ1->turn = turn1[0];
	RQ1->next = NULL;
	p = RQ1;
	for (int i = 1; i < 5; i++) {
		q = (PCB*)malloc(sizeof(PCB));
		strcpy_s(q->name, name1[i]);
		q->need = need1[i];
		q->turn = turn1[i];
		p->next = q;
		q->next = NULL;
		p = p->next;
	}
	//生成链表RQ2
	RQ2 = (PCB*)malloc(sizeof(PCB));
	strcpy_s(RQ2->name, name2[0]);
	RQ2->need = need2[0];
	RQ2->turn = turn2[0];
	RQ2->next = NULL;
	p = RQ2;
	for (int i = 1; i < 5; i++) {
		q = (PCB*)malloc(sizeof(PCB));
		strcpy_s(q->name, name2[i]);
		q->need = need2[i];
		q->turn = turn2[i];
		p->next = q;
		q->next = NULL;
		p = p->next;
	}
	//轮转法
	p = RQ1;//处理第一个进程p1
	int flag = 0;
	while (p!= NULL) {		
		while (p != NULL && p->need != 0) {
			q = p;
			while(q->next != NULL) { q = q->next; }//p指向一个节点，q指p的前一个节点5
			if (p->need > cputime) {//进程所需时间大于一个时间片
				clock += cputime;
				p->need -= cputime;//分配时间片，相应数值修改

				if (r != NULL) { r->next = p->next; }
				q->next = p;
				p = p->next;
				q->next->next = NULL;//将p加到就绪队列的队尾

			}
			else {
				flag++;
				clock += p->need;
				p->need = 0;//时间片大于所需时间，这个进程结束
				p->turn += clock;//调度时间增加
				
				if (flag == 1)
					RQ1 = p;
				r = p;
				p = p->next;
			}
		}
		while (p != NULL && p->need == 0) {
			p = p->next;
		}
	}
	//短进程优先调度，此时clock=69
	for (int i = 0; i < 5; i++) {
		q = RQ2;
		p= (PCB*)malloc(sizeof(PCB));
		p->next = NULL;
		p->need = 100;
		while (q != 0) {
			if (q->need != 0 && q->need < p->need)
				p = q;
			    q = q->next;//进程所需时间短，排在队列的队首
			}
		clock += p->need;//运行时间最短的进程首先执行
		p->turn += clock;//周转时间包含了已等待的时间
		p->need = 0;//和时间片调度不同，短进程优先每个进程都会执行完
	}
	p = RQ1;
	cout << "RQ1(轮转法）各进程周转时间：" << endl;
	while (p != NULL) {
		cout << "进程"<<p->name<<"周转时间是" << p->turn<<endl;
		p = p->next;
	}
	
	p = RQ2;
	cout << "RQ2（短进程优先）各进程周转时间：" << endl;
	while (p != NULL) {
		cout << "进程" << p->name << "周转时间是" << p->turn<<endl;
		p = p->next;
	}
	cout << endl;
    return 0;
}

