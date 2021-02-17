#pragma once
/*************************************************************	
		  ��ɽ�����������������ԭ�򣬵���ɽ�����ﵽһ��������
		  �Ὣ�������´��ҡ�ѭ������ÿһ�У�ÿһ�ж�ѡ�����ŵ�λ��
		  ֱ����ͻ�ĸ���Ϊ0
****************************************************************/

#ifndef CLIMBHILL_H
#define CLIMBHILL_H
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#define random(x) (rand()%x)  // �������������ĺ���

using namespace std;
class climbHill {
private:
	int N; // ���̴�С
	int step;				   // ��¼���еĲ���
	int maxSteps;         // �ﵽ���ɨ���������δ���������
	int rowPosition;           // �ڸ���λ�ü������λ��
public:
	climbHill(int N) {
		step = 0;
		maxSteps = N * N * 2;
		this->N = N;
		rowPosition = 0;
	}

	//����������ﵽһ���Ĳ�������δ�����ô�����´�������
	//�����ɶԽ��ߵĻʺ����ӣ�Ȼ�������������
	void reset(vector<int>& chessboard) {
		chessboard.clear();
		for (int i = 0; i < N; i++) {
			chessboard.push_back(i);
		}
		srand((unsigned)time(0));
		for (int row1 = 0; row1 < N; row1++) {
			int row2 = random(N);
			// ��������У���ɢ���̣�����֤�ʺ��ڲ�ͬ��
			swap(chessboard[row1], chessboard[row2]);
		}
	}

	// ���㵱ǰ���̴��ڵ��໥�����Ļʺ����
	int getNumofConflicts(vector<int> *chessboard) {
		int numOfConflicts = 0;
		int width = this->N;
		for (int i = 0; i < width; i++) {
			for (int j = i + 1; j < width; j++) {
				// �����ڻʺ�λ�ڶԽ��ߵ�ʱ�� ��ͻ��+1
				if (abs(j - i) == abs((*chessboard)[i] - (*chessboard)[j])) {
					numOfConflicts++;
				}
				// �����ڻʺ�λ��ͬһ�е�ʱ�򣬳�ͻ��+1
				if ((*chessboard)[i] == (*chessboard)[j]) {
					numOfConflicts++;
				}
			}
		}
		return numOfConflicts;
	}

	// ����ĳһ�е�����λ��
	int chooseTheBestPosition(vector<int>* chessboard, int row) {
		// ��¼Ϊ�ƶ�֮ǰ��λ��
		int originPosition = (*chessboard)[row];
		// ��¼ÿһ��λ�ö�Ӧ������״̬����Ӧ�ĳ�ͻ��
		vector<int> conflictData;
		for (int i = 0; i < N; i++) {
			(*chessboard)[row] = i;
			conflictData.push_back(getNumofConflicts(chessboard));
		}
		//��¼��ͻ����С�ļ���λ�ã�Ȼ�����Ȼ��һ��λ��
		int less = 0;
		vector<int> bestPosition;
		for (int i = 0; i < N; i++) {
			if (i == 0) {
				bestPosition.push_back(0);
				less = conflictData[0];
				continue;
			}
			// �ж�iλ�ó�ͻ��С������λ��,����������洢��С��ͻ��λ��
			if (conflictData[i] < less) {
				bestPosition.clear();
				bestPosition.push_back(i);
				less = conflictData[i];
			}
			// ��ͬ����С�ĳ�ͻλ��Ҳ��ӵ�������
			else if (conflictData[i] == less) {
				bestPosition.push_back(i);
			}
		}
		if (bestPosition.size() == 1)
			return bestPosition[0];

		// ���λ���ж���������ɸѡ
		srand((unsigned)time(0));
		return bestPosition[rand() % bestPosition.size()];
	}

	// ÿ��ѡ������λ��
	vector<int>* scanChessboard(vector<int>* chessboard, int row) {
		(*chessboard)[row] = chooseTheBestPosition(chessboard, row);
		return chessboard;
	}

	// ��⣬������Ѱ״̬���õ������ֱ����ͻ��Ϊ0
	vector<int>* solve(vector<int> *chessboard) {
		// �������
		srand(time(NULL));
		int resetTime = 0;  // ��������
		step = 0;   // ͳ�����в���
		// ����ͻ��Ϊ0ʱ��ֹ��ɽ
		while (getNumofConflicts(chessboard) > 0) {
			if (step >= maxSteps) {
				reset(*chessboard);
				resetTime++;
				step = 0;
				cout << "�������" << endl;
			}
			// ��rowPostion�еĻʺ��Ƶ�ͬһ�е�����λ��
			chessboard = scanChessboard(chessboard, rowPosition++);
			// �ж�rowPosition�Ƿ���㣬��ֹԽ��
			rowPosition = rowPosition >= N ? rowPosition % N : rowPosition;
			step++;
			// 	cout << step << endl;
		}
		cout << "Solved the problem, totally " << step << " steps. Including " << resetTime << " reset times." << endl;
		return chessboard;
	}

};

#endif // !