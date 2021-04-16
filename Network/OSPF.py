# 对于计算机而言，无穷可以用一个相对大很多的数来表示
MAX=100

# dijkstra算法求最短路径
def dijkstra(network, node):
    # 判断图是否为空，如果为空直接退出
    if network is None:
        return None
    nodes = [i for i in range(len(network))]  # 获取图中所有节点
    distance = dict()  # 最短距离
    path = {node: {node: []}}  # 最短路径
    forwarding_table = {node: {node: []}}  # 转发表
    N = list()  # 用于存放已访问过的节点
    n0 = 0
    n1 = 0
    for i in nodes:  # 定义节点到起点的距离
        distance[i] = network[i][node]
    nodes.remove(node)
    N.append(node)

    while nodes:  # 当还有结点没有访问过时
        try:
            least_dist = MAX
            for p in N:  # 加进一个点后，计算其相邻所有点的最短路径
                for q in nodes:
                    new_dist = network[node][p] + network[p][q]
                    if new_dist <= least_dist:  # 比较
                        network[node][q] = new_dist
                        least_dist = new_dist  # 更新最短距离
                        n1 = p
                        n0 = q
                    else:
                        pass
            distance[n0] = least_dist  # 赋予最短距离
            path[node][n0] = [i for i in path[node][n1]]
            path[node][n0].append(n0)  # 添加最短路径
            nodes.remove(n0)  # 未访问的结点减少
            N.append(n0)  # 已访问过的点增加
        except:
            break
    nodes = [i for i in range(len(network))]
    nodes.remove(node)

    for i in nodes:  # 转发表
        forwarding_table[node][i] = [i for i in forwarding_table[node][node]]
        # 到某个节点最短路径的第一个节点作为路由器下一跳
        forwarding_table[node][i].append (path[node][i][0])

    next = forwarding_table[node]
    next=sorted(next.items(),key=lambda x:x[0])  # sorted函数按key值对字典排序

    return distance, next

# 索引为字符串，对应节点路由
def toletter(a):
    b = str(a)
    # 替换
    b = b.replace('0', 'A')
    b = b.replace('1', 'B')
    b = b.replace('2', 'C')
    b = b.replace('3', 'D')
    b = b.replace('4', 'E')
    b = b.replace('5', 'F')
    b = b.replace('[]', '-')
    b = b.replace('[', '')
    b = b.replace(']', '')
    b = b.replace(',', '')
    b = b.replace(' ', '')
    return b

# 程序输入的字符转为索引，且便于循环结束条件的判断
def tonumber(a):
    b=str(a)
    b = b.replace('A', '0')
    b = b.replace('B', '1')
    b = b.replace('C', '2')
    b = b.replace('D', '3')
    b = b.replace('E', '4')
    b = b.replace('F', '5')
    b = b.replace('#', '-1')
    b = int(b)
    return b

# 输出路由表，路由表由二维列表表示
def Output_Chart(list):
    print("目的网络  距离  下一跳")
    for i in range(len(list)):
        print(list[i][0] + "       "+str(list[i][1]) + "       "+list[i][2] + "     ")

if __name__ == '__main__':
    N=eval(input("请输入结点个数:"))  # 给定二维列表大小
    # 用一个二维列表network[i][j]来存从第i个点到第j个点的路径权重
    network = [[MAX for _ in range(N)] for _ in range(N)]  # 先全部初始化为MAX，无穷大
    for i in range(N):
        network[i][i]=0  # 到自身的距离为0
    for i in range(N):   # 接下来根据输入的链路信息更新二维列表
        print("请输入链路状态分组,输入“# 0”结束：")
        print(toletter(i)+":")  # toletter(i)对应范围：第1个字母A-第N个字母
        while True:
            a, b = map(str, input(">>").split())  # 键入链路状态，以空格隔开
            a=tonumber(a)
            if a==-1 or b==0:  # 输完一个节点邻接信息后
                print(network[i])   # 用一个一维列表network[i]表示从第i个节点到各结点的最短路径
                break
            else:
                network[i][a] = int(b)  # 没有输完就继续，更新距离为b

    print("邻接矩阵：")
    for i in range(N):
        print(network[i])  # 所有结点的邻接信息已输完

    for i in range(len(network)):  # 计算每个结点的路由表
        print("["+toletter(i)+"]的路由表:")
        distance, next = dijkstra(network, i)  # 查找从源点开始到其他节点的最短路径
        distance_list = list(distance.items())  # 将distance字典转为距离列表

        nodes = [i[0] for i in distance_list]  # 距离列表的第一列是->节点路由列表
        nodes = list(toletter(nodes))

        weight = [i[1] for i in distance_list]  # 距离列表的第二列是->距离列表

        nextjump = [i[1] for i in next]  # 将next字典的value部分转为->下一跳列表
        nextjump = list(toletter(nextjump))

        chart = list(zip(nodes, weight, nextjump))  # 将三个列表合并为->路由表
        Output_Chart(chart)  # 输出该节点路由表
