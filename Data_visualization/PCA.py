from numpy import *

def loadDataSet(fileName, delim='\t'):
    fr = open(fileName)
    stringArr = [line.strip().split(delim) for line in fr.readlines()]
    datArr = [list(map(float,line)) for line in stringArr]
    print("原数据：\n",datArr)
    return mat(datArr)

def pca(dataMat, topNfeat=999999):
    meanVals = mean(dataMat, axis=0)
    DataAdjust = dataMat - meanVals #减去平均值
    #print("样例减去均值后：\n",DataAdjust)

    covMat = cov(DataAdjust, rowvar=0)
    print("协方差矩阵：\n",covMat)

    eigVals,eigVects = linalg.eig(mat(covMat)) #计算特征值和特征向量
    print("特征值：\n",eigVals)
    print("特征向量：\n",eigVects)

    eigValInd = argsort(eigVals)
    print("将特征值的索引由小到大排序后:",eigValInd)
    k=1
    eigValInd = eigValInd[:-(topNfeat+k):-1]   #保留最大的前K个特征值
    print("保留最大的前K个特征值:","k=",k)
    redEigVects = eigVects[:,eigValInd]
    print("对应前K个特征向量\n",eigValInd,"\n",redEigVects)#对应的特征向量

    lowDDataMat = DataAdjust * redEigVects     #将数据转换到低维新空间
    reconMat = (lowDDataMat * redEigVects.T) + meanVals   #重构数据，用于调试
    print("降维后数据：\n",lowDDataMat)


    return lowDDataMat, reconMat

import matplotlib
import matplotlib.pyplot as plt

dataMat = loadDataSet('D:\\Master_bar.txt')
lowDMat, reconMat = pca(dataMat,1)
print ("shape(lowDMat): ",shape(lowDMat))

fig = plt.figure()

'''
plt.xlabel('Professional score')
plt.ylabel('Total score')
plt.axhline(y=311.3,ls=":",c="green")#添加水平直线
plt.axvline(x=136.4,ls="-",c="green")#添加垂直直线
'''
ax = fig.add_subplot(111)#作为单个整数编码的子绘图网格参数
ax.scatter(dataMat[:,0].flatten().A[0],dataMat[:,1].flatten().A[0],marker='^',s=90)
ax.scatter(reconMat[:,0].flatten().A[0],reconMat[:,1].flatten().A[0],marker='o',s=50,c='red')
plt.show()
