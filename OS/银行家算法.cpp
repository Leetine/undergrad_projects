// 银行家算法.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#define n 5//进程个数
#define m 3//资源种类
int available[m] = { 0 };
int alloc[n][m]= { 0 };
int need[n][m]= { 0 };
int request[m];
int x;
void input() {
	cout << "请输入m种可利用资源的个数：";
	for (int i = 0; i < m; i++) {
		cin >> available[i];
	}
	cout << "请输入第1-n个进程已分配的各资源的个数：";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> alloc[i][j];
		}
	}	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << "已分配：" << alloc[i][j] << " ";
		}
		cout << endl;
	}
	cout << "请输入第1-n个进程对各资源的需求量：";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> need[i][j];
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << "需求：" << need[i][j] << " ";
		}
		cout << endl;
	}
}

void readrequest() {		
	cout << "进程的编号x是:";
	cin >> x;
	cout << "请输入进程p" << x << "对各资源请求的个数"<<endl;
	for (int i = 0; i < m; i++) {
			cin >> request[i];
	}
}

/*bool judge() {
	char reply;
	cout << "还有下一个进程吗？有，输入Y，没有，输入N"<<endl;
	cin>>reply;
	if (reply == 'Y') { return true; }
	if (reply == 'N') { return false; }
}*/

bool safe() {
	int work[m]; int finish[n];
	int i, j, p, q = 0, count=0;
	for (i = 0; i < n; i++) { finish[i] = false; }//标志向量初始化
	finish[x - 1] = true;
	for (j = 0; j < m; j++) work[j] = available[j];//把可利用资源数赋给work[]
	while (1) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				if (need[i][j] <= work[j])
					count++;
			}
			if (finish[i] == false && count == m) {
				//某个进程未完成，并且各类资源需求值小于对应可利用的值
				for (p = 0; p < m; p++) {
					work[p] += alloc[i][p]; //释放资源
				}
				finish[i] = true;//修改状态				
			}
			count = 0;
			i = 0;
		}	
	}	
	if (q == n - 1) {
		cout << "预分配安全!";		
		return 1;
	}
	else {
		cout << "系统不安全!";
		finish[x - 1] = false;
		return 0;
	}
}
int main(){
	        input();
			readrequest();
			for (int i = 0; i< m;i++) {
				if (request[i] > need[x-1][i]) {
					cout << "非法请求！" << endl;
					break;
				}
			}
			for (int i = 0; i < m; i++) {
				if (request[i] > available[i]) {
					cout << "进程阻塞！" << endl;
					break;
				}
			}
			//预分配
			for (int j = 0; j < m;j++) {
				available[j] -= request[j];
				alloc[x - 1][j] += request[j];
				need[x - 1][j] -= request[j];
			}
			//安全性判别
			if(!safe()) {
				cout << "无法分配资源！恢复原本状态";
				for (int j = 0; j < m; j++) {
					available[j] += request[j];
					alloc[x - 1][j] -= request[j];
					need[x - 1][j] += request[j];
				}
			}			
		
	return 0;
}
