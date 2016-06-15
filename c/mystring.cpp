#include "mystring.h"

int uc_MyString::InitString(char *str)
{
	if (strlen(str)> 1024)
	{
		return 1;
	}		
	strcpy(c_string, str);
	index = 0;
	return 0;		
}


int uc_MyString::GetNextInt(void)
{
	char buf[32];
	int i, flag = 0, buf_index = 0;

	memset(buf, 0 ,sizeof buf);

	for (i=index; i<1024; i++)
	{
		if (c_string[i] == '\0')		// 结束
			return (-999999);

		index++;
		
		if ((c_string[i] >= '0' && c_string[i] <= '9'))
		{
			if (flag == 0)
			{
				flag = 1;
			}
			buf[buf_index] = c_string[i];
			buf_index++;
			continue;
		}

		if (flag == 1)
		{
			return atoi(buf);
		}
	}
	return 0;

}

//获取下一个浮点数
float uc_MyString::GetNextFloat(void)
{
	char buf[32];
	int i, flag = 0, buf_index = 0;

	memset(buf, 0 ,sizeof buf);

	for (i=index; i<1024; i++)
	{
		if (c_string[i] == '\0')		// 结束
			return (float)(-999999);

		index++;
		
		if (c_string[i] == '.'
			||  (c_string[i] >= '0' && c_string[i] <= '9'))
		{
			if (flag == 0)
			{
				flag = 1;
			}
			buf[buf_index] = c_string[i];
			buf_index++;
			continue;
		}

		if (flag == 1)
		{
			return (float)atof(buf);
		}
	}
	return 0;
}


//包含--的下一个浮点数
float uc_MyString::GetNextFloat_0(void)
{
	char buf[32];
	int i, flag = 0, buf_index = 0;

	memset(buf, 0 ,sizeof buf);

	for (i=index; i<1024; i++)
	{
		if (c_string[i] == '\0')		// 结束
			return (float)(-999999);

		index++;
		
		if (c_string[i] == '-' 
			|| 	c_string[i] == '.'
			||  (c_string[i] >= '0' && c_string[i] <= '9'))
		{
			if (flag == 0)
			{
				flag = 1;
			}
			buf[buf_index] = c_string[i];
			buf_index++;
			continue;
		}

		if (flag == 1)
		{
			if (*buf == '-' && *(buf+1) == '-')
				return 0;
			else	
				return (float)atof(buf);
		}
	}
	return 0;
}

void uc_MyString::GetNextString(char* str)
{
	strcpy(str,c_string+index);
	return;
}

int uc_MyString::CalBlankNum(void)
{
	int num = 0, i;
	
	for (i=index; i<1024; i++)
	{
		if (c_string[i] != ' ')
		{
			break;
		}
		num++;
	}

	return num;
}



