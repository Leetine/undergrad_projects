#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//结构体类型定义
struct Student
{
	char name[64];
	int age;
}s1; //定义类型的同时定义变量 
typedef struct Student2
{
	char name[64];
	int age;
}Student2; 
struct{
	char name[64];
	int age;
}s2;
void test01()
{
	struct Student s = {"lily",20};
	s.age = 40;
	Student2 s3;//在栈上分配的空间 
	s3.age = 22;
	//s3.name = "lucy";
	strcpy(s3.name,"lucy");
	printf("s3: %s  %d\n",s3.name,s3.age);
	//在堆区分配
	Student2 *p = (Student2 *)malloc(sizeof(Student2));
	strcpy(p->name,"tom");
	p->age = 25;
	printf("*p: %s  %d\n",p->name,p->age);
}
//结构体赋值
struct Teacher
{
	char *name;
	int age;
};
void test02()
{
	printf("sizeof(struct Student): %d\n",sizeof(struct Student));
	struct Student s10 = {"lucy",18};
	struct Student s11;
	s11 = s10;	
	printf("s10: %s %d\n",s10.name,s10.age);
	printf("s11: %s %d\n",s11.name,s11.age);
	
	struct Teacher t1;
	t1.name = (char *)malloc(sizeof(char)*64);
	strcpy(t1.name,"lily");
	t1.age = 22;
	printf("t1: %s %d\n",t1.name,t1.age);
	struct Teacher t2;
	//t2 = t1;//浅拷贝 
	t2.name = (char *)malloc(sizeof(char)*64);//深拷贝 
	strcpy(t2.name,t1.name);
	
	printf("t2: %s %d\n",t2.name,t2.age);
	
	if(t1.name!=NULL)
	{
		free(t1.name);
		t1.name = NULL;
	}
	//....
	if(t2.name!=NULL)
	{
		free(t2.name);
		t2.name = NULL;
	}
	
} 
//结构体数组
void test03()
{
	struct Student ss[3] = { {"lucy",20},{"lily",21},{"tom",22}};
	struct Student ss1[3] = { "lucy",20,"lily",21,"tom",22};
	for(int i = 0;i<3;i++)
	{
		printf("name : %s  age: %d\n",ss1[i].name,ss1[i].age);
	}
	//在堆区分配
	struct Student *p = (struct Student *)malloc(sizeof(struct Student)*3);
	for(int i=0;i<3;i++)
	{
		sprintf(p[i].name,"lucy_%d",i+1);
		p[i].age = 20+i;
	}
	for(int i=0;i<3;i++)
	{
		printf("%s %d\n",p[i].name,p[i].age);
	}
} 
//结构体嵌套指针
//被调函数分配空间
void allocate_memeory(struct Teacher ** teacher)
{
	struct Teacher * temp = (struct Teacher *)malloc(sizeof(struct Teacher));
	if(temp==NULL)
	{
		return;
	}
	temp->name = (char *)malloc(sizeof(char)*64);
	strcpy(temp->name,"lucy");
	temp->age = 22;
	
	* teacher = temp;
} 

void printTeacher(struct Teacher * teacher)
{
	printf("%s %d\n",teacher->name,teacher->age);
}
void free_memory(struct Teacher **teacher)
{
	if(NULL==teacher)
	{
		return;
	}
	struct Teacher *t = *teacher;
	if(t->name!=NULL)
	{
		free(t->name);
		t->name = NULL;
	}
	free(t);
}
void test04()
{
	struct Teacher *p = NULL;
	allocate_memeory(&p);
	printTeacher(p);
	free_memory(&p);	
} 
struct Teacher2
{
	char name[64];
	char **students;
};
//结构体的偏移量，内存对齐
//非标准数据类型：内存原则
//1.数组成员对齐规则 第一个元素放在offset为0的位置，后续元素应该放在
//offset为min（当前成员大小，#pargama pack(n)) 的整数倍的地方开始
//2.结构体的大小：sizeof(结构体) 是 min（当前成员大小，#pargama pack(n)) 的整数倍，不足的不齐

typedef struct Person
{
	char a;
	int b;
	int c;
	
} Person;

struct A
{
	int a;
	char b;
	double c;
	float d;
};
 struct B
 {
 	char a;
 	struct A b;
 	double c;
 };
 void test05()
 {
 	printf("offsetof(d): %d\n",offsetof(struct A,d));
 	printf("sizeof(struct B): %d\n",sizeof(struct B));
 }
int main(int argc, char** argv) {
	test05();
	return 0;
}
