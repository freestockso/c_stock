#include "gongsi.h"
#include "publicfunc.h"
#include "baocunshuju.h"

//read all data and cal commpm data
int     uc_GongSi::Init(char *pcode)
{
    int ret;
		
	strcpy(code,pcode);	
	dayk_size = 0;
	shouru_lirun_size = 0;
	quanxi_size = 0;

	ret = readDK();
	if (ret != 0)       // if error,and this gongsi can used at second time
		return -1;
	readQuanXi();
    readFin();	
	Cal_cqj();
	Cal_pe();
	Cal_p_man();
	Cal_p_woman();
	Cal_Qiangdu();
	SetType();
	Cal_p30d();	
	Cal_p120d();
	Cal_syl30();
	Cal_syl250();
    printf("[OK] init ..%s....\n", pcode);
	return 0;
}



//get most 7 years data of dayk
int     uc_GongSi::readDK(void)
{
	char file_read[64];
	char  tmp[16];
	unsigned int day;
	float res;
	unsigned int  today;

	dayk_size = 0;
//	if (pdayk == NULL)
//		pdayk = (ut_DayK*)malloc(sizeof ut_DayK * GONGSI_MaxLineDayK);

	today = (unsigned int)MyDate_GetToday();			
	today = today - 10000 * 7;      //most 7 year data
		
	sprintf(file_read, "..\\data\\dayk\\%s_dayk.txt", code);

    if  (MYFile_FileIsExist(file_read) == NO)
    {
        printf("--[WARNING] is not exist...%s..\n",file_read);
        return (-1);
    }

	ifstream if_file(file_read);
	memset(pdayk, 0, sizeof pdayk);
	if_file >> tmp;

    while (strcmp(tmp, "[END]"))
	{
		day = atoi(tmp);
        pdayk[dayk_size].date = day;
		if_file >> pdayk[dayk_size].open;
		if_file >> pdayk[dayk_size].high;
		if_file >> pdayk[dayk_size].low;
        if_file >> pdayk[dayk_size].close;
        if_file >> pdayk[dayk_size].money;
		if_file >> res;
		if (day > today)		
		{
	        dayk_size++;
        }
        if_file.getline(tmp, sizeof tmp);
		if_file >> tmp;		
	}
	if_file.close();

	//日线数据量太少，不计入统计
	if (dayk_size < 120 + 10)		
		return -1;
	
	dayk_size_first = dayk_size;
	return 0;
}


void uc_GongSi::readFin(void)
{
	char file_source[64];
    FILE *fp_read;
    char readbuf[1024];

	shouru_lirun_size = 0;
	{
		sprintf(file_source,"..\\data\\fin\\%s_tb.txt",code);	
		ifstream if_file(file_source);	
		if ((fp_read = fopen(file_source,"rb")) != NULL )  //文件存在
		{
			fclose(fp_read);		
		
			if_file.getline(readbuf, sizeof readbuf); //first line invalid
			while(!if_file.eof())
			{
				if_file >> shouru_lirun[shouru_lirun_size].date;
				if_file >> shouru_lirun[shouru_lirun_size].shouru;
				if_file >> shouru_lirun[shouru_lirun_size].lirun;	
				if_file >> shouru_lirun[shouru_lirun_size].tb_shouru;					
				if_file >> shouru_lirun[shouru_lirun_size].tb_lirun;									
				if_file >> shouru_lirun[shouru_lirun_size].gdqyl;													
				if_file >> shouru_lirun[shouru_lirun_size].gdqy;																	
				shouru_lirun_size++;
			};
			shouru_lirun_size--;
		}
	}
	shouru_lirun_size_first = shouru_lirun_size;
	return;
}


int     uc_GongSi::readQuanXi(void)
{
	char file_read[64];
	char  tmp[16];
	unsigned int day;

	quanxi_size = 0;
	sprintf(file_read, "..\\data\\qx\\%s.txt",code);
    if  (MYFile_FileIsExist(file_read) == NO)
    {
        printf("--[WARNING] is not exist...%s...\n",file_read);
        return (-1);
    }
	
	ifstream if_file(file_read);

	if_file >> tmp;
    while (strcmp(tmp, "[END]"))
	{
		day = atoi(tmp);
		quanxi[quanxi_size].date = day;
        if_file >> quanxi[quanxi_size].songGu;
        if_file >> quanxi[quanxi_size].peiGu;
		if_file >> quanxi[quanxi_size].peiGuJia;
        if_file >> quanxi[quanxi_size].fenhong;
        
        quanxi_size++;
        if_file.getline(tmp, sizeof tmp);
		if_file >> tmp;		
    }

    if_file.close();
	return 0;
}

// ChuQuanJia, not include FenHong data
void     uc_GongSi::Cal_cqj(void)
{
    float bs = 1.0;
    float fh = 0;
    int i;

    if (type == GONGSITYPE_NULL)
    	return;

    for (i=0; i<dayk_size; i++)
    {
        bs = Get_songgu(pdayk[0].date, pdayk[i].date);
        pdayk[i].chuQuanJia = pdayk[i].close * bs;
    }
    return;
}


void     uc_GongSi::Cal_pe(void)
{
	PE = 0;
	float lr, price, sz, gb;

	if (shouru_lirun_size < 6)
		return;
	if (shouru_lirun[shouru_lirun_size-1].lirun <= 0)
		return;
	lr = shouru_lirun[shouru_lirun_size-1].lirun;
	price = pdayk[dayk_size-1].close;
	gb = Get_today_gb();
	sz = gb * price;

	PE = sz / lr;

	//printf("-------PE = %10.2f---------\n", PE);

}



float uc_GongSi::Get_songgu(unsigned int d_begin, unsigned int date)
{
    int i;
    float bs = 1;       //number of SongGu BeiShu

    for (i=0; i<quanxi_size; i++)
    {
		if (quanxi[i].date <= d_begin)
			continue;
        if (quanxi[i].date > date)
        	break;
        bs = bs * (quanxi[i].songGu + 1);
    }

	return bs;
}

//if the date nont exist, return the pre day
int uc_GongSi::Get_IndexByDate(int date)
{
	int low, high, mid;

	low = 0;
	high = dayk_size;
	while (low < high)
	{
		mid = (low + high) / 2;
        if (pdayk[mid].date == date)
		{
			return mid;
		}
		else if (pdayk[mid].date > date)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
    return low;

#if 0
	int i;

	if (dayk_size == 0)
		return 0;

	for (i=0; i<dayk_size; i++)
	{
		if (pdayk[i].date == date)
		{
			return i;
		}	
		if (pdayk[i].date > date)		
		{
			i--;
			return i;
		}
	}
	return (i-1);
#endif	
}


char* 	uc_GongSi::GetStr_p30p120(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].p_woman >= pdayk[index].p_man)
            str[i] = '+';
        else
            str[i] = '.';
    }

	return str;
}

char* 	uc_GongSi::GetStr_p30d_hy(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].p30d >= 0)
            str[i] = '+';
        else
            str[i] = '.';
    }

	return str;
}

char* 	uc_GongSi::GetStr_syl30_hy(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].syl30 >= 10)
            str[i] = '*';
        else if (pdayk[index].syl30 >= 0)
            str[i] = '+';
        else
            str[i] = '.';
    }

	return str;
}

char* 	uc_GongSi::GetStr_syl30_one(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].syl30 >= 12)
            str[i] = '-';
        else if (pdayk[index].syl30 >= 0)
            str[i] = '.';
        else
            str[i] = 'B';
    }

	return str;
}

char* 	uc_GongSi::GetStr_syl250_one(void)
{
	static char str[256];
	int  index;


    memset(str, 0, sizeof str);
	index = dayk_size - 1;
    if (pdayk[index].syl250 >= 50)
        strcpy(str, "[*]");
    else if (pdayk[index].syl250 >= 0)
        strcpy(str, "[+]");    
    else
        strcpy(str, "[.]");
	return str;
}

char* 	uc_GongSi::GetStr_syl30_czg(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].syl30 >= 15)
            str[i] = '-';
        else if (pdayk[index].syl30 >= 0)
            str[i] = '.';
        else
            str[i] = 'B';
    }

	return str;
}


char* 	uc_GongSi::GetStr_syl250_hy(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].syl250 = 0)
            str[i] = '+';
        else
            str[i] = '.';
    }

	return str;
}


char* 	uc_GongSi::GetStr_fin(void)
{
	static char str[256];
	int i, index;

	memset(str, 0 ,sizeof str);

	if (shouru_lirun_size < 8)
		return str;

	index = shouru_lirun_size - 1;
	if (shouru_lirun[index-7].gdqyl >= 0)
		sprintf(str,"[%02.0f", shouru_lirun[index-7].gdqyl);
	else
		sprintf(str,"[00");
	for (i=7; i>0; i--)
	{
		if (shouru_lirun[index-i].gdqyl <  shouru_lirun[index-i+1].gdqyl)
			sprintf(str+strlen(str),"+");			
		else
			sprintf(str+strlen(str),"-");			
	}

	if (shouru_lirun[index].gdqyl >= 0)	
		sprintf(str+strlen(str),"%02.0f,", shouru_lirun[index].gdqyl);
	else
		sprintf(str+strlen(str),"00,");

	if (PE < 99)	
		sprintf(str+strlen(str),"%2.0f]", PE);
	else
		sprintf(str+strlen(str),"**]");
		
	return str;
}

char* 	uc_GongSi::GetStr_p30d_trend_hy(void)
{
	static char str[256];
	int i, index;
	int width = 120;


    memset(str, 0, sizeof str);

    for (i=0; i<width; i++)
    {
        index = dayk_size - width + i;
        if (pdayk[index].p30d >pdayk[index-1].p30d)
            str[i] = '+';
        else
            str[i] = '.';
    }

	return str;
}

char* 	uc_GongSi::GetStr_tag_zs(void)
{
	static char str[256];
	int index;
	
	index = dayk_size - 1;
    memset(str, 0, sizeof str);

	sprintf(str+strlen(str), "[ ");
	if (pdayk[index].p120d > 0)
		sprintf(str+strlen(str), " +");			
	else
		sprintf(str+strlen(str), " .");			
	if (pdayk[index].p120d > pdayk[index-1].p120d)
		sprintf(str+strlen(str), "+  ");			
	else
		sprintf(str+strlen(str), ".  ");			


	if (pdayk[index].syl250 > 30)
		sprintf(str+strlen(str), "* ");			
	else if (pdayk[index].syl250 > 0)
		sprintf(str+strlen(str), "+ ");			
	else
		sprintf(str+strlen(str), ". ");		
		

	sprintf(str+strlen(str), "]    [");

	if (pdayk[index].p30d > 0)
		sprintf(str+strlen(str), " +");			
	else
		sprintf(str+strlen(str), " .");			
	if (pdayk[index].p30d > pdayk[index-1].p30d)
		sprintf(str+strlen(str), "+  ");			
	else
		sprintf(str+strlen(str), ".  ");			


	if (pdayk[index].syl30 > 15)
		sprintf(str+strlen(str), "* ");			
	else if (pdayk[index].syl30 > 0)
		sprintf(str+strlen(str), "+ ");			
	else
		sprintf(str+strlen(str), ". ");			

	sprintf(str+strlen(str), "]");	

	return str;
}

char* 	uc_GongSi::GetStr_tag_hy(void)
{
	static char str[256];
	int index;
	
	index = dayk_size - 1;
    memset(str, 0, sizeof str);

	sprintf(str+strlen(str), "[ ");
	if (pdayk[index].p120d > 0)
		sprintf(str+strlen(str), " +");			
	else
		sprintf(str+strlen(str), " .");			
	if (pdayk[index].p120d > pdayk[index-1].p120d)
		sprintf(str+strlen(str), "+  ");			
	else
		sprintf(str+strlen(str), ".  ");			


	if (pdayk[index].syl250 > 40)
		sprintf(str+strlen(str), "* ");			
	else if (pdayk[index].syl250 > 0)
		sprintf(str+strlen(str), "+ ");			
	else
		sprintf(str+strlen(str), ". ");		
		

	sprintf(str+strlen(str), "]    [");

	if (pdayk[index].p30d > 0)
		sprintf(str+strlen(str), " +");			
	else
		sprintf(str+strlen(str), " .");			
	if (pdayk[index].p30d > pdayk[index-1].p30d)
		sprintf(str+strlen(str), "+  ");			
	else
		sprintf(str+strlen(str), ".  ");			


	if (pdayk[index].syl30 > 20)
		sprintf(str+strlen(str), "* ");			
	else if (pdayk[index].syl30 > 0)
		sprintf(str+strlen(str), "+ ");			
	else
		sprintf(str+strlen(str), ". ");			

	sprintf(str+strlen(str), "]");	

	return str;
}


//120 average price of ChuQuanJia
void    uc_GongSi::Cal_p_man()    
{
	int width;
	int i,j, index;
	float total;

	width = 120;
    if (type == GONGSITYPE_NULL)
    	return;
	if (dayk_size <= width )
		return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].p_man = 0;
    }

	for (i=dayk_size-1; i>width; i--)   
	{
		index = i;
		total = 0;
		for (j=0; j<width; j++)
		{
			total += pdayk[index-j].chuQuanJia;
		}
		total = total / width;
		pdayk[index].p_man = total;
	}
	
	return;
}

//30 average price of ChuQuanJia
void    uc_GongSi::Cal_p_woman()    
{
	int width;
	int i,j, index;
	float total;

	width = 30;
    if (type == GONGSITYPE_NULL)
    	return;
	if (dayk_size <= width )
		return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].p_woman = 0;
    }

	for (i=dayk_size-1; i>width; i--)
	{
		index = i;
		total = 0;
		for (j=0; j<width; j++)
		{
			total += pdayk[index-j].chuQuanJia;
		}
		total = total / width;
		pdayk[index].p_woman = total;
	}
	
	return;
}


//120 QD, just consider duo
void    uc_GongSi::Cal_Qiangdu(void)    
{
    int i, j, phkd;
    int index_now, index_ago;
    float total;

	if (type == GONGSITYPE_NULL)
		return ;
    if (dayk_size < GONGSI_MinLineDatK)
        return;

	//计算公司强度
    for (i=0; i<dayk_size; i++) 
    {
        pdayk[i].QD_120_1 = 0;
    }
	
    for (i=0; i<dayk_size - GONGSI_MinLineDatK; i++)    
    {
        index_now = dayk_size - 1 - i;
        index_ago = index_now - GONGSI_MinLineDatK;
        pdayk[index_now].QD_120_1 = My_div(pdayk[index_now].p_woman,pdayk[index_ago].p_woman) - 1;
	}
		 
    //PingHua 
    phkd = 5;
    for (i=1; i<dayk_size - phkd; i++)
    {
        total = 0;
        index_now = dayk_size - i;
        for (j=0; j<phkd; j++)
        {
            total += pdayk[index_now-j].QD_120_1;
        }
        pdayk[index_now].QD_120_1 = total / phkd;
    }	
	return;
}



void    uc_GongSi::SetType(void)
{
    if (*code == '8')   
        type = GONGSITYPE_HANGYE;
    else if (*code == '3' && *(code+1) == '9')
        type = GONGSITYPE_ZHISHU;
    else
        type = GONGSITYPE_GONGSI;
    return;        
}


void	uc_GongSi::Output_view(void)
{
    int i;
    char file_write[64];
    char buf[256];

	printf("...Output_view...%s....\n", code);

	sprintf(file_write, "..\\data\\view\\%s.txt",code);
	ofstream of_file(file_write);
	
	sprintf(buf,"%12s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n","date", "close", "cqj", "qd", "P30", "P120", "p30d", "p120d", "syl30", "syl250");	
	of_file << buf;   
	
	for (i=0; i<dayk_size;i++)
	{
		sprintf(buf,"%12d%12.4f%12.4f%12.4f%12.2f%12.2f%12.0f%12.0f%12.2f%12.2f\n",
			pdayk[i].date, 
			pdayk[i].close,
			pdayk[i].chuQuanJia,
			pdayk[i].QD_120_1,
			pdayk[i].p_woman,
			pdayk[i].p_man,
			pdayk[i].p30d,
			pdayk[i].p120d,			
			pdayk[i].syl30,
			pdayk[i].syl250
			);
		of_file << buf;   
	}
	of_file.close();
}

void	uc_GongSi::Cal_p30d(void)
{
    int i, days, bflag;

    if (type != GONGSITYPE_GONGSI)
        return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].p30d = 0;
    }

    bflag = YES;
    days = 0;
    for (i=0; i<dayk_size; i++)
    {
        if (pdayk[i].p_woman == 0)
        {
            pdayk[i].p30d = (float)0;
            continue;
        }
        if (pdayk[i].chuQuanJia >= pdayk[i].p_woman)
        {
            if (bflag == NO)
            {
                bflag = YES;
                days = 0;
            }    
            days++;
            pdayk[i].p30d = (float)days * days;
            continue;
        }
        if (pdayk[i].chuQuanJia < pdayk[i].p_woman)
        {
            if (bflag == YES)
            {
                bflag = NO;
                days = 0;
            }
            days++;
            pdayk[i].p30d = (float)days * days * (-1);
            continue;
        }        
    }
    return;
}

void	uc_GongSi::Cal_p120d(void)
{
    int i, days, bflag;

    if (type != GONGSITYPE_GONGSI)
        return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].p120d = 0;
    }

    bflag = YES;
    days = 0;
    for (i=0; i<dayk_size; i++)
    {
        if (pdayk[i].p_man == 0)
        {
            pdayk[i].p120d = (float)0;
            continue;
        }
        if (pdayk[i].chuQuanJia >= pdayk[i].p_man)
        {
            if (bflag == NO)
            {
                bflag = YES;
                days = 0;
            }    
            days++;
            pdayk[i].p120d = (float)days * days;
            continue;
        }
        if (pdayk[i].chuQuanJia < pdayk[i].p_man)
        {
            if (bflag == YES)
            {
                bflag = NO;
                days = 0;
            }
            days++;
            pdayk[i].p120d = (float)days * days * (-1);
            continue;
        }        
    }
    return;
}


void	uc_GongSi::Cal_syl30(void)
{
    int i,j, i_now;
    float yl, cj, syl, total;

    if (type != GONGSITYPE_GONGSI)
        return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].yl30= 0;
        pdayk[i].cj30= 0;        
        pdayk[i].syl30 = 0;                
    }

    for (i=30; i<dayk_size; i++)
    {
        i_now = i;
        yl = 0;
        cj = 0;
        for (j= i_now-30; j<=i_now; j++)
        {
            syl = pdayk[i_now].chuQuanJia /  pdayk[j].chuQuanJia - 1;
            yl = yl + pdayk[j].money * syl;
            cj = cj + pdayk[j].money;
        }
        pdayk[i_now].yl30 = yl;
        pdayk[i_now].cj30 = cj;
        pdayk[i_now].syl30 = My_div(yl,cj) * 100;
    }

    //pinghua
    for (i=dayk_size-1; i>5; i--)
    {
        total = 0;
        for (j=0; j<5; j++)
            total = total + pdayk[i-j].syl30;
        pdayk[i].syl30 = total / 5;
    }

    return;
}


void	uc_GongSi::Cal_syl250(void)
{
    int i,j, i_now;
    float yl, cj, syl, total;

    if (type != GONGSITYPE_GONGSI)
        return;

    for (i=0; i<dayk_size; i++)
    {
        pdayk[i].yl250= 0;
        pdayk[i].cj250= 0;  
        pdayk[i].syl250 = 0;  
    }

    if (dayk_size < 255)
        return;

    for (i=250; i<dayk_size; i++)
    {
        i_now = i;
        yl = 0;
        cj = 0;
        for (j= i_now-250; j<=i_now; j++)
        {
            syl = pdayk[i_now].chuQuanJia /  pdayk[j].chuQuanJia - 1;
            yl = yl + pdayk[j].money * syl;
            cj = cj + pdayk[j].money;
        }
        pdayk[i_now].yl250 = yl;
        pdayk[i_now].cj250 = cj;
        pdayk[i_now].syl250 = My_div(yl,cj) * 100;
    }

    //pinghua
    for (i=dayk_size-1; i>30; i--)
    {
        total = 0;
        for (j=0; j<30; j++)
            total = total + pdayk[i-j].syl250;
        pdayk[i].syl250 = total / 30;
    }

    return;
}

int uc_GongSi::Is_InGNHY(char *hyname)
{

	char file_source[64];
	char readbuf[1024];	
	char key[16];

	if (name[0] >= '0' && name[0] <= '9')
		return NO;

	if (!strcmp(hyname, "次新股") && atoi(code) > 599999)
		return NO;

	strcpy(key, hyname);

	sprintf(file_source,"..\\data\\hangye_gongsi.txt");
	ifstream if_file(file_source);		


	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return NO;
	
		if (!strcmp(readbuf,key) )
		{	
			while(1)
			{
				if_file.getline(readbuf, sizeof readbuf);
				if_file >> readbuf;    //code
				if (!strcmp(readbuf,"##") )
				{
					return NO;
				}

				if (!strcmp(readbuf,code ))
				{
					return YES;
				}
			}
		}	
	}

	return NO;
}


// first read to memory, MUST NOT read file every time
int uc_GongSi::Is_InHY(char *hycode, char* hyname)
{
    static  ut_CodeHY  codeHY[8192];
    static  int size_codeHY = 0;
    int i;

    if (!strcmp(hycode, "399101"))
        return Is_399101();
    if (!strcmp(hycode, "399102"))
        return Is_399102();
    if (!strcmp(hycode, "399300"))
        return Is_399300();

	//if hycode > 8805, then see it is a gainian hangye
	if (atoi(hycode) > 880500)
	{
		return Is_InGNHY(hyname);
	}

	
    //check if the codeHY is init
    if (size_codeHY == 0)
    {
    	char file_source[64];
    	char readbuf[1024];

    	sprintf(file_source,"..\\data\\index_c.txt");
    	ifstream if_file(file_source);		

        if_file.getline(readbuf, sizeof readbuf); // remove the first line

    	while(1)
    	{
            if_file >> codeHY[size_codeHY].gscode;
    		if (!strcmp(codeHY[size_codeHY].gscode,"[END]") )
                break;
            if_file >> readbuf;
            if_file >> codeHY[size_codeHY].hycode;            
            size_codeHY++;
    		if_file.getline(readbuf, sizeof readbuf);
       	}
    }


    //find code is inHY?
    for (i=0; i<size_codeHY; i++)
    {
        if (strcmp(codeHY[i].gscode, code))
            continue;
        if (!strcmp(codeHY[i].hycode, hycode))  
            return YES;
    }
    return NO;
}


char *uc_GongSi::GetHYCode(void)
{
    static  ut_CodeHY  codeHY[8192];
    static  int size_codeHY = 0;
    int i;

    //check if the codeHY is init
    if (size_codeHY == 0)
    {
    	char file_source[64];
    	char readbuf[1024];

    	sprintf(file_source,"..\\data\\index_c.txt");
    	ifstream if_file(file_source);		

        if_file.getline(readbuf, sizeof readbuf); // remove the first line

    	while(1)
    	{
            if_file >> codeHY[size_codeHY].gscode;
    		if (!strcmp(codeHY[size_codeHY].gscode,"[END]") )
                break;
            if_file >> readbuf;
            if_file >> codeHY[size_codeHY].hycode;            
            size_codeHY++;
    		if_file.getline(readbuf, sizeof readbuf);
       	}
    }


    //ret hycode
    for (i=0; i<size_codeHY; i++)
    {
        if (!strcmp(codeHY[i].gscode, code))
            return codeHY[i].hycode;
    }
    return NULL;
}


int uc_GongSi::Is_399102(void)
{
	if (code[0] == '3'
		&& code[1] == '0'
		&& code[2] == '0'
	)
	{
		return YES;
	}
	return NO;
}

int uc_GongSi::Is_399101(void)
{
	if (code[0] == '0'
		&& code[1] == '0'
		&& code[2] == '2'
	)	
	{
		return YES;
	}
	return NO;
}

int uc_GongSi::Is_399300(void)
{
	uc_BaseData database;
	
    return database.Is_hs300(code);
}

int uc_GongSi::Is_czg(void)
{

	int index;

	if (type != GONGSITYPE_GONGSI)
	    return NO;

	index = shouru_lirun_size - 1;
	if (index < 6)
		return NO;

	if (shouru_lirun[index].gdqyl < 8)
		return NO;

	if 	(shouru_lirun[index].tb_lirun < 7)
		return NO;
	if 	(shouru_lirun[index-1].tb_lirun < 7)
		return NO;
	if 	(shouru_lirun[index].tb_shouru< 6)
		return NO;
	if 	(shouru_lirun[index-1].tb_shouru < 6)
		return NO;


	return YES;	
}

int uc_GongSi::Is_czg_low(void)
{

	int index;

	if (type != GONGSITYPE_GONGSI)
	    return NO;

	index = shouru_lirun_size - 1;
	if (index < 6)
		return NO;

	if (shouru_lirun[index].gdqyl < 0.08)
		return NO;

	if 	(shouru_lirun[index].tb_lirun < 0.2)
		return NO;
	if 	(shouru_lirun[index-1].tb_lirun < 0.2)
		return NO;
	if 	(shouru_lirun[index].tb_shouru< 0.2)
		return NO;
	if 	(shouru_lirun[index-1].tb_shouru < 0.2)
		return NO;


	return YES;	
}


int uc_GongSi::Get_fin_index(int date)
{
	int i;
	
	for (i=5; i<shouru_lirun_size; i++)
	{
		if (shouru_lirun[i].date == date)
			return i;
	}

	return INVALID_INDEX;
}


float uc_GongSi::Get_today_gb(void)
{
	char file_source[64];
    FILE *fp_read;
	char f_code[128];
	float gb = 0;

	sprintf(file_source,"..\\data\\gb.txt");	
	if ((fp_read = fopen(file_source,"rb")) != NULL )  //文件存在
	{
		fclose(fp_read);		

		ifstream if_file(file_source);		
		if_file >> f_code;

		while(!if_file.eof())
		{
			if (!strcmp(f_code, code))
			{
				if_file >> gb;
				return gb;
			}
			if_file.getline(f_code, sizeof f_code);
			if_file >> f_code;
		};
	}
	
	return gb;
}
