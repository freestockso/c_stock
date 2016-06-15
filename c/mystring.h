#ifndef _MYSTRING_H_
#define  _MYSTRING_H_


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



class uc_MyString 
{
public:
	char c_string[1024];
	int	index;

	int InitString(char *);
	float GetNextFloat_0(void);		// -- Ëã×÷0´¦Àí
	float GetNextFloat(void);
	int	GetNextInt(void);
	void GetNextString(char*);
	int  CalBlankNum(void);
	
};


#endif

