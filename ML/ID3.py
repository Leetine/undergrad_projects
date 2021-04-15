import math
import operator
import copy
import numpy as np
import DrawDecisionTREE
import pandas as pd


def createDataSet():
    # dataSet中最后一列记录的是类别，其余列记录的是特征值
    dataSet = [
        ['青绿', '蜷缩', '浊响', '清晰', '凹陷', '硬滑', '是'],
        ['乌黑', '蜷缩', '沉闷', '清晰', '凹陷', '硬滑', '是'],
        ['乌黑', '蜷缩', '浊响', '清晰', '凹陷', '硬滑', '是'],
        ['青绿', '蜷缩', '沉闷', '清晰', '凹陷', '硬滑', '是'],
        ['浅白', '蜷缩', '浊响', '清晰', '凹陷', '硬滑', '是'],
        ['青绿', '稍蜷', '浊响', '清晰', '稍凹', '软粘', '是'],
        ['乌黑', '稍蜷', '浊响', '稍糊', '稍凹', '软粘', '是'],
        ['乌黑', '稍蜷', '浊响', '清晰', '稍凹', '硬滑', '是'],
        ['乌黑', '稍蜷', '沉闷', '稍糊', '稍凹', '硬滑', '否'],
        ['青绿', '硬挺', '清脆', '清晰', '平坦', '软粘', '否'],
        ['浅白', '硬挺', '清脆', '模糊', '平坦', '硬滑', '否'],
        ['浅白', '蜷缩', '浊响', '模糊', '平坦', '软粘', '否'],
        ['青绿', '稍蜷', '浊响', '稍糊', '凹陷', '硬滑', '否'],
        ['浅白', '稍蜷', '沉闷', '稍糊', '凹陷', '硬滑', '否'],
        ['乌黑', '稍蜷', '浊响', '清晰', '稍凹', '软粘', '否'],
        ['浅白', '蜷缩', '浊响', '模糊', '平坦', '硬滑', '否'],
        ['青绿', '蜷缩', '沉闷', '稍糊', '稍凹', '硬滑', '否']
    ]
    # label中记录的是特征的名称
    labels = ['色泽', '根蒂', '敲声', '纹理', '脐部', '触感']

    classList = [example[-1] for example in dataSet]  # 类别列表
    inidata = copy.deepcopy(dataSet)  # 数据
    inilabels = copy.deepcopy(labels)  # 特征

    return dataSet, labels, classList, inidata, inilabels


# 求数据集的信息熵
def calcShannonEnt(dataSet):
    # 计算参与训练的数据量，即训练集中共有多少条数据;用作信息熵概率 p 的分母
    numEntries = len(dataSet)
    # 计算每个分类标签label在当前节点出现的次数，即每个类别在当前节点下别分别出现多少次，用作信息熵概率 p 的分子
    labelCounts = {}

    # 每次读入一条样本数据
    for featVec in dataSet:
        # 将当前实例的类别存储，即每一行数据的最后一个数据代表的是类别
        currentLabel = featVec[-1]

        # 为所有分类创建字典，每个键值对都记录了当前类别出现的次数
        if currentLabel not in labelCounts:
            labelCounts[currentLabel] = 0

        labelCounts[currentLabel] += 1

    # 使用循环方法，依次求出公式求和部分每个类别所占的比例及相应的计算
    shannonEnt = 0  # 记录信息熵的值
    for key in labelCounts:
        # 算出该类别在该节点数据集中所占的比例
        prob = labelCounts[key] / numEntries
        shannonEnt -= prob * math.log(prob, 2)
    print("shannonEnt：" + str(shannonEnt))
    return shannonEnt


'''
1.dataSet为该节点的父节点使用的数据集
2.retDataSet为某特征下去除特定属性后其余的数据
3.value为某个特征下的其中一个属性
4.index为当前操作的是哪一个特征
'''


# 通过遍历dataSet数据集，求出去除父节点特征后的其余特征组合成的数据集
def splitDataSet(dataSet, index, value):
    # 存储划分好的数据集
    retDataSet = []
    for featVec in dataSet:
        if featVec[index] == value:
            reducedFeatVec = featVec[:index]
            reducedFeatVec.extend(featVec[index + 1:])
            retDataSet.append(reducedFeatVec)
    print("集合被划分了一次")
    return retDataSet


# 选择最好的特征划分数据集
def chooseBestFeatureToSplit(dataSet):
    # 求该数据集中共有多少特征（由于最后一列为label标签，所以减1）
    numFeatures = len(dataSet[0]) - 1
    # 将该结点视为叶子节点，计算该节点的信息熵
    baseEntropy = calcShannonEnt(dataSet)
    # 初始化最优信息增益值，和最优的特征索引
    bestInfoGain, bestFeature = 0, -1

    # 依次遍历每一个特征，计算其信息增益
    # 当前数据集中有几列就说明有几个特征
    for i in range(numFeatures):
        # 将数据集中每一个特征下的所有属性拿出来生成一个列表
        featList = [example[i] for example in dataSet]
        # 使用集合对列表进行去重，获得去重后的集合，即：此时集合中每个属性只出现一次
        uniqueVals = set(featList)
        print("第" + str(i) + "个特征对应的uniqueVals:")
        print(uniqueVals)
        # 创建一个临时信息熵
        newEntropy = 0

        # 依次遍历该特征下的所有属性
        for value in uniqueVals:
            # 依据每一个属性划分数据集
            subDataSet = splitDataSet(dataSet, i, value)
            # 计算该属性包含的数据占该特征包含数据的比例
            pro = len(subDataSet) / len(dataSet)
            # 计算每个属性结点的信息熵，并乘以相应的权重，再求他们的和，即为该特征节点的信息熵
            newEntropy += pro * calcShannonEnt(subDataSet)

        print("该特征节点的信息熵:" + str(newEntropy))

            # 计算信息增益
        infoGain = baseEntropy - newEntropy
        print("该特征信息增益:" + str(infoGain))
        print()

        # 比较所有特征中的信息增益，返回最好特征划分的索引值
        if (infoGain > bestInfoGain):
            bestInfoGain = infoGain
            bestFeature = i

    print("最好特征划分的索引值:" + str(bestFeature))
    # 返回最优的特征索引
    return bestFeature


# C45 选择最好的特征划分数据集
def C45_chooseBestFeatureToSplit(dataSet):
    # 求该数据集中共有多少特征（由于最后一列为label标签，所以减1）
    numFeatures = len(dataSet[0]) - 1
    # 将该结点视为叶子节点，计算该节点的信息熵
    baseEntropy = calcShannonEnt(dataSet)
    # 初始化最优增益值率，和最优的特征索引
    bestInfoGainRatio, bestFeature = 0, -1

    # 记录每个特征的信息增益，以列表形式存储
    eachEntropy = []
    # 记录每个特征下的属性数据集占特征数据集的比例
    eachFeaturesPro = []
    # 依次遍历每一个特征，计算其信息增益
    # 当前数据集中有几列就说明有几个特征
    for i in range(numFeatures):
        # 将数据集中每一个特征下的所有属性拿出来生成一个列表
        featList = [example[i] for example in dataSet]
        # 使用集合对列表进行去重，获得去重后的集合，即：此时集合中每个属性只出现一次
        uniqueVals = set(featList)
        print("第" + str(i) + "个特征对应的uniqueVals:")
        print(uniqueVals)
        # 创建一个临时信息熵
        newEntropy = 0
        # 记录该特征下的属性数据集占特征数据集的比例
        eachpro = []

        # 依次遍历该特征下的所有属性
        for value in uniqueVals:
            # 依据每一个属性划分数据集
            subDataSet = splitDataSet(dataSet, i, value)
            # 计算该属性包含的数据占该特征包含数据的比例
            pro = len(subDataSet) / len(dataSet)
            # 计算每个属性结点的信息熵，并乘以相应的权重，再求他们的和，即为该特征节点的信息熵
            newEntropy += pro * calcShannonEnt(subDataSet)
            # 将该属性数据集数量占特征数据集比例加入到字典
            eachpro += [pro]
        print("该特征节点的信息熵:" + str(newEntropy))
        eachFeaturesPro.append(eachpro)

        # 计算该特征信息增益
        infoGain = baseEntropy - newEntropy
        print("该特征信息增益:" + str(infoGain))
        print()

        '''1.把每个特征的信息增益保存起来
           2.求所有特征信息增益的平均值
           3.对信息增益大于平均值的特征求增益率
           4.选择增益率最大的特征'''

        eachEntropy += [infoGain]

    #print("每个特征的信息增益:")
    #print(eachEntropy)
    # 对所有特征信息增益取平均值
    aveEntropy = np.mean(eachEntropy)
    print("所有特征信息增益的平均值:" + str(aveEntropy))

    # 对信息增益大于平均值的特征求增益率
    for j in range(numFeatures):
        # 当所有值相等时，平均值也与他们的值相等
        if eachEntropy[j] >= aveEntropy:
            IVa = 0
            for prob in eachFeaturesPro[j]:  # 计算分母
                IVa -= prob * math.log(prob, 2)
            # 计算增益率
            newGainRatio = eachEntropy[j] / IVa
            print(str(j) + "特征的信息增益率:" + str(newGainRatio))
            # 找到最大的信息增益率和对应的特征
            if newGainRatio >= bestInfoGainRatio:
                bestInfoGainRatio = newGainRatio
                bestFeature = j
    print("最优的特征索引:" + str(bestFeature))
    # 返回最优的特征索引
    return bestFeature


'''classList:为标签列的列表形式'''


# 返回该节点数据集中类别出现次数最多的类别
def majorityCnt(classList):
    classCount = {}
    for vote in classList:
        if vote not in classCount.keys():
            classCount[vote] = 0
        classCount[vote] += 1  # classCount以字典形式记录每个类别出现的次数

    # 倒叙排列classCount得到一个字典集合，然后取出第一个就是结果（好瓜/坏瓜），即出现次数最多的结果
    # sortedClassCount的形式是[(),(),(),...],每个键值对变为元组并以列表形式返回
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)

    return sortedClassCount[0][0]  # 返回的是出现类别次数最多的“类别”


# 训练算法：构造树的数据结构
def createTree(dataSet, labels, FutherClassList, inidata, inilabels):
    # 如果该结点为空集，则将其设为叶子结点，节点类型为其父节点中类别最多的类。
    if len(dataSet) == 0:
        print("结点为空集")
        return majorityCnt(FutherClassList)

    # classList中记录了该节点数据中“类别”一列，保存为列表形式
    classList = [example[-1] for example in dataSet]

    # 第一个停止条件：所有的类标签完全相同，则直接返回该类标签
    # 如果数据集到最后一列的第一个值出现的次数=整个集合的数量，也就说只有一个类别，直接返回结果即可
    if classList.count(classList[0]) == len(classList):
        return classList[0]

    # 第二个停止条件：使用完了所有特征，仍然不能将数据集划分成仅包含唯一类别的分组
    # 如果最后只剩一个特征，那么出现相同label多的一类，作为结果
    if len(dataSet[0]) == 1:  # 所有的特征都用完了，只剩下最后的标签列了
        print("使用完了所有特征，出现相同label多的一类，作为结果")
        return majorityCnt(classList)

    # 选择最优的特征
    bestFeat = chooseBestFeatureToSplit(dataSet)  # 返回的是最优特征的索引
    # 获取最优特征
    bestFeatLabel = labels[bestFeat]
    print("本轮决策树生长的最优特征索引和名称是：" + str(bestFeat) + str(bestFeatLabel))
    print("---------------")
    # 初始化决策树
    myTree = {bestFeatLabel: {}}
    # 将使用过的特征数据删除
    del (labels[bestFeat])
    # 在数据集中去除最优特征列，然后用最优特征的分支继续生成决策树
    indexF = inilabels.index(bestFeatLabel)
    featValues = [example[indexF] for example in inidata]
    uniqueVals = set(featValues)
    # 遍历该特征下每个属性节点，继续生成决策树
    for value in uniqueVals:
        # 求出剩余的可用的特征
        subLabels = labels[:]
        # 子树生长，决策树扩大
        myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet, bestFeat, value),
                                                  subLabels, classList, inidata, inilabels)
    return myTree


# 训练算法：构造树的数据结构 C45
def C45_createTree(dataSet, labels, FutherClassList, inidata, inilabels):
    # 如果该结点为空集，则将其设为叶子结点，节点类型为其父节点中类别最多的类。
    if len(dataSet) == 0:
        print("结点为空集")
        return majorityCnt(FutherClassList)

    # classList中记录了该节点数据中“类别”一列，保存为列表形式
    classList = [example[-1] for example in dataSet]

    # 第一个停止条件：所有的类标签完全相同，则直接返回该类标签
    # 如果数据集到最后一列的第一个值出现的次数=整个集合的数量，也就说只有一个类别，直接返回结果即可
    if classList.count(classList[0]) == len(classList):  # count()函数是统计括号中的值在list中出现的次数
        print("所有的类标签完全相同")
        return classList[0]

    # 第二个停止条件：使用完了所有特征，仍然不能将数据集划分成仅包含唯一类别的分组
    # 如果最后只剩一个特征，那么出现相同label多的一类，作为结果
    if len(dataSet[0]) == 1:  # 所有的特征都用完了，只剩下最后的标签列了
        print("使用完了所有特征，出现相同label多的一类，作为结果")
        return majorityCnt(classList)

    # 选择最优的特征
    bestFeat = C45_chooseBestFeatureToSplit(dataSet)  # 返回的是最优特征的索引
    # 获取最优特征
    bestFeatLabel = labels[bestFeat]
    print("本轮决策树生长的最优特征索引和名称是：" + str(bestFeat) + str(bestFeatLabel))
    print("---------------")
    # 初始化决策树
    C45_myTree = {bestFeatLabel: {}}
    # 将使用过的特征数据删除
    del (labels[bestFeat])

    # 在数据集中去除最优特征列，然后用最优特征的分支继续生成决策树
    indexF = inilabels.index(bestFeatLabel)
    featValues = [example[indexF] for example in inidata]
    uniqueVals = set(featValues)

    # 遍历该特征下每个属性节点，继续生成决策树
    for value in uniqueVals:
        # 求出剩余的可用的特征
        subLabels = labels[:]

        C45_myTree[bestFeatLabel][value] = C45_createTree(splitDataSet(dataSet, bestFeat, value),
                                                          subLabels, classList, inidata, inilabels)

    return C45_myTree

# 测试算法
def classify(inputTree, featLabels, testVec):
    # 拿到当前树模型的根节点
    firstStr = list(inputTree.keys())[0]
    # 拿到当前树模型去除根节点后的分支树
    secondDict = inputTree[firstStr]
    # 返回根节点特征在特征列表中的索引
    featIndex = featLabels.index(firstStr)
    # 在测试集中找到该特征下的属性
    key = testVec[featIndex]
    # 根据该属性找到下一层的特征节点
    valueOfFeat = secondDict[key]
    # 输出查看类别判定过程：根据特征，选择属性
    print('---判定特征：', firstStr)
    #print('某特征下去除特定属性后其余的数据:', secondDict)
    print('测试用例该特征的属性值：', key)
    print('结果：', valueOfFeat)

    # 判断分支是否结束;判断valueOfFeat是否是dict类型
    # 若是dict类型，则表示该节点非叶子节点，继续递归；
    # 若不是dict类型，则表示该节点是叶子节点，已经分类完成，可以打印类别
    if isinstance(valueOfFeat, dict):
        classLabel = classify(valueOfFeat, featLabels, testVec)
    else:
        classLabel = valueOfFeat

    return classLabel


if __name__ == '__main__':
    print("------------------ID3--------------------")
    DDT = DrawDecisionTREE.DrawDecisionTree()

    dataSet, labels, classList, inidata, inilabels = createDataSet()
    myTree = createTree(dataSet, labels, classList, inidata, inilabels)
    print("ID3决策树模型：")
    print(myTree)
    DDT.createPlot(myTree)

    featLabels = ['色泽', '根蒂', '敲声', '纹理', '脐部', '触感']
    testVec = ['青绿', '蜷缩', '沉闷', '清晰', '稍凹', '软粘']
    print("测试集：")
    print(testVec)
    classify(myTree, featLabels, testVec)

    print("------------------C45--------------------")
    DDT = DrawDecisionTREE.DrawDecisionTree()

    dataSet, labels, classList, inidata, inilabels = createDataSet()
    C45_myTree = C45_createTree(dataSet, labels, classList, inidata, inilabels)
    print("C45决策树模型：")
    print(C45_myTree)
    DDT.createPlot(C45_myTree)

    featLabels = ['色泽', '根蒂', '敲声', '纹理', '脐部', '触感']
    testVec = ['青绿', '蜷缩', '沉闷', '清晰', '稍凹', '软粘']
    print("测试集：")
    print(testVec)
    classify(C45_myTree, featLabels, testVec)
