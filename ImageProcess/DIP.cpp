// zhongzhilvbo.cpp : 定义控制台应用程序的入口点。
//
#include"pch.h"
#include <stdio.h>   
#include <math.h>   
#include <stdlib.h>   
#include "tchar.h"
#include "string.h"

#define DATA_X 256      //数字图像水平像素个数
#define DATA_Y 256      //数字图像竖直像素个数

void OpenFile(const char *cFilePath, int nOriginalData[DATA_Y][DATA_X])
{
 printf("正在获取数据......\n");
 FILE *fp;
 fp = fopen(cFilePath, "r");
 if (NULL == fp)
  {
	    printf("open file failed! \n");
	     return;
	   }

	   unsigned char *pData = (unsigned char *)malloc(sizeof(unsigned char)*DATA_X*DATA_Y);
   if (NULL == pData)
	  {
	       printf("memory malloc failed!\n");
	       return;
	   }

   fread(pData, sizeof(unsigned char)*DATA_X*DATA_Y, 1, fp);

	   int count_x = 0;
   int count_y = 0;

   for (; count_y < DATA_Y; count_y++)
	    {
	   for (; count_x < DATA_X; count_x++)
		     {
		          nOriginalData[count_y][count_x] = pData[count_y*DATA_Y + count_x];
		      }
	    }

	   free(pData);
	  fclose(fp);

	   return;
}

void SaveFile(const char *cFilePath, int nResultData[DATA_Y][DATA_X])
{
    printf("正在保存数据......\n");
    int count_x, count_y;

	    FILE *fp;
  fp = fopen(cFilePath, "w");
	 if (NULL == fp)
	  {
	       printf("open file failed! \n");
	      return;
	   }

	   for (count_y = 0; count_y < DATA_Y; count_y++)
	    {
	        for (count_x = 0; count_x < DATA_X; count_x++)
		   {
		           fwrite(&nResultData[count_y][count_x], 1, 1, fp);
		      }
	   }

	  fclose(fp);
   printf("文件保存成功! \n");

   return;
}

 bool JunZhiLvBo(const int nOriginalData[DATA_Y][DATA_X], int nResultData[DATA_Y][DATA_X])
{
     printf("正在进行均值滤波......\n");
     int count_x, count_y;
	
	     /*3*3模版滤波计算，不计算边缘像素*/
	    for (count_y = 1; count_y < DATA_Y; count_y++)
	    {
	      for (count_x = 1; count_x < DATA_X; count_x++)
	       {
		          nResultData[count_y][count_x] = (int)((nOriginalData[count_y - 1][count_x - 1] +
			                                              nOriginalData[count_y - 1][count_x] +
			                                               nOriginalData[count_y - 1][count_x + 1] +
			                                                  nOriginalData[count_y][count_x - 1] +
			                                                   nOriginalData[count_y][count_x] +
			                                               nOriginalData[count_y][count_x + 1] +
			                                                    nOriginalData[count_y + 1][count_x - 1] +
			                                               nOriginalData[count_y + 1][count_x] +
			                                                    nOriginalData[count_y + 1][count_x + 1]) / 9);
		      }
	   }

	   /*对四个边缘直接进行赋值处理*/
	   for (count_x = 0; count_x < DATA_X; count_x++)                                        //水平边缘像素等于原来像素灰度值
	   {
	      nResultData[0][count_x] = nOriginalData[0][count_x];
	      nResultData[DATA_Y - 1][count_x] = nOriginalData[DATA_Y - 1][count_x];
	     }
   for (count_y = 1; count_y < DATA_Y - 1; count_y++)                                     //竖直边缘像素等于原来像素灰度值
		     {
	         nResultData[count_y][0] = nOriginalData[count_y][0];
		        nResultData[count_y][DATA_X - 1] = nOriginalData[count_y][DATA_X - 1];
		    }
	
		    return true;
 }

 int _tmain(int argc, _TCHAR* argv[])
{
  int nOriginalData[DATA_Y][DATA_X]; //保存原始图像灰度值
	   int nResultData[DATA_Y][DATA_X];   //保存滤波后的灰度值

	   memset(nOriginalData, 0, sizeof(nOriginalData));  //初始化数组
    memset(nResultData, 0, sizeof(nResultData));

	   char cOpenFilePath[] = "2.bmp";                                           //图像文件路径
	
	   OpenFile(cOpenFilePath, nOriginalData);
	
	   if (!JunZhiLvBo(nOriginalData, nResultData))                                   //滤波计算
	  {
		      printf("操作失败!\n");
		       return 0;
	    }
	
		     char cSaveFilePath[] = "Res2.raw";                                        //文件保存路径
	   SaveFile(cSaveFilePath, nResultData);

	     return 0;
}