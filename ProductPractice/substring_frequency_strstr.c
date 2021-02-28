#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//���ַ��������ַ������ֵ�����
//���磺char *p = "abcd1111222abcd333444abcd555666aa"
//���ַ���p��"abcd"���ֵĴ���

//do-whileģ��
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
//whileģ��
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

//�Զ��庯���ӿ�(70��)
//��װ�����������ģ�ͷ���(30��)
//���ַ��������ַ������ֵ�����
//���磺char *p = "abcd1111222abcd333444abcd555666aa"
//���ַ���p��"abcd"���ֵĴ���
int getCount(char* str/*in*/, char* sub/*int*/, int* ncount/*out*/);

int getCount(char* str, char* sub, int* ncount)
{
	//������Ч����֤
	if (str == NULL || sub == NULL|| ncount == NULL)
	{
		printf("func getCount() err:%d(str == NULL || sub == NULL|| ncount == NULL)\n", -1);
		return -1;
	}
	//��ʼ
	int tmpCount = 0;
	char* p = str;//��Ҫ�޸��βε�ֵ
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

	*ncount = tmpCount;//ָ��ļ�Ӹ�ֵ
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