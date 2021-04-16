# Net数目
rows=9

# 打印路由表
def Output_Chart(list):
    print("目的网络  距离  下一跳")
    for i in range(rows):
        if(list[i][0] == "-1"): continue  # 若没有router相应的信息，不输出
        else:                             # 下标 0 1 2 分别表示目的网络、距离、下一跳
            print(list[i][0] + "     "+list[i][1] + "     "+list[i][2] + "     ")

# 距离向量算法，根据原路由和修改后的RIP报文得到更新的路由表
def Dis_Vec(listB,listC_new):
    for row in range(rows):
        if listB[row][0] != "-1" and listC_new[row][0] == "-1":  # RIP报文没有新信息则不变
            continue
        elif listB[row][0] == "-1" and listC_new[row][0] != "-1":  # 原来的路由表中没有目的网络N而C中有
            listB[row][0] = listC_new[row][0]                      # 把该项目添加到路由表中
            listB[row][1] = listC_new[row][1]
            listB[row][2] = listC_new[row][2]
        elif listB[row][0] != "-1" and listC_new[row][0] != "-1":
            if listB[row][2] == listC_new[row][2]:  # 在路由表中有目的网络N，且下一跳路由器是C
                listB[row][1] = listC_new[row][1]   # 用收到的距离替换原路由表中的距离
            else:  # 在路由表中有目的网络N，但下一跳路由器不是C，比较距离与路由表中的距离
                if listB[row][1] <= listC_new[row][1]:   # 收到的项目中的距离大于或等于路由表中的距离，则不变
                    continue
                elif listB[row][1] > listC_new[row][1]:  # 收到的项目中的距离小于路由表中的距离，则更新
                    listB[row][1] = listC_new[row][1]
                    listB[row][2] = listC_new[row][2]
    print("经过计算，路由器B更新后的的路由表：")
    Output_Chart(listB)  # 输出B更新后的路由表

def main():
    file_b = "C:\\Users\\LPC\\Desktop\\B.txt"
    print("正在从 " + file_b + " 读取路由器B的路由表：")
    f = open(file_b, "r")
    chartB = f.read().split("\n")
    for i in range(len(chartB)):
        chartB[i] = chartB[i].split()  # 读取每一行以空格隔开的数据
    print("路由器B的路由表：")
    Output_Chart(chartB)
    file_c = "C:\\Users\\LPC\\Desktop\\C.txt"
    print("正在从 " + file_c + " 读取路由器C发来的RIP报文：")
    f = open(file_c, "r")
    chartC = f.read().split("\n")
    for i in range(len(chartC)):
        chartC[i] = chartC[i].split()
    print("C发来的RIP报文：")
    Output_Chart(chartC)
    chartC_new = list(chartC)
    for row in range(rows):
        chartC_new[row][1]=str(int(chartC_new[row][1])+1)  # 距离都增加一，str转为int类型才可以加一
        chartC_new[row][2] = 'C'  # 将下一跳地址改为C
    print("修改后的RIP报文：")
    Output_Chart(chartC_new)
    Dis_Vec(chartB, chartC_new)  # 使用距离向量算法更新B的路由表，并输出B更新后的路由表

if __name__ == '__main__':
    main()