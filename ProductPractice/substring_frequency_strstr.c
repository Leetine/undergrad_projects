#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//求字符串中子字符串出现的字数
//例如：char *p = "abcd1111222abcd333444abcd555666aa"
//求字符串p中"abcd"出现的次数

//do-while模型
int main31()
{
	//strstr(str,str2)
	int ncount = 0;
	char *p = "abcd1111222abcd333444abcd555666aa";
	do {
		p = strstr(p,"abcd");
		if (p != NULL)
		{
			ncount++;
			p = p + strlen("abcd");
		}
		else
		{
			break;
		}
	} while (*p!='\0');

	printf("ncount = %d\n", ncount);
	return 0;
}
//while模型
int main32()
{
	//strstr(str,str2)
	int ncount = 0;
	char* p = "abcd1111222abcd333444abcd555666aa";
	while(p = strstr(p, "abcd"))
	{
		ncount++;
		p = p + strlen("abcd");
		if (*p == '\0')
		{
			break;
		}
	}

	printf("ncount = %d\n", ncount);
	return 0;
}

//自定义函数接口(70分)
//封装函数，与测试模型分离(30分)
//求字符串中子字符串出现的字数
//例如：char *p = "abcd1111222abcd333444abcd555666aa"
//求字符串p中"abcd"出现的次数
int getCount(char* str/*in*/, char* sub/*int*/, int* ncount/*out*/);

int getCount(char* str, char* sub, int* ncount)
{
	//参数有效性验证
	if (str == NULL || sub == NULL|| ncount == NULL)
	{
		printf("func getCount() err:%d(str == NULL || sub == NULL|| ncount == NULL)\n", -1);
		return -1;
	}
	//开始
	int tmpCount = 0;
	char* p = str;//不要修改形参的值
	do {
		p = strstr(p, sub);
		if (p != NULL)
		{
			tmpCount++;
			p = p + strlen(sub);
		}
		else
		{
			break;
		}
	} while (*p != '\0');

	*ncount = tmpCount;//指针的间接赋值
	return 0;
}

int main33()
{
	int ret = 0;
	char* p = "abcd1111222abcd333444abcd555666aa";
	char sub[] = "abcd";

	int ncount = 0;
	ret = getCount(p, sub, &ncount);
	if (ret != 0)
	{
		printf("fun getCount(p, sub, &ncount) err:%d\n", ret);
	}
	printf("ncount = %d\n", ncount);
	return 0;
}