#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
//字符串基本操作
//字符串：以0或者'\0'结尾的字符数组。(数字0 和 字符'\0'等价)
//字符串的长度：字符串中字符的个数 
void test03()
{
	char str1[] = { 'h','e','l','l','o' };		//大小为5
	printf("%s\n", str1);

	//字符数字部分初始化，剩余补0 
	char str2[100] = { 'h','e','l','l','o' };
	printf("%s\n", str2);

	char str3[] = "hello";//大小为6，长度为5，最常用的方法
	printf("%s\n", str3);
	printf("sizeof(str1): %d\n", sizeof(str1));
	printf("sizeof(str3): %d\n", sizeof(str3));
	printf("strlen(str3):%d\n", strlen(str3));

	char str4[100] = "hello";
	printf("str4: %s\n", str4);
	printf("sizeof(str4): %d\n", sizeof(str4));
	printf("strlen(str4):%d\n", strlen(str4));

	char str5[] = "hello\0world";
	printf("str5: %s\n", str5);
	printf("sizeof(str5): %d\n", sizeof(str5));//字符串，遇到\0就自动结束
	printf("strlen(str5):%d\n", strlen(str5));

	char str6[] = "hello\0x12world";
	printf("str6: %s\n", str6);
	printf("sizeof(str6): %d\n", sizeof(str6));
	printf("strlen(str6):%d\n", strlen(str6));

}
//字符串拷贝 strcpy()
void test04()
{
	//源
	char str[] = "hello";
	//目的 
	char buf[64];
	int i = 0;
	for (i = 0; *(str + i) != '\0'; i++)
	{
		buf[i] = str[i];
	}
	buf[i] = '\0';//别忘了最后一步

	printf("%s\n", buf);
}

void myCopy2(const char *str/*in*/, char *des/*out*/)
{
	if (NULL == str)
	{
		return;
	}
	if (NULL == des)
	{
		return;
	}
	char *tempStr = (char *)str;
	char *tempDes = des;
	while (*tempDes++ = *tempStr++);
}
void test05()
{
	//源
	char str[] = "hello";
	//目的 
	char buf[64] = { 0 };
	//memset(buf,0,64)
	myCopy2(NULL, buf);
	printf("%s\n", buf);
}

//字符串反转 "abcde"   "edcba"
//常用API
void test06()
{
	//1.格式化字符串 sprintf 
	char buf[1024] = { 0 };
	sprintf(buf, "hello,%s,欢迎加入我们！", "lucy");
	printf("%s\n", buf);

	//拼接字符串
	memset(buf, 0, 1024);
	char str1[] = "hello";
	char str2[] = "world";
	int num = sprintf(buf, "%s%s", str1, str2);
	printf("%s\n", buf);
	printf("%d\n", num);

	//设置宽度，对齐方式
	memset(buf, 0, 1024);
	int number = 8;
	sprintf(buf, "0000%o", number);
	printf("%s\n", buf);
	//sscanf
}
//strstr
/*
一个字符串，它的开头或结尾有若干个空格 “    adbfmnejdndk      ” ；
**
1.求去掉空格后字符串的长度
2.求去掉空格后的字符串
3.求去掉空格后的字符串，要求不额外增加空间
*/
//1.求去掉空格后字符串的长度
/**/
int getCount(const char *str/*in*/, int *count/*out*/);
int getCount(const char *str/*in*/, int *count/*out*/)
{
	if (NULL == str || NULL == count)
	{
		return -1;
	}
	const char *p = str;
	int i = 0;
	int j = strlen(str) - 1;
	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	*count = j - i + 1;
	return 0;
}
void test07()
{
	int count = 0;
	const char *str = "    abcdeffff       ";
	int ret = getCount(str, &count);
	if (ret != 0)
	{
		printf("func int getCount(const char *str/*in*/,int *count/*out*/) error: %d\n", ret);
		return;
	}
	printf("count: %d\n", count);
}
//2.求去掉空格后的字符串
int mytrim(const char *str/*in*/, char *outbuf/*out*/)
{
	if (NULL == str || NULL == outbuf)
	{
		return -1;
	}
	const char *p = str;
	int i = 0;
	int j = strlen(str) - 1;
	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	int count = j - i + 1;
	strncpy(outbuf, str + i, count);
	outbuf[count] = '\0';
	return 0;

}
void test08()
{
	int count = 0;
	const char *str = "      abcdeffff       ";
	//char outbuf[1024] = {0};
	char *outbuf = NULL;
	outbuf = (char *)malloc(sizeof(char) * 1024);
	int ret = mytrim(str, outbuf);
	if (ret != 0)
	{
		printf("func int mytrim(const char *str/*in*/,char *outbuf/*out*/) error: %d\n", ret);
		return;
	}
	printf("%s\n", outbuf);
	if (outbuf != NULL)
	{
		free(outbuf);
		outbuf = NULL;
	}
}
//3.求去掉空格后的字符串，要求不额外增加空间 
int mytrim3(char *str/*in-out*/);
int mytrim3(char *str/*in-out*/)
{
	if (NULL == str)
	{
		return -1;
	}
	const char *p = str;
	int i = 0;
	int j = strlen(str) - 1;
	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	int count = j - i + 1;
	strncpy(str, str + i, count);
	str[count] = '\0';
	return 0;
}
void test09()
{
	int count = 0;
	char str[1024] = "      abcdeffff       ";
	//str[1] = 'a';
	int ret = mytrim3(str);
	if (ret != 0)
	{
		printf("func int mytrim(const char *str/*in*/,char *outbuf/*out*/) error: %d\n", ret);
		return;
	}
	printf("%s\n", str);

}
int main(int argc, char** argv) {
	test06();
	return 0;
}








