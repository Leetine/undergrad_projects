#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
	��һ���ַ����������
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

//1.ͨ���ݹ�ķ�ʽ����������ַ���

void inverse02(char* str)
{
	if (str == NULL)//�ݹ�������쳣����
	{
		return;
	}
	if (*str == '\0')//�ݹ����������
	{
		return;
	}
	inverse02(str + 1);//ע�⣬û��ִ�д�ӡ������ִ������һ���ַ��ĵ�ַ��ջ
	printf("%c", *str);
}

int main52()
{
	char buf[] = "abcdefghijk";
	inverse02(buf);
	return 0;
}

//2.�ݹ��ȫ�ֱ�����������Ľ������ȫ�ֱ�����
/*����ú���������߳�ͬʱ���ʣ���ȫ�ֱ����п��ܳ��ֽ����������*/
char g_buf[100];
void inverse03(char* str)
{
	if (str == NULL)//�ݹ�������쳣����
	{
		return;
	}
	if (*str == '\0')//�ݹ����������
	{
		return;
	}
	inverse03(str + 1);//ע�⣬û��ִ�д�ӡ������ִ������һ���ַ��ĵ�ַ��ջ
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

//3.�ݹ�ͷ�ȫ�ֱ������ݹ�ָ�뺯��������
void inverse04(char* str,char *result)
{
	if (str == NULL||result==NULL)//�ݹ�������쳣����
	{
		return;
	}
	if (*str == '\0')//�ݹ����������
	{
		return;
	}
	inverse04(str + 1, result);//ע�⣬û��ִ�д�ӡ������ִ������һ���ַ��ĵ�ַ��ջ
	
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
