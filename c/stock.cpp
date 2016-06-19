/*  修改记录
	2011-4-15:开始加入针对f10数据的分析
	2011-4-16:完成对收入利润数据的提取分析，公司层面
	2011-4-17 完成对收入利润行业层面的提取分析

	2011-6 增加201处理，处理定投方式的处理

	2011-12
	  1) 增加对公司相对季度的ps和pe的输出；       
	  2) 删除stock 2中对公司的历史强度的跟踪和输出     20111211

	2012-6-17:增加105操作，对gb的单独提取

//行业指数	20120816 sina软件调整指数
887系列删除
采用880301 880401细类


*/




#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <strstrea.h>
#include <direct.h>
#include <stdio.h>

#include "baocunshuju.h"
#include "gongsi.h"
#include "market.h"

int pf_GengXinAllDayK(void); //更新所有K线交易数据

uc_Market   	Market;
unsigned int   	G_mode;


//帮助文件函数
void man(void)
{   
    printf("%5d   Get DK data ... \n", A_gDK);     
    printf("%5d   Get QX and Codename info... \n", A_gQX);         
    printf("%5d   Get gongsi/HY/hy_for_aliyun info... \n", A_gHY);     
    printf("%5d   Cal fin data for all gongsi...\n", A_cFin); 
    printf("%5d   Cal fin list for find download...\n", A_rFINList);     
    printf("%5d   Cal HY fin...\n", A_cHYF); 
    
    return;
}

void SaveBinDat(void)
{
	FILE *fp;
   
	fp = fopen("..\\data\\market_bin.dat", "wb");
	fwrite(&Market, sizeof Market, 1, fp);
	fclose(fp);

	return;
}


void ReadBinDat(void)
{
	FILE *fp;
   
	fp = fopen("..\\data\\market_bin.dat", "rb");
	fread(&Market, sizeof Market, 1, fp);
	fclose(fp);

    printf("...ReadBinDat..ok....\n");
    
	return;
}

//compute fin, not include compute from source fin data
int CFin(void)
{
    uc_BaseData  ucv_baocunshuju;

	char file_source[64] = "..\\data\\tdx_gongsi.txt" ;
	char readbuf[1024];	
	ifstream if_file(file_source);		    


	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			break;
	
        strcpy(ucv_baocunshuju.gongsi_name, readbuf);
        ucv_baocunshuju.pf_BaoCun_F10_data();

		if_file.getline(readbuf, sizeof readbuf);
	}	

    return 0;
}

int main(int argc, char* argv[])
{

	int action = A_NULL;
	int ret;

	if (argc != 1)
		action = atol(argv[1]);
	else
		action = A_NULL;

	G_mode = action;

    if (action == A_gDK)
    {
        ret = pf_GengXinAllDayK();  
        return 0;
    }

    if (action ==A_cFin)
    {
		CFin();
        return 0;
    }
    

    if (action == A_gHY)
    {
    	uc_BaseData  ucv_baocunshuju;			

  		//ucv_baocunshuju.Tiqu_tdx_index_Info();
		ucv_baocunshuju.Tiqu_hy_for_aliyun();
		ucv_baocunshuju.Tiqu_hs300_Info();
		ucv_baocunshuju.Tiqu_HYinfo();
        return 0;
    }

    if (action == A_gHY4ali)
	{
    	uc_BaseData  ucv_baocunshuju;			

  		ucv_baocunshuju.Tiqu_hy_for_aliyun();
  		
        return 0;
    }
    
    if (action == A_gQX)
    {
        uc_BaseData  ucv_baocunshuju;
        ucv_baocunshuju.pf_BaoCunQuanXi();
        ucv_baocunshuju.pf_Baocun_code_and_name();
        return 0;
    }

    if (action == A_rFINList)
    {
		//Market.Action_FinList();
        return 0;
    }

    if (action == A_cHYF)
    {
        Index_std_c();
        ReadBinDat();
        Market.cHYF();
        return 0;
    }

    if (action == A_m0)
    {
        Market.Init();
        SaveBinDat();
        return 0;
    }
    if (action == A_m1)
    {
        Index_std_c();
        ReadBinDat();
    	Market.Cal_Paiming(); 
    	Market.Cal_m1();
        Market.out_m1();
        return 0;
    }
    
	man();
	
    return 0;
}


//更新所有的公司/887指数系列/399902/399101/399006三大指数系列
int pf_GengXinAllDayK(void)
{
    uc_BaseData  ucv_baocunshuju;
	char file_source[64] = "..\\data\\tdx_gongsi.txt" ;
	char readbuf[1024];	
	ifstream if_file(file_source);		
	
    ucv_baocunshuju.data_type = _SINA_;
    
	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			break;
	
        strcpy(ucv_baocunshuju.gongsi_name, readbuf);
        ucv_baocunshuju.pf_BaoCunDayK();

		if_file.getline(readbuf, sizeof readbuf);
	}	


 
    //指数
    strcpy(ucv_baocunshuju.gongsi_name, "399101");
    ucv_baocunshuju.pf_BaoCunDayK();


    strcpy(ucv_baocunshuju.gongsi_name, "399300");
    ucv_baocunshuju.pf_BaoCunDayK();


    strcpy(ucv_baocunshuju.gongsi_name, "399102");
    ucv_baocunshuju.pf_BaoCunDayK();

    return 1;
}









