#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
//函数指针：一个指针，指向一个函数 
//指针函数： 返回值是指针的函数 
//函数是一种数据类型
//函数：   返回值类型  函数名（函数参数） 
//函数名：函数的入口地址。
//一个函数在编译的时候会分配一个地址，这个地址就是函数的指针 
int add(int a,int b)
{
	return a+b;
} 
//定义函数指针的三种方法：
//1.先声明函数类型，然后通过类型定义指针
void test06()
{
	typedef int(FUNC_TYPE)(int,int);
	FUNC_TYPE *pADD = add;
	int result = add(100,200);
	//用函数指针调用函数 
	int num = pADD(100,100);//(*pADD)(100,100);
	printf("num= %d\n",num);
} 
//2.直接定义函数指针类型
void test07()
{
	typedef int(*FUNC_PTYPE)(int,int);
	FUNC_PTYPE pADD = add;
	int num = pADD(100,100);//(*pADD)(100,100);
	printf("num= %d\n",num);
}
//3.直接定义函数指针变量
void test08()
{
	int(*f)(int,int);
	f = add;
	printf("%d\n",f(100,100));
} 
//函数指针数组
int add1(int a,int b)
{
	return a+b;
}
int add2(int a,int b)
{
	return a+b+10;
}
int add3(int a,int b)
{
	return a+b+20;
}
int add4(int a,int b)
{
	return a+b+30;
}
int add5(int a,int b)
{
	return a+b+40;
}
void test09()
{
	int(*f[])(int,int) = {add1,add2,add3,add4,add5};
	for(int i=0;i<5;i++)
	{
		printf("%d\n",f[i](100,100));
	}
	int(*ff[5])(int,int);
	ff[0] = add1;
	ff[1] = add2;
	ff[2] = add3;
	ff[3] = add4;
	ff[4] = add5;
	for(int i=0;i<5;i++)
	{
		printf("%d\n",ff[i](100,100));
	}
	
} 
//函数指针应用场景：做函数参数（回调函数） 
int allAdd(int a,int b,int(*f)(int,int)) 
{
	return f(a,b);
}
void test10()
{
	int a = 100;
	int b = 100;
	int num = allAdd(a,b,add5);
	printf("%d\n",num);
}


//void printIntArray(int arr[],int len)
//{
//	for(int i=0;i<len;i++)
//	{
//		printf("%d ",arr[i]);
//	}
//}
//void printDoubleArray(double arr[],int len)
//{
//	for(int i=0;i<len;i++)
//	{
//		printf("%lf ",arr[i]);
//	}
//}

//万能打印数组函数：int[] double[] char[] struct Person[] 
typedef void(*print_type)(void*);

void printAllArray(void *arr,int len,int elemsize,print_type print)
{
	char *start = (char *)arr;
	for(int i=0;i<len;i++)
	{
		char *end = start+i*elemsize;
		print(end);
	}
}
//打印int
void printInt(void *data)
{
	int *temp = (int *)data;
	printf("%d ",*temp);
} 
//打印double 
void printDouble(void *data)
{
	double *temp = (double *)data;
	printf("%lf ",*temp);
}
//打印结构体
struct Door
{
	char name[64];
	float price;
};
void printDoor(void *data)
{
	struct Door *temp = (struct Door *)data;
	printf("%s, %f ",temp->name,temp->price);
}
void test11()
{
	int arr[] = {1,2,3,4,5,6,7,8,9};
	int len = sizeof(arr)/sizeof(arr[0]);
	int elemsize = sizeof(arr[0]);
	printAllArray(arr,len,elemsize,printInt);
	printf("\n----------------------------------\n");
	double drr[] = {1.1,2.2,3.3};
	int len2 = sizeof(drr)/sizeof(drr[0]);
	int elemsize2 = sizeof(drr[0]);
	printAllArray(drr,len2,elemsize2,printDouble);
	printf("\n----------------------------------\n");
	struct Door door[3] = {"l1",5.6f,"l2",4.5f,"l3",6.7f};
	int len3 = sizeof(door)/sizeof(door[0]);
	int elemsize3 = sizeof(door[0]);
	printAllArray(door,len3,elemsize3,printDoor);
	
}
int main(int argc, char** argv) {
	test11();
	return 0;
}
