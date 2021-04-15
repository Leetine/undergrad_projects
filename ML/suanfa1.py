import random
import math
import matplotlib.pyplot as plt
import numpy as np


class GA(object):

    # 初始化种群 生成chromosome_length大小的population_size个个体的种群
    def __init__(self, population_size, chromosome_length, pc, pm, x_bounder):

        self.population_size = population_size  # 种群大小
        self.choromosome_length = chromosome_length  # 染色体长度，基因的个数
        self.x_bounder = [x_bounder[0], x_bounder[1]]  # 自变量范围
        self.population = [[]]  # 种群，二维列表
        self.max_value = x_bounder[1]
        self.pc = pc  # 交叉概率
        self.pm = pm  # 突变概率
        self.fitness_value = []  # 个体适应值列表

    # 五个测试函数
    def f(self, x):
        # return x
        return -(x - 15) * (x - 15) + 2
        # return 10 + x * np.sin(4 * x)
        # return x + 7 *np.cos(4 * x)
        # return 10 * np.sin(5 * x) + 7 * np.cos(4 * x)

    # 种群初始化，将问题由参数空间转到编码空间
    def species_origin(self):
        population = [[]]  # 定义空的种群二维列表
        # 对于每一个个体，每一位基因都随机初始化为二进制数，这样随机产生种群
        for i in range(self.population_size):
            # 染色体暂存器
            temporary = []
            # 随机产生一个染色体,由二进制数组成
            for j in range(self.choromosome_length):
                temporary.append(random.randint(0, 1))
            # 将染色体添加到种群中
            population.append(temporary)
        # 将种群返回，种群是个二维数组，个体和染色体两维
        return population[1:]

    # 从二进制到十进制
    # input:种群
    def translation(self, population):
        temporary = []  # 一维数组，存放每个个体编码后的十进制数
        for i in range(len(population)):
            total = 0  # 转换后的十进制数
            for j in range(self.choromosome_length):
                total += population[i][j] * (math.pow(2, j))
            # 从第一个基因开始，每位对2求幂，再求和
            # 如：101 转成十进制为：1 * 2^0 + 0 * 2^1 + 1 * 2^2 = 1 + 0 + 4 = 5
            temporary.append(total)  # 一个染色体编码完成，由一个二进制数编码为一个十进制数
        return temporary  # 返回种群中所有个体编码完成后的十进制数

    # 适应度计算，目标函数相当于环境对染色体进行筛选
    def function(self, population):
        function1 = []
        temporary = self.translation(population)  # 某种群全部个体转为十进制后的一维数组
        for i in range(len(temporary)):  # 对于每一个个体
            # 一个基因代表一个决策变量，其算法是先转化成十进制，然后再除以2的基因个数次方减1(固定值)
            # 是一种编码规则，如果选择好的适应度函数，优化速度和平滑性会比较好
            x = temporary[i] * self.max_value / (math.pow(2, self.choromosome_length) - 1)
            function1.append(self.f(x))  # 计算适应值函数，因为是求目标函数最大值，所以前者就直接设为后者
        return function1

    # 选择操作，只保留非负值的适应度/函数值
    def fitness(self, function1):
        # 存放每个个个体适应值的一维数组
        fitness_value = []
        # num 其实就是种群大小
        num = len(function1)
        for i in range(num):  # 对于每一个个体
            if function1[i] > 0:  # 如果个体适应度大于0,则不变
                temporary = function1[i]
            else:
                temporary = 0.0  # 如果个体适应度小于0,则定为0
            fitness_value.append(temporary)  # 把个个体适应值添加到列表
        # 循环，直到遍历种群每个个体
        return fitness_value

    # 首先，计算适应度和
    def sum(self, fitness_value):
        total = 0
        for i in range(len(fitness_value)):
            total += fitness_value[i]
        return total

    # 其次，计算适应度斐伯纳契列表，这里是为了求出累积的适应度
    def cumsum(self, fitness1):
        for i in range(len(fitness1) - 2, -1, -1):
            # range(start,stop,[step])
            # 倒计数
            total = 0
            j = 0
            while (j <= i):
                total += fitness1[j]
                j += 1
            # 将所有个体的适应度正则化，将适应度划分成区间
            fitness1[i] = total
            fitness1[len(fitness1) - 1] = 1

    # 选择种群中个体适应度最大的个体
    def selection(self, population, fitness_value):
        new_fitness = []  # 单个公式暂存器
        total_fitness = self.sum(fitness_value)  # 将所有的适应度求和
        for i in range(len(fitness_value)):  # 每个个体的适应值除以总和体现个体优劣
            new_fitness.append(fitness_value[i] / total_fitness)
        self.cumsum(new_fitness)  # 将所有个体的适应度正则化
        ms = []  # 存活的种群
        population_length = pop_len = len(population)  # 求出种群长度
        for i in range(pop_len):  # 根据随机数确定哪几个能存活
            ms.append(random.random())  # 产生种群个数的随机值
        ms.sort()  # 存活的种群排序
        fitin = 0
        newin = 0
        new_population=new_pop = population  # 先令子种群等于父种群
        # 以轮盘赌方式确定繁殖种群
        while newin < pop_len:  # 对于每一个个体
            # 如果这个个体对应产生的随机值小于适应值，保留此个体
            # 某个体适应度越大,新的种群中该个体数量越多
            if ms[newin] < new_fitness[fitin]:
                new_pop[newin] = population[fitin]
                newin += 1
            else:
                fitin += 1
        population = new_pop  # 用新种群替换父种群

    # 交叉操作，生成新的交叉个体
    def crossover(self, population):
        pop_len = len(population)  # 种群大小
        for i in range(pop_len - 1):  # 对于每一个个体，以一定的概率发生交叉事件
            # 有 pc大小的概率发生交叉操作
            if random.random() < self.pc:
                # 在染色体长度内个数内随机生成单点交叉点cpoint,如长度为10，可能在cpoint=6处交叉
                cpoint = random.randint(0, len(population[0]))
                # 将tmporary1作为暂存器，暂时存放第i个染色体中的前0到cpoint个基因，
                # 然后再把第i+1个染色体中的后cpoint到第i个染色体中的基因个数，补充到temporary2后面
                temporary1 = []
                temporary2 = []
                temporary1.extend(population[i][0:])
                temporary1.extend(population[i + 1][:len(population[i])])
                # 将tmporary2作为暂存器，暂时存放第i+1个染色体中的前0到cpoint个基因，
                # 然后再把第i个染色体中的后cpoint到第i个染色体中的基因个数，补充到temporary2后面
                temporary2.extend(population[i + 1][0:])
                temporary2.extend(population[i][:len(population[i])])
                population[i] = temporary1
                population[i + 1] = temporary2
        # 第i个染色体和第i+1个染色体交叉完成

    # 突变操作，在基因层面变动
    def mutation(self, population):
        # 求出种群中所有个体的个数
        px = len(population)
        # 个体基因的个数
        py = len(population[0])
        # 对于每个个个体
        for i in range(px):
            # 有 pm大小的概率发生交叉操作
            if random.random() < self.pm:
                # 在基因个数的范围内，随机产生突变位点
                mpoint = random.randint(0, py - 1)
                # 将mpoint的基因进行单点随机变异，1变为0，0变为1
                if (population[i][mpoint] == 1):
                    population[i][mpoint] = 0
                else:
                    population[i][mpoint] = 1

    # 将每一个染色体都转化成十进制，max_value为基因最大值，为了后面画图用
    def b2d(self, best_individual):
        total = 0
        b = len(best_individual)  # 染色体长度,每繁殖一代都产生一个最好个体
        for i in range(b):  # range表示基因个数，对最好个体，按下面的公式将二进制转为十进制
            total = total + best_individual[i] * math.pow(2, i)  # 第i位权值为2的i次方
        total = total * self.max_value / (math.pow(2, self.choromosome_length) - 1)
        return total

    # 寻找最好的适应度和个体
    def best(self, population, fitness_value):

        px = len(population)  # 种群大小
        bestindividual: list = []
        bestfitness = fitness_value[0]  # 最好适应度初始化为第一个个体的适应度
        # print(fitness_value)
        for i in range(1, px):
            # 循环找出最大的适应度，适应度最大的也就是最好的个体
            if fitness_value[i] > bestfitness:
                bestfitness = fitness_value[i]  # 这个个体的适应度作为最好适应度
                bestindividual= population[i]  # 这个个体作为最好个体（一条染色体）
        return [bestindividual, bestfitness]

    def plot(self, results):
        X = []
        Y = []
        for i in range(Iteration):  # 绘图，演化代数x范围 0-Iteration
            X.append(i)  # y 是一次繁殖后的最好个体适应值
            Y.append(results[i][0])
        plt.plot(X, Y)
        plt.show()

    def main(self):
        sum = 0.0
        average = 0.0
        dev = 0.0
        best_f = []
        for t in range(n_runtimes):  # 多次运行求最优解的平均值和方差
            print(t)
            results = [[]]
            population = pop = self.species_origin()  # 每次运行都要重新随机生成初始种群
            for i in range(Iteration):  # 演化代数
                function_value = self.function(population)  # 目标函数
                fitness_value = self.fitness(function_value)  # 适应值，由不好逐渐向最优解收敛
                best_individual, best_fitness = self.best(population, fitness_value)
                results.append([best_fitness, self.b2d(best_individual)])
                # 将最好的个体和最好的适应度保存，并将最好的个体转成十进制
                self.selection(population, fitness_value)  # 选择
                self.crossover(population)  # 杂交
                self.mutation(population)  # 变异
            results = results[1:]
            results.sort()
            self.plot(results)  # 绘图，横轴演化代数，纵轴函数值
            print("本次运算的函数最优个体是" + str(results[i][1]))
            print("本次运算的函数最优解是" + str(results[i][0]))  # 演达到化代数后输出本次求得的最优解
            best_f.append(results[i][0])  # 保存单次运行的最优解，为了画收敛图
            sum += results[i][0]   # 累加单次运行的最优解，为了求方差和平均值
            # 绘图
            x = np.linspace(start=ga.x_bounder[0], stop=ga.x_bounder[1], num=200)
            plt.plot(x, self.f(x))  # 绘制测试函数，在定义域内
            x0 = results[i][1]  # 绘制本次运行迭代结束后最终求得的最优解，标为红色
            plt.scatter(x0, self.f(x0), s=40, lw=0, c='red', alpha=1)
            plt.show()
        average = sum / n_runtimes  #计算平均值
        for num in best_f:
            dev = dev + (num - average) ** 2
        dev = pow(dev / (len(best_f) - 1), 0.5)  # 计算方差
        print("平均值：" + str(average))
        print("方差：" + str(dev))


if __name__ == '__main__':
    Iteration = 500
    # 迭代次数
    n_runtimes = 10
    # 遗传算法是一种随机算法，同一函数，同一参数的程序，重复运行多次对每一次求得的最优解取平均、计算方差
    population_size = 100  # 种群大小
    chromosome_length = 10  # 染色体长度
    pc = 0.7  # 交叉概率
    pm = 0.1  # 突变概率
    x_bounder = [10, 20]  # 自变量范围
    ga = GA(population_size, chromosome_length, pc, pm, x_bounder)
    ga.main()
