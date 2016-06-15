

#include "publicfunc.h"


int	MyDate_GetToday(void)
{
	char today[30];
	struct tm *newtime;
	long ltime;

	time( &ltime );

	/* Obtain coordinated universal time: */
	newtime = localtime( &ltime );
	sprintf(today, "%d%02d%02d", 
		1900 + newtime->tm_year, 
		1 + newtime->tm_mon, 
		newtime->tm_mday);

	return atoi(today);   	
}


int	MyDate_GetToyear(void)
{
	struct tm *newtime;
	long ltime;

	time( &ltime );

	/* Obtain coordinated universal time: */
	newtime = gmtime( &ltime );

	return (1900 + newtime->tm_year);   	
}

float My_div(float a, float b)
{
	if (b==0)
		return 0;
	return (a/b);	
}



int 
MYFile_Copy(char* source, char* des)
{
    ifstream if_file(source);        
	ofstream of_file(des);

    of_file << if_file.rdbuf();
    
    of_file.close();
    if_file.close();

	return 1;
}


int 
MYFile_Add(char* source, char* buf)
{
	char back[128];

	sprintf(back, "%s.back", source);
	MYFile_Copy(source, back);

    ifstream if_file(back);        
	ofstream of_file(source);

    of_file << if_file.rdbuf();
	of_file << buf;					//ÊäÈëÐÂµÄ
    
    of_file.close();
    if_file.close();

	return YES;
}


int  MYFile_FileIsExist(char *filename)
{
	FILE *fp_read;
	
	if ((fp_read = fopen(filename,"rb")) == NULL) 
	{
		return NO;
	}
	fclose(fp_read);
	return YES;
}

void Index_std_c(void)
{
	char file_source[64], file_write[64]; 
	char readbuf[1024];
	unsigned int i;

	sprintf(file_source,"..\\data\\index.txt");
	sprintf(file_write,"..\\data\\index_c.txt");

	ifstream if_file(file_source);	
	ofstream of_file(file_write);	

	
	while(1)
	{	
		if (if_file.eof())
			break;
		memset(readbuf, 0, sizeof readbuf);
		if_file.getline(readbuf, sizeof readbuf); 
		if (strlen(readbuf) == 0)
		    break;
		for (i=0; i<strlen(readbuf);i++)
		{
			if ((int)readbuf[i] == ',')
			{
				readbuf[i] = ' ';
			}			
		}
		readbuf[strlen(readbuf)] = '\n';	
		of_file	<< readbuf;
	}

    of_file << "[END]";
	of_file.close();
	return;
}




