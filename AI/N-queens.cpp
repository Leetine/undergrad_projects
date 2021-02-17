#include"pch.h"
#include "climbHill.h"
#include "geneticAlgorithm.h"
#include "simulatedAnnealing.h"
#include <vector>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <iostream>
using namespace std;
// 定义N皇后问题，皇后数目为N
#define random(x) (rand()%x)  // 定义产生随机数的函数

int N;
IMAGE block;
IMAGE queen;

//---------------------------回溯法------------------------------
int x[100];

void showmap(int **map, int m, int n) {	
	HWND h = initgraph(m * 16, n * 16, SHOWCONSOLE);
	LONG oldstyle = GetWindowLong(h, GWL_STYLE);
	SetWindowLong(h, GWL_STYLE, oldstyle|WS_MAXIMIZEBOX|WS_SIZEBOX);	
	loadimage(&block, L"block.jpg",16, 16,true);
	loadimage(&queen, L"queen.jpg", 16, 16,true);
	for (int i = 0; i < N; i++) {
		int x, y;
		for (int j = 0; j < N; j++) {
			//贴图需要x,y坐标
			x = 16 * j;
			y = 16 * i;
			//游戏状态的分析
			switch (map[i][j]) {
			case 0:
				putimage(x, y, &block);//显示图片
				break;
			case 1:
				putimage(x, y, &queen);//显示图片
				break;
			}
		}
	}	
	
}
/*
判断第k个后能不能放在x[k]处
两个皇后不能放在统一斜线上：
若2个皇后放置的位置分别是（i,j）和（k,l）,
且 i-j = k -l 或 i+j = k+l，则说明这2个皇后处于同一斜线上。
*/
void output() {
	int **map1 = (int**)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) {
		map1[i] = (int*)malloc(sizeof(int)*N);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map1[i][j] = 0;
		}
	}
	cout << "测试后找到第一个方案" << endl;
	for (int i = 1; i <= N; i++) {
		map1[i - 1][x[i] - 1] = 1;
			cout << "(" << i << "," << x[i] << ")" << endl;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << map1[i][j] << " ";
		}
		cout << endl;
	}
	showmap(map1, N, N);
	for (int i = 0; i < N; i++)
		free(*(map1 + i));
}

int place(int k) {
	for (int j = 1; j < k; j++) {
		if (x[j] == x[k] || abs(x[j] - x[k]) == abs(j - k))
			return 0;
	}
	return 1;
}

void BackTrace1(int n) {//非递归搜索第一个解	
	memset(x, 0, sizeof(x));
	int k = 1;
	x[1] = 0;//初始将第一个皇后放在第一行第一列，也即解空间的第一个位置
	while (k >= 1) {
		x[k] += 1;//先放在第一个位置
		while ((x[k] <= n && !(place(k)))) {//如果不能放
			x[k]++;  //放在下一个位置
		}
		if (x[k] <= n) {
			if (k == n) {// 如果已经放完了n个皇后
				output();//输出坐标、示意图、棋盘
				break;
			}
			else {//  //没有处理完，让k自加，处理下一个皇后
				k++;
				x[k] = 0;
			}
		}
		else {// 当前无法完成放置，则进行剪枝，回溯回去，回到第k-1步
			k--;
		}
	}	
}
//---------------------------爬山法------------------------------

typedef vector<int> CollisionList_t;
//皇后位置的表示方法:
//使用数组chessman[N]来表示N个皇后的位置
//第i个皇后chessman[i]的下标i表示其行所在的位置，
//chessman[j]表示其列的位置。

void print_chessboard(int* chessman, int N)
{
	int **map2 = (int**)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) {
		map2[i] = (int*)malloc(sizeof(int)*N);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map2[i][j] = 0;
		}
	}
	for (int i = 1; i <= N; i++) {
		map2[i - 1][chessman[i - 1]] = 1;
			cout << "(" << i << "," << chessman[i-1] +1<< ")" << endl;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << map2[i][j] << " ";
		}
		cout << endl;
	}
	showmap(map2, N, N);
	for (int i = 0; i < N; i++)
		free(*(map2 + i));
}

// 随机生成一个初始化状态，在每行每列上放置一个皇后
void generate_init_state(int* chessman, int N)
{
	for (int i = 0; i < N; ++i) {
		chessman[i] = i;
	}

	for (int j = 0; j < N; ++j) {
		int r = rand();
		r = r % N;
		swap(chessman[r], chessman[N - r - 1]);
	}
}

// 返回冲突的皇后个数
int h(int* chessman, int N, CollisionList_t& collision_list)
{
	collision_list.clear();
	int collision = 0;
	for (int i = 0; i < N; ++i) {
		for (int row = i + 1; row < N; row++) {
			if ((chessman[row] == chessman[i] + row - i)
				|| (chessman[row] == chessman[i] - (row - i))) {
				collision_list.push_back(row);
				++collision;
			}
		}
	}
	return collision;
}

// 如果交换后冲突不比原来的大，就进行交换
// 只有交换成功后才改变cl为新的冲突列表
int try_exchange(int* chessman, int N, int row1, int row2, CollisionList_t& cl)
{
	CollisionList_t new_cl;

	// 交换两行的皇后的位置
	swap(chessman[row1], chessman[row2]);
	int new_collision = h(chessman, N, new_cl);
	if (new_collision > cl.size()) {
		// 取消之前的交换
		swap(chessman[row1], chessman[row2]);
	}
	else {
		cl = new_cl;
	}

	return new_cl.size();
}

int choose_next_state(int* chessman, int N, CollisionList_t& cl)
{
	int old_collision = cl.size();
	int new_collision;

	int row1 = -1;
	int row2 = -1;

	// 优化最后只有一个冲突的解
	if (cl.size() == 1) {
		for (int i = 0; i < N; ++i) {
			if (i != cl[0] && (try_exchange(chessman, N, cl[0], i, cl) == 0)) {
				return 0;
			}
		}
	}

	do {
		// 最后的选择，随机的选择两个皇后调换其位置
		row1 = rand() % N;
		do {
			row2 = rand() % N;
		} while (row1 == row2);
		new_collision = try_exchange(chessman, N, row1, row2, cl);
	} while (new_collision > old_collision);

	return new_collision;
}

// 使用随机爬山法寻找一个N皇后问题的解
int queen_solution(int N)
{
	int* chessman = new int[N];
	int max_tries = N * N;
	int max_steps = N * N;
	int tries = 0;
	while (tries < max_tries) {
		++tries;

		int steps = 0;
		int collision = 0;
		CollisionList_t collision_list;

		srand(time(NULL) + tries * collision);
		generate_init_state(chessman, N);
		collision = h(chessman, N, collision_list);

		while ((collision != 0) && (steps < max_steps)) {
			collision = choose_next_state(chessman, N, collision_list);
			++steps;
		}

		if (collision == 0) {
			cout << "测试后找到一个方案:" << endl;
			print_chessboard(chessman, N);
			return 1;
		}
	}

	return 0;
}


//---------------------------遗传算法和模拟退火------------------
// 初始化棋盘
void initChessboard(vector<int>& chessboard) {
	for (int i = 0; i < N; i++) {
		chessboard.push_back(i);
	}

	srand((unsigned)time(0));
	for (int row1 = 0; row1 < N; row1++) {
		int row2 = random(N);
		// 随机交换行，打散棋盘，但保证皇后都在不同列
		swap(chessboard[row1], chessboard[row2]);
	}
}

// 打印棋盘
void printChessboard(vector<int>& chessboard) {
	int **map3 = (int**)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) {
		map3[i] = (int*)malloc(sizeof(int)*N);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map3[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++) {
		map3[i][chessboard[i]] = 1;
		cout << "(" << i+1 << "," << chessboard[i] + 1 << ")" << endl;		
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << map3[i][j] << " ";
		}
		cout << endl;
	}
	showmap(map3, N, N);
	for (int i = 0; i < N; i++)
		free(*(map3 + i));
}

// 转换数字string 为int
int stringToInt(string choose) {
	if (choose == "1") return 1;
	if (choose == "2") return 2;
	if (choose == "3") return 3;
	if (choose == "4") return 4;
	if (choose == "5") return 5;
}


int main() {

	//定义数据结构Vector<int> 下标代表行数，值代表列数，表示一个棋盘
	N = 0;
	cout << "输入你要解决的N皇后问题的N值" << endl;
	cin >> N;
	cout << "========queens Problems ======= " << endl;
	cout << "Choose one algorithhm to solve the problem " << endl;
	cout << "1. Data backing Alogrithm" << endl;
	cout << "2. Mount climbing Alogrithm " << endl;
	cout << "3. Simulated Annealing Alogrithm " << endl;
	cout << "4. Genetic Algorithm " << endl;
	cout << "5. quit " << endl;
	string choose;

	// 初始化对象
	// 1. 三种算法对应三个棋盘
	//vector<int> temp1_chessboard;
	vector<int> temp3_chessboard;
	vector<int> temp4_chessboard;
	// 2.统计时间
	double totalTime;

	// 3.解决算法的对象
	climbHill sol1(N);
	simulatedAnnealing sol3(N);
	geneticAlgorithm sol4(N);
	
	
	while (1) {
		cout << "--------------------------------------------" << endl;
		cout << "请选择：" << endl;
		cin >> choose;
		int num = stringToInt(choose);
		int t = 0;   // 执行算法的次数
		if (num == 1) {			
			clock_t start, stop; //clock_t为clock()函数返回的变量类型
			double duration;
			start = clock();
			BackTrace1(N);	
			stop = clock();
			duration = (double)(stop - start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位，即时钟打点
			cout << "回溯法解决" << N << "皇后问题用时：" << duration << "s" << endl;
		}
		if (num == 2) {
			clock_t start, stop; //clock_t为clock()函数返回的变量类型
			double duration;
			start = clock();
			int result = queen_solution(N);
			if (result != 1) {
				cout << "本次失败，请重新运行." << endl;
			}
			stop = clock();
			duration = (double)(stop - start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位，即时钟打点
			cout << "爬山法解决" << N << "皇后问题用时：" << duration << "s" << endl;

		}
		if (num == 3) {
			cout << "正在求解" << endl;
			initChessboard(temp3_chessboard);
			clock_t start, stop; //clock_t为clock()函数返回的变量类型
			double duration;
			start = clock();
			temp3_chessboard = *sol3.solve(&temp3_chessboard);	// 求解部分
			printChessboard(temp3_chessboard);
			stop = clock();
			duration = (double)(stop - start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位，即时钟打点
			cout << "模拟退火法解决" << N << "皇后问题用时：" << duration << "s" << endl;
		}
		if (num == 4) {
			cout << "正在求解" << endl;
			initChessboard(temp4_chessboard);
			clock_t start, stop; //clock_t为clock()函数返回的变量类型
			double duration;
			start = clock();
			temp4_chessboard = *sol4.solve(temp4_chessboard);
			printChessboard(temp4_chessboard);
			stop = clock();
			duration = (double)(stop - start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位，即时钟打点
			cout << "遗传算法解决" << N << "皇后问题用时：" << duration << "s" << endl;
		}
		if (num == 5) {
			exit(0);
			break;
		}

	}
}