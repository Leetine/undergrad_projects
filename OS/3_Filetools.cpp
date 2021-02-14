#include"pch.h"
//#include <bits/stdc++.h>
#include<iostream>
using namespace std;

void creat()
{
	cout << "file name >> ";
	char name[30], order[65];
	cin >> name;


	strcpy(order, "touch ");
	strcat(order, name);
	//cout << order << endl;
	system(order);

}

void write()
{
	char name[30];
	cout << "file name >> ";
	cin >> name;

	char order[65];
	strcpy(order, "vim ");
	strcat(order, name);
	system(order);
}

void read()
{
	write();
}

void copy()
{
	char from[50], to[50];
	cout << "from where to where >> ";
	cin >> from;
	cin >> to;

	char order[105];

	strcpy(order, "cp ");
	strcat(order, from);
	strcat(order, " ");
	strcat(order, to);

	system(order);

}

void chmod()
{
	char name[30];
	cout << "file name >> ";
	cin >> name;

	char power[30];
	cout << "power >> ";
	cin >> power;

	char order[60];
	strcpy(order, "chmod ");
	strcat(order, power);
	strcat(order, " ");
	strcat(order, name);

	system(order);
}

void check()
{
	char order[65];
	strcpy(order, "ls -l ");
	char name[35];

	cout << "name >> ";
	cin >> name;
	strcat(order, name);

	system(order);
}

void mkdir()
{
	char name[30];
	char order[65];

	cout << "name >> ";
	cin >> name;

	strcpy(order, "mkdir ");
	strcat(order, name);

	system(order);
}

void rmdir()
{
	char name[30];
	char order[65];

	cout << "name >> ";
	cin >> name;

	strcpy(order, "rm -rf ");
	strcat(order, name);

	system(order);
}

void mvdir()
{
	char from[40], to[40];

	cout << "from >> ";
	cin >> from;
	cout << "to >> ";
	cin >> to;

	char order[70];

	strcpy(order, "mv ");
	strcat(order, from);
	strcat(order, " ");
	strcat(order, to);

}

void link()
{
	char command1[35];
	char command2[35];
	char command[105];

	getchar();

	cout << "file1 >> ";
	fgets(command1, 34, stdin);

	command1[strlen(command1) - 1] = '\0';

	cout << "file2 >> ";
	fgets(command2, 34, stdin);


	strcpy(command, "ln -s ");

	strcat(command, command1);
	strcat(command, " ");
	strcat(command, command2);

	system(command);
}

void select()
{
	cout << "0.exit" << endl;
	cout << "1.creat" << endl;
	cout << "2.write" << endl;
	cout << "3.read" << endl;
	cout << "4.copy" << endl;
	cout << "5.chmod" << endl;
	cout << "6.check" << endl;
	cout << "7.mkdir" << endl;
	cout << "8.rmdir" << endl;
	cout << "9.mvdir" << endl;
	cout << "10.link" << endl;

	int n;

	cout << "select >> ";
	cin >> n;

	switch (n)
	{
	case 0: break;
	case 1: creat(); break;
	case 2: write(); break;
	case 3: read(); break;
	case 4: copy(); break;
	case 5: chmod(); break;
	case 6: check(); break;
	case 7: mkdir(); break;
	case 8: rmdir(); break;
	case 9: mvdir(); break;
	case 10: link(); break;
	}

	if (n == 0) exit(0);
}


int main()
{
	while (true)
	{
		select();
	}
	return 0;
}
