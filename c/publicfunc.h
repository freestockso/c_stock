/*
公共函数头文件
*/
#ifndef  _PBULICFUNC_H_
#define _PBULICFUNC_H_


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <strstrea.h>
#include <direct.h>


#include "macro_define.h"
#include "struct_define.h"





int	MyDate_GetToday(void);			//获取今天的日期，格式是YYYYMMDD,integer type
int MyDate_GetToyear(void);			//get this year, YYYY, integer type;

float My_div(float a, float b);		//My divide function, if b = 0, return 0;

int MYFile_Copy(char* source, char* des);	//copy [source file] to [des file]

int  	MYFile_FileIsExist(char *filename);
int 	MYFile_Add(char* source, char* buf);
void 	Index_std_c(void);




#endif



