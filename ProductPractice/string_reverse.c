#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
	将一个字符串逆序输出
*/
int inverse(char* str)
{
	if (str == NULL)
	{
		return -1;
	}
	int length = strlen(str);

	char* p1 = str;
	char* p2 = str + length - 1;
	while (p1 < p2)
	{
		char c = *p1;
		*p1 = *p2;
		*p2 = c;
		++p1;
		--p2;
	}
	//printf("%s\n", str);
	return 0;

}
int main51()
{
	char buf[] = "abcdefghigklmn";
	int ret = 0;
	ret = inverse(buf);
	if (ret != 0)
	{
		printf("func inverse(char* str) err:%d\n", ret);
		return 0;
	}
	printf("%s\n", buf);
	return 0;
}

//1.通过递归的方式，逆向输出字符串

void inverse02(char* str)
{
	if (str == NULL)//递归结束的异常条件
	{
		return;
	}
	if (*str == '\0')//递归结束的条件
	{
		return;
	}
	inverse02(str + 1);//注意，没有执行打印，而是执行让下一个字符的地址入栈
	printf("%c", *str);
}

int main52()
{
	char buf[] = "abcdefghijk";
	inverse02(buf);
	return 0;
}

//2.递归和全局变量（把逆序的结果存入全局变量）
/*如果该函数被多个线程同时访问，用全局变量有可能出现结果乱序问题*/
char g_buf[100];
void inverse03(char* str)
{
	if (str == NULL)//递归结束的异常条件
	{
		return;
	}
	if (*str == '\0')//递归结束的条件
	{
		return;
	}
	inverse03(str + 1);//注意，没有执行打印，而是执行让下一个字符的地址入栈
	//printf("%c", *str);
	//strcat(g_buf, str);
	strncat(g_buf, str,1);
}
int main53()
{
	char buf[] = "abcdefghijk";
	memset(g_buf, 0, sizeof(g_buf));

	inverse03(buf);

	printf("g_buf:%s\n", g_buf);
	return 0;
}

//3.递归和非全局变量（递归指针函数参数）
void inverse04(char* str,char *result)
{
	if (str == NULL||result==NULL)//递归结束的异常条件
	{
		return;
	}
	if (*str == '\0')//递归结束的条件
	{
		return;
	}
	inverse04(str + 1, result);//注意，没有执行打印，而是执行让下一个字符的地址入栈
	
	strncat(result, str, 1);
}
int main54()
{
	char buf[] = "abcdefghijk";
	char result_buf[1024] = { 0 };

	inverse04(buf,result_buf);

	printf("result_buf:%s\n", result_buf);
	return 0;
}
