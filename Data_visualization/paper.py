import requests
import re
import nltk
import matplotlib.pyplot as plt
import pandas as pd
from wordcloud import WordCloud

# 读取指定 url网页
def getHTMLText(url):
    try:
        r = requests.get(url, timeout=30)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        print("getHTMLText finished")
        return r.text
    except:
        return ""


# 搜索目标信息（题目，作者）存在列表
def parsePage(info, html):
    try:
        _title = re.findall(r'title = {[^\}]+\}', html)  # 绝了，等号前后要加空格，不然匹配不到
        _author = re.findall(r'author = {[^\}]+\}', html)
        for i in range(len(_title)):
            title = _title[i].split('=')[1].replace("{", "").replace("}", "")
            author = _author[i].split('=')[1].replace("{", "").replace("}", "")
            info.append([title, author])
    except:
        print("")


# 打印每条信息，未经过分词
def printPaperList(info):
    tplt = "{:4}\t{:64}\t{:64}"
    print(tplt.format("序号", "题目", "作者"))
    count = 0
    for g in info:
        count += 1
        print(tplt.format(count, g[0], g[1]))


# 1.字符串切割成单词存在alist列表中
# 2.对一维列表处理停用词
# 3.对一维列表计算词频
# 4.将词频字典存储到csv文件中
# 5.画折线图、柱状图、词云
def splitTitleInfo(info):
    # --切割
    alist = []
    for g in info:
        alist.append(re.split(" |: ", g[0]))  # 按照两种方式切割，除去冒号
    alist = [num for elem in alist for num in elem]  # 所有标题每一个单词为列表元素
    while '' in alist:
        alist.remove('')  # 去掉空字符
    combinestr = ["Networks"]  # 只是单复数不同的词，合并
    alist = ["Network" if x in combinestr else x for x in alist]
    print(alist)

    # -- 处理停用词
    clean_tokens = list()
    stopword_list = [k.strip() for k in open('stopwords.txt', encoding='utf8').readlines() if k.strip() != '']
    clean_tokens = [i for i in alist if i not in stopword_list]

    # -- 计算频率
    freq = nltk.FreqDist(clean_tokens)
    for key, val in freq.items():
        print(str(key) + ':' + str(val))

    # -- 将字典存储到csv文件中
    pd.DataFrame(freq,index=[0]).to_csv('hotword.csv')

    # -- 画折线图
    freq.plot(20, cumulative=False)

    # -- 画柱状图
    items = list(freq.items())  # 将词频字典转换为二维数组
    items.sort(key=lambda x: x[1], reverse=True)  # 根据词频进行由高到低排序
    fig, ax = plt.subplots()  # 创建图形
    x = []  # 定义x轴的值和y轴的值
    height = []
    for i in range(20):  # 遍历频率前十的词汇
        a, b = items[i]
        x.append(a)
        height.append(b)
    ax.bar(x, height, 0.5, edgecolor="blue", color="yellow")  # 创建柱状图
    ax.set_title('majority words')  # 设置title和labels
    ax.set_xlabel('word')
    ax.set_ylabel('count')
    plt.xticks(x, rotation=30)
    for i in range(20):  # 标出具体频数
        plt.text(i - 0.25, height[i] + 3, height[i])
    plt.show()

    # -- 画词云图
    wordcloud = WordCloud(  # 创建词云图

        background_color="white",  # 设置背景为白色

        width=1500,  # 设置图片的宽度

        height=960,  # 设置图片的高度

        margin=10,  # 设置图片的边缘

        min_font_size=10,

        max_font_size=220,

        max_words=50,

    ).fit_words(freq)

    plt.imshow(wordcloud, interpolation='bilinear')  # 绘制图片

    plt.axis("off")  # 消除坐标轴

    plt.show()


# 1.字符串切割成单词存在blist列表中
# 2.计算相同人名出现的次数
# 3.将作者字典存储到csv文件中
# 4.画折线图、柱状图
def splitAuthorInfo(info):
    # --切割
    blist = []
    for g in info:
        blist.append(re.split(" and ", g[1]))  # and 连接人名
    blist = [num for elem in blist for num in elem]  # 所有标题每一个单词为列表元素
    print(blist)

    # -- 计算频率
    freq = nltk.FreqDist(blist)
    for key, val in freq.items():
        print(str(key) + ':' + str(val))

    # -- 画图
    freq.plot(20, cumulative=False)

    # -- 将作者字典存储到csv文件中
    pd.DataFrame(freq, index=[0]).to_csv('author.csv')

    # -- 画柱状图
    items = list(freq.items())  # 将词频字典转换为二维数组
    items.sort(key=lambda x: x[1], reverse=True)  # 根据词频进行由高到低排序
    fig, ax = plt.subplots()  # 创建图形
    x = []  # 定义x轴的值和y轴的值
    height = []
    for i in range(20):  # 遍历频率前十的词汇
        a, b = items[i]
        x.append(a)
        height.append(b)
    ax.bar(x, height, 0.5, edgecolor="blue", color="yellow")  # 创建柱状图
    ax.set_title('majority words')  # 设置title和labels
    ax.set_xlabel('word')
    ax.set_ylabel('count')
    plt.xticks(x, rotation=30)
    for i in range(20):  # 标出具体频数
        plt.text(i - 0.25, height[i] + 3, height[i])
    plt.show()

def main():
    infoList = []
    url = 'http://openaccess.thecvf.com/ICCV2019.py'
    html = getHTMLText(url)
    parsePage(infoList, html)
    printPaperList(infoList)
    splitTitleInfo(infoList)
    splitAuthorInfo(infoList)


main()
