#pragma once
#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include <iostream>
#include <vector>
#include <string>
class geneticAlgorithm {
private:
	int N;                          // �ʺ����Ŀ
	int numOfIndividuals;           //  ��Ⱥ������Ŀ
	int maxNumberOfNotConflicts;   // ÿһ���������Ĳ���ͻ����
	vector<double> fitness;           // ÿһ���������Ӧ��ֵ�б�
	vector<int> targetChessboard;  // Ŀ������
	vector<vector<int>> population;  //��Ⱥ

public:
	geneticAlgorithm(int N) {
		this->N = N;
		maxNumberOfNotConflicts = calculateMaxNotConflicts(N);
		numOfIndividuals = 4 * N;
	}

	// ����������Ĳ���ͻ����
	int calculateMaxNotConflicts(int N) {
		int num = 0;
		for (int i = N - 1; i > 0; i--) {
			num += i;
		}
		return num;
	}

	// ��Ӧ�Ⱥ����������岻��ͻ�Ļʺ���
	int getNumOfNotConflicts(vector<int> *chessboard) {
		int numOfNotConflicts = 0;
		int width = this->N;
		for (int i = 0; i < width; i++) {
			for (int j = i + 1; j < width; j++) {
				// �����ڻʺ�λ�ڶԽ��ߵ�ʱ�� �Ҳ���ͬһ��ʱ
				//cout << i << " " << j << " " << N << " " << chessboard->size() << endl;
				if (abs(j - i) != abs((*chessboard)[i] - (*chessboard)[j]) && (*chessboard)[i] != (*chessboard)[j]) {
					numOfNotConflicts++;
				}
			}
		}
		return numOfNotConflicts;
	}

	// ������Ⱥ�����̣��в����ڳ�ͻ�ĸ����λ�ã����������򷵻�-1
	int positionOfNotConflict(vector<vector<int>>& population) {
		for (int i = 0; i < numOfIndividuals; i++) {
			if (getNumOfNotConflicts(&(population[i])) == this->maxNumberOfNotConflicts) {
				return i;
			}
		}
		return -1;
	}

	// ������Ⱥ��ÿһ���������Ӧ��,���ҽ�����������fitness��
	void calculateFitness(vector<vector<int>>& population, vector<double>& fitness) {
		fitness.clear();
		int sumOfNotConflicts = 0;
		vector<int> notConflict;
		for (int i = 0; i < numOfIndividuals; i++) {
			int n = getNumOfNotConflicts(&(population[i]));
			sumOfNotConflicts += n;
			//	cout << "CalculateFitness: " << i << " " << n << endl;
			notConflict.push_back(n);
		}
		for (int i = 0; i < numOfIndividuals; i++) {
			fitness.push_back(1.0 * notConflict[i] / sumOfNotConflicts);
			//cout << " " << 1.0 * notConflict[i] / sumOfNotConflicts;
		}

	}

	// �������N�����壬�γ�һ����Ⱥ
	vector<vector<int>>* createPopulation(vector<vector<int>>& population, int numberOfIndividual) {
		population.clear();
		vector<int> chessboard;
		for (int i = 0; i < numberOfIndividual; i++) {
			for (int j = 0; j < N; j++) {
				chessboard.push_back(j);
			}
			for (int row1 = 0; row1 < N; row1++) {
				int row2 = random(N);
				// ��������У���ɢ���̣�����֤�ʺ��ڲ�ͬ��
				swap(chessboard[row1], chessboard[row2]);
			}

			population.push_back(chessboard);
			chessboard.clear();
		}
		return &population;
	}

	// ���ѡ����������,���̶��㷨��˼���ǲ���һ��0~1�������������Ӧ�ȵı�����ѡ����
	vector<vector<int>>* select(const vector<vector<int>>& population, const vector<double>& fitness, vector<vector<int>>& parents) {
		float m = 0;
		parents.clear();
		float p1 = (rand() % 100) * 1.0 / 100;
		float p2 = (rand() % 100)* 1.0 / 100;
		for (int i = 0; i < numOfIndividuals; i++) {
			m += fitness[i];
			if (p1 <= m) {
				// �����һ������
				// �����������p1 �� m ~m+fitness[i] ������Ϊѡ����i
			//	cout << "select1 " << p1 << " " << m << " " << "i " << i << endl;
				parents.push_back(population[i]);
				break;
			}
		}
		m = 0;
		for (int i = 0; i < numOfIndividuals; i++) {
			m += fitness[i];
			// ����ڶ�������
			if (p2 <= m) {
				//	cout << "select2 " << p2 << " " << m << " " << "i " << i << endl;
				parents.push_back(population[i]);
				break;
			}
		}
		return &parents;
	}

	// ��������������ӽ�������һ�����,˫��+˫���ӽ�
	vector<int>* crossover(const vector<int>& chessboard1, const vector<int>& chessboard2, vector<int> &son) {		
		int pos1 = 0, pos2 = 0;
		son.clear();
		while (pos1 >= pos2) {
			pos1 = rand() % N;
			pos2 = rand() % N;
		}
		for (int i = 0; i < this->N; i++) {
			if (i < pos1 || i > pos2) {
				son.push_back(chessboard1[i]);
			}
			else {
				son.push_back(chessboard2[i]);
			}
		}
		//cout << "crossover: " << pos1 << " " << pos2 << endl;
		return &son;
	}

	// �Ըò����ĺ��������б���
	vector<int>* mutate(vector<int>& chessboard) {
		//srand((unsigned)time(NULL));
		int row1 = rand() % N;
		int col1 = rand() % N;
		chessboard[row1] = col1;
		//cout << "mutate :" << row1 << " " << col1 << endl;
		return &chessboard;
	}
	
	// ���
	vector<int>* solve(vector<int>& chessboard) {
		double pMutate = 0.2;
		double pCrossover = 0.9;

		vector<int> individual;
		population = *createPopulation(population, numOfIndividuals);  // ��ʼ����Ⱥ
		int numOfGeneration = 0;   // �����Ĵ���
		srand((unsigned)time(NULL));
		// ��ʼ����
		do {
			// ������Ⱥ��ÿһ���������Ӧ��ֵ,���������fitness
			calculateFitness(population, fitness);
			vector<vector<int>> newPopulation;
			do {
				// �����ѡ����������Ϊ��ĸ
				vector<vector<int>> parents;
				parents = *select(population, fitness, parents);
				// ֻ�������ŵĶ���
				while (1) {
					// ��ֹ�����棬sonΪ��
					vector<int> son = parents[0];
					// ������֣����������
					// һ�����ʷ������棬��������,������Ӧ�ȸ���
					float pc = rand() % 100 * 1.0 / 100;
					if (pc < pCrossover) {
						son = *(crossover((parents).at(0), (parents).at(1), son));
					}
					// һ�����ʷ������죬�ı����
					float pm = rand() % 100 * 1.0 / 100;
					if (pm < pMutate) {
						son = *mutate(son);
					}
					// ���������ڻ���ڸ�ĸ������ӵ���Ⱥ��
					if (getNumOfNotConflicts(&son) >= getNumOfNotConflicts(&parents[0]) && getNumOfNotConflicts(&son) >= getNumOfNotConflicts(&parents[1])) {
						//cout << getNumOfNotConflicts(&son) << " " << getNumOfNotConflicts(&parents[0]) << " " << getNumOfNotConflicts(&parents[0]) << endl;
						// �����Ӽ����µ���Ⱥ��
						/*if (getNumOfNotConflicts(&son) == 27) {
							printChessboard(son);
							system("pause");
						}*/
						newPopulation.push_back(son);
						son.clear();
						parents.clear();
						break;
					}
					else {
						if (pMutate <= 0.98)
							pMutate += 0.02;
					}
				}
			} while (newPopulation.size() != numOfIndividuals); // ֱ��������Ⱥ��Ŀ���µ�����Ⱥ��ģ

			// ����Ⱥ�������Ⱥ
			population.clear();
			for (int i = 0; i < numOfIndividuals; i++) {
				population.push_back(newPopulation[i]);
			}
			numOfGeneration++;
		} while (positionOfNotConflict(population) == -1);
		// �õ���Ⱥ��������Ӧ�ȵĸ���
		chessboard = population[positionOfNotConflict(population)];
		cout << "total generations: " << numOfGeneration << endl;
		return &chessboard;
	}
};


#endif // !GENTIC_ALGORITHM_H