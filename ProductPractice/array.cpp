#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//һά���飬��ά����  ����ָ���ָ������ �ṹ�� 
void test01()
{
	int arr[] = {1,2,3,4,5};
	//���������������� 
	//1.sizeof(arr);
	//2.&arr 
	//���������������������������ָ��������Ԫ�ص�ָ�루����ָ�룩 
	//arr = NULL;
	int *p = arr;
	p += 3;
	printf("%d\n",*p);
	//������±�����Ǹ�����
	printf("%d\n",p[-3]); 
} 
//��ζ���һ���������ͣ���ζ���һ��ָ��ָ��һ����������
void  test02()
{
	int arr[] = {1,2,3,4,5};
	//1.�ȶ����������ͣ�Ȼ����ָ��ָ������
	typedef  int(ARRAY_TYPE)[5];
	ARRAY_TYPE myarray;//int myarray[5];
	for(int i=0;i<5;i++)
	{
		myarray[i] = i+100;
	}
	for(int i=0;i<5;i++)
	{
		printf("%d\n",myarray[i]);
	}
	printf("---------------------------\n");
	int a = 10;
	int *p = &a;
	ARRAY_TYPE* pArray = &myarray;
	//pArray+1;
	//*pArray 
	printf("%d\n",*(*pArray+1));
	
	
	//2.ֱ�Ӷ�������ָ������
	typedef int(*ARRAY_PTYPE)[5]; 
	ARRAY_PTYPE p_array;
	p_array = &myarray;
	printf("%d\n",*(*p_array+4));
	
	//3.ֱ�Ӷ�������ָ�����
	int(*pArrayTemp)[5];
	pArrayTemp = &myarray;
	printf("%d\n",*(*pArrayTemp+2));
} 
//��ά����
void test03()
{
	int arr[3][3] = {
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	int len = sizeof(arr)/sizeof(arr[0][0]);
	int arr2[3][3] = {1,2,3,4,5,6,7,8,9};
	int arr3[][3] = {1,2,3,4,5,6};
	//���֤����ά�������Դ洢
	int *p = (int *)arr;
	for(int i=0;i<len;i++)
	{
		printf("%d\n",p[i]);
	} 
} 
//��ά����������������������ʽ
void printArray(int arr[3][3])
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
}
void printArray2(int arr[][3])
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
}
void printArray3(int (*arr)[3])
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
}
void test04()
{
	int arr[3][3] = {
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	int len = sizeof(arr)/sizeof(arr[0][0]);
	printArray3(arr);
} 
//ָ�����飺һ��Ԫ��Ϊָ�������
//ջ��ָ������
void array_print(char **p,int len)
{
	for(int i=0;i<len;i++)
	{
		printf("%s  ",p[i]);
	}
	printf("\n");
}
void array_sort(char **arr,int len)
{
	for(int i=0;i<len;i++)
	{
		for(int j=len-1;j>i;j--)
		{
			if(strcmp(arr[j-1],arr[j])>0)
			{
				char* tem = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = tem;
			}
		}
	}
}
void test06()
{
	char *p[] = {"bbb","ccc","aaaa","ddd"};
	//char **p = {"bbb","ccc","aaaa","ddd"};//���� 
	int len = sizeof(p)/sizeof(char *);
	array_print(p,len);
	array_sort(p,len);
	array_print(p,len);
} 
//����ָ������
char ** allocate_memory(int n)
{
	if(n <= 0)
	{
		return NULL;
	}
	char **temp = (char **)malloc(sizeof(char *)*n);
	if(temp == NULL)
	{
		return NULL;
	}
	//�ֱ��ÿһ��ָ�����ռ�
	for(int i=0;i<n;i++)
	{
		temp[i] = (char *)malloc(sizeof(char)*64);
		sprintf(temp[i],"%d_hello",i+1); 
    }
    return temp;
}
void freeArray(char **arr,int len)
{
	if(NULL == arr)
	{
		return ;
	}
	for(int i=0;i<len;i++)
	{
		free(arr[i]);
		arr[i] = NULL;
	}
	free(arr);
}
void test07()
{
	int n = 10;
	char **p = allocate_memory(n);
	array_print(p,n);
	freeArray(p,n);
	p = NULL;
} 
//ָ�������Ӧ�ó���
/*
*�ҳ��ؼ����ڱ��е�λ��
*"do" "for" 
*/

int searchKey(const char* table[]/*in*/,const char *key/*in*/,const int size/*in*/,int *pos/*out*/)
{
	if(NULL==table || NULL==key || NULL==pos)
	{
		return -1;
	}
	int i=0;
	for(i=0;i<size;i++)
	{
		if(strcmp(key,table[i])==0)
		{
			*pos = i;
			return 0;
		}
	}
	if(i==size)
	{
		*pos = -1;
	}	
	return -2;
}
void test09()
{
	int pos = 0;
	char *table[] = {
		"do",
		"for",
		"case",
		"return"
	};
	int size = sizeof(table)/sizeof(*table);
	int ret = 0;
	ret = searchKey((const char **)table,"case",size,&pos);
	if(ret!=0)
	{
		printf("func searchKey error: %d\n",ret);
		return;
	}
	printf("pos= %d\n",pos);
}
//��ƾ������ҽ���������ָ������
void test10()
{
	char *table[] = {
		"do",
		"for",
		"case",
		"return",
		0
	};
	char *table2[] = {
		"do",
		"for",
		"case",
		"return",
		'\0'
	};
	char *table3[] = {
		"do",
		"for",
		"case",
		"return",
		NULL
	};
	for(int i=0;table3[i]!=NULL;i++)
	{
		printf("%s\n",table3[i]);
	}
}
int main(int argc, char** argv) {
	test02();
	return 0;
}








