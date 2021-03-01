#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


/*
��һ���ַ�����ͷ���β����n���ո񣨡�   abcdefgdddd    ����
1)��ȥ���ո����ַ�������
2)ȥ��ǰ��ո񣬷���һ�����ַ���
*/
//1)��ȥ���ո����ַ�������
int main41()
{
	int ncount = 0;
	char* p = "   abcdefgdddd   ";
	int i = 0;
	int j = 0;
	j = strlen(p)-1;

	while (isspace(p[i]) && p[i]!='\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	ncount = j - i + 1;
	printf("ncount:%d\n", ncount);
	return 0;
}

//��ƺ���ģ��
int getCount_space(char *str,int *ncount)
{
	if (str == NULL || ncount == NULL) 
	{
		return -1;
	}
	char* p = str;
	int i = 0;
	int j = 0;
	j = strlen(p) - 1;

	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}

	*ncount = j - i + 1;
	return 0;
}
int main42()
{
	int ncount = 0;
	char* str = "   abcdefgdddd   ";
	int ret = 0;
	ret = getCount_space(str, &ncount);
	if (ret != 0)
	{
		printf("getCount_space(char *str,int *ncount) err:%d\n", ret);
	}
	printf("ncount:%d\n", ncount);
	return 0;
}

//2)ȥ��ǰ��ո񣬷���һ�����ַ���
int trimSpace(char* inbuf, char* outbuf);
int trimSpace2(char* inbuf);

int trimSpace(char* inbuf, char* outbuf)
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return -1;
	}
	char* p = inbuf;
	int i = 0;
	int j = 0;
	int ncount = 0;
	j = strlen(p) - 1;

	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	ncount = j - i + 1;
	strncpy(outbuf, inbuf + i, ncount);
	outbuf[ncount] = '\0';
	return 0;
}
int main43()
{
	int ncount = 0;
	char* str = "   abcdefgdddd   ";
	char buf[1024] = { 0 };
	int ret = 0;
	ret = getCount_space(str, &ncount);
	if (ret != 0)
	{
		printf("func getCount_space(char *str,int *ncount) err:%d\n", ret);
	}
	printf("ncount:%d\n", ncount);

	ret = trimSpace(str, buf);
	if (ret != 0)
	{
		printf("func trimSpace(char* inbuf, char* outbuf) err:%d\n", ret);
	}
	printf("%s\n", buf);
	return 0;
}

//��ʹ���µĿռ䣬��Ȼ��ԭ�����ַ����ռ�
//Ҫ��inbuf��ָ����ڴ�ռ���Ա��޸�
int trimSpace2(char* inbuf)
{
	if (inbuf == NULL)
	{
		return -1;
	}
	char* p = inbuf;
	int i = 0;
	int j = 0;
	int ncount = 0;
	j = strlen(p) - 1;

	while (isspace(p[i]) && p[i] != '\0')
	{
		i++;
	}
	while (isspace(p[j]) && p[j] != '\0')
	{
		j--;
	}
	ncount = j - i + 1;
	strncpy(inbuf, inbuf + i, ncount);
	inbuf[ncount] = '\0';
	return 0;
}
int main44()
{
	int ncount = 0;
	//char* str = "   abcdefgdddd   "; //�ᱨ����Ϊָ����ڴ�ռ䲻��д
	char str[1024] = "   abcdefgdddd   ";
	int ret = 0;
	
	ret = trimSpace2(str);
	if (ret != 0)
	{
		printf("func trimSpace(char* inbuf, char* outbuf) err:%d\n", ret);
	}
	printf("%s\n", str);

	return 0;
}