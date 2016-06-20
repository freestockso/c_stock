
#include "macro_define.h"
#include "baocunshuju.h"
#include "publicfunc.h"
#include "mystring.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


//887003特别对待，属于我的行业
ut_DZH_CodeName mycodename[128] = 
{
"887001","银行类"," ",
"887002","钢铁行业"," ",
"887003","家具行业"," ",
"887004","石油行业"," ",
"887005","公路桥梁"," ",
"887006","汽车类"," ",
"887007","交通运输"," ",
"887008","医疗器械"," ",
"887009","酒店旅游"," ",
"887010","房地产"," ",

"887011","商业百货"," ",
"887012","物资外贸"," ",
"887013","食品行业"," ",
"887014","纺织行业"," ",
"887015","电力行业"," ",
"887016","农林牧渔"," ",
"887017","传媒娱乐"," ",
"887018","化工行业"," ",
"887019","煤炭行业"," ",
"887020","建筑建材"," ",

"887021","水泥行业"," ",
"887022","家电行业"," ",
"887023","电子信息"," ",
"887024","综合行业"," ",
"887025","机械行业"," ",
"887026","化纤行业"," ",
"887027","农药化肥"," ",
"887028","电器行业"," ",
"887029","NULL"," ",
"887030","开发区"," ",

"887031","NULL"," ",
"887032","船舶制造"," ",
"887033","生物制药"," ",
"887034","电子器件"," ",
"887035","有色金属"," ",
"887036","酿酒行业"," ",
"887037","造纸行业"," ",
"887038","环保行业"," ",
"887039","陶瓷行业"," ",
"887040","服装鞋类"," ",

"887041","供水供气"," ",
"887042","发电设备"," ",
"887043","纺织机械"," ",
"887044","印刷包装"," ",
"887045","塑料制品"," ",
"887046","玻璃行业"," ",
"887047","NULL"," ",
"887048","仪器仪表"," ",
"887049","券商类"," ",
"887050","保险类"," ",


"900000","银行类"," "   		//行业结束
};


int uc_BaseData::pf_BaoCunDayK(void)
{
    int ret;
    
    ret = vf_BaoCunShuJu_SINA();
	return ret;
}


int uc_BaseData::vf_BaoCunShuJu_SINA(void)
{
    int ret = 0;
    char file_name_sina[64];

    ret = vf_GetDealSourceFileName_SINA(file_name_sina);
    if (ret != 0)
        return ret;

    ret = vf_SaveBiaoZhunDayK_SINA(file_name_sina);
	return ret;
    
}




int uc_BaseData::vf_BaoCunShuJu_DZH(void)
{
	printf("\nBaoCunShuJu-vf_BaoCunShuJu_DZH\n");
	return 1;
}


//获取交易数据 20120816
int  uc_BaseData::vf_GetDealSourceFileName_SINA(char *fliename)
{
    int ret = 0;
    FILE *fp_read;

    if (*gongsi_name== '0' || *gongsi_name== '3')
	{
	    sprintf(fliename, "c:\\zd_zszq\\Vipdoc\\sz\\lday\\sz%s.day",gongsi_name);
	}
	else if (*gongsi_name == '6' || *gongsi_name == '8' || *gongsi_name == '9')
	{
	    sprintf(fliename, "c:\\zd_zszq\\Vipdoc\\sh\\lday\\sh%s.day",gongsi_name);
	}
	else
	{
		printf("[error][%s]输入的股票代码不对\n",gongsi_name);
		return -1;
	}		

	if ((fp_read = fopen(fliename,"rb")) == NULL) 
	{
		printf("[error][%s]输入的股票代码不对\n",gongsi_name);
		return -1;
	}
    fclose(fp_read);

    return 0;
}


void uc_BaseData::pf_BaoCun_F10_data(void)
{
	size_shouru_lirun = 0;
	memset(&shouru_lirun, 0, sizeof shouru_lirun);

	vf_BaoCun_F10_Shouru_Lirun();
	Compute_Shouru_lirun();

	printf("[pf_BaoCun_F10_data].....[%s]\n", gongsi_name);
	
	return;
}

//计算季度增长
void uc_BaseData::Compute_Shouru_lirun(void)
{
	char file_source[64];
    FILE *fp_read;
    char readbuf[1024],writebuf[128];
    int i, month_day;

	/*
		读取数据
	*/
	{
	    size_shouru_lirun = 0; 									//renew
		memset(&shouru_lirun, 0, sizeof shouru_lirun);	    
		sprintf(file_source,"..\\data\\fin\\%s_fin.txt",gongsi_name);	
		ifstream if_file(file_source);	
		if ((fp_read = fopen(file_source,"rb")) != NULL )  //文件存在
		{
			fclose(fp_read);		
		
			if_file.getline(readbuf, sizeof readbuf); //first line invalid
			while(!if_file.eof())
			{
				if_file >> shouru_lirun[size_shouru_lirun].date;
				if_file >> shouru_lirun[size_shouru_lirun].shouru;
				if_file >> shouru_lirun[size_shouru_lirun].lirun;	
				if_file >> shouru_lirun[size_shouru_lirun].gdqy;					
				size_shouru_lirun++;

				if_file.getline(readbuf, sizeof readbuf);
								
			};
			size_shouru_lirun--;
		}
	}
	
	/*
		compute season 同比增长
	*/
	for (i=size_shouru_lirun-1; i>0; i--)
	{
		month_day = shouru_lirun[i].date % 10000;
		if (month_day != 331) 		//各季度的收入
		{
			shouru_lirun[i].shouru= shouru_lirun[i].shouru - shouru_lirun[i-1].shouru;		
			shouru_lirun[i].lirun = shouru_lirun[i].lirun - shouru_lirun[i-1].lirun;
		}
	}

	//compute for 4 season
	for (i=size_shouru_lirun-1; i>=4; i--)
	{
		shouru_lirun[i].shouru = shouru_lirun[i].shouru + shouru_lirun[i-1].shouru + shouru_lirun[i-2].shouru + shouru_lirun[i-3].shouru;
		shouru_lirun[i].lirun = shouru_lirun[i].lirun + shouru_lirun[i-1].lirun + shouru_lirun[i-2].lirun + shouru_lirun[i-3].lirun;
		shouru_lirun[i].gdqyl = My_div(shouru_lirun[i].lirun, shouru_lirun[i].shouru) * 100;
	}

	//compute for HB
	for (i=size_shouru_lirun-1; i>=5; i--)	
	{
		shouru_lirun[i].tb_shouru = (My_div(shouru_lirun[i].shouru, shouru_lirun[i-1].shouru) - 1)*100;
		shouru_lirun[i].tb_lirun = (My_div(shouru_lirun[i].lirun, shouru_lirun[i-1].lirun) - 1)*100;		
	}

/*	
	for (i=size_shouru_lirun-1; i>3; i--)
	{
		if (shouru_lirun[i-4].shouru != 0)
			shouru_lirun[i].tb_shouru = shouru_lirun[i].shouru / shouru_lirun[i-4].shouru - 1;
		else
			shouru_lirun[i].tb_shouru = 5;

		if 	(shouru_lirun[i-4].lirun != 0)
			shouru_lirun[i].tb_lirun = shouru_lirun[i].lirun / shouru_lirun[i-4].lirun - 1;		
		else
			shouru_lirun[i].tb_lirun = 5;

		if (shouru_lirun[i].tb_lirun < 0 && shouru_lirun[i-4].lirun < 0)	
			shouru_lirun[i].tb_lirun = shouru_lirun[i].tb_lirun * (-1);

		
		shouru_lirun[i].gdqyl = My_div(
			shouru_lirun[i].lirun +  shouru_lirun[i-1].lirun +  shouru_lirun[i-2].lirun + shouru_lirun[i-3].lirun,
			shouru_lirun[i].gdqy + shouru_lirun[i-1].gdqy + shouru_lirun[i-2].gdqy  + shouru_lirun[i-3].gdqy
		) * 4;
	}
*/
	/*
		输出
	*/
	{
		char file_write[64];	
				
		sprintf(file_write,"..\\data\\fin\\%s_tb.txt",gongsi_name);		
		ofstream of_file(file_write);	 
			
		sprintf(writebuf ,"%12s%16s%16s%12s%12s%12s%16s\n","date","sr","lr","srtb","lrtb","gdqyl","gdqy"); 
		of_file << writebuf; 
			
		for (i=0; i<size_shouru_lirun; i++)
		{
			sprintf(writebuf ,"%12d%16.0f%16.0f%12.4f%12.4f%12.3f%16.0f\n",
						shouru_lirun[i].date,
						shouru_lirun[i].shouru,
						shouru_lirun[i].lirun,
						shouru_lirun[i].tb_shouru,
						shouru_lirun[i].tb_lirun,
						shouru_lirun[i].gdqyl,
						shouru_lirun[i].gdqy						
						); 
					of_file << writebuf; 
		}
	}

	return;
}




void uc_BaseData::F10_File_bzh(void)
{
	char file_source[64], file_write[64]; 
	char readbuf[1024];
	unsigned int i;

	if (*gongsi_name == '6')
	{
		sprintf(file_source,"D:\\new_njzq_v6\\vipdoc\\shf10\\%s.txt",gongsi_name);
		sprintf(file_write,"D:\\new_zszq\\vipdoc\\shf10\\%s_std.txt",gongsi_name);
	}
	else
	{
		sprintf(file_source,"D:\\new_njzq_v6\\vipdoc\\szf10\\%s.txt",gongsi_name);
		sprintf(file_write,"D:\\new_zszq\\vipdoc\\szf10\\%s_std.txt",gongsi_name);
	}

	ifstream if_file(file_source);	
	ofstream of_file(file_write);	

	
	while(1)
	{	
		if (if_file.eof())
			break;
			
		memset(readbuf, 0, sizeof readbuf);
		if_file.getline(readbuf, sizeof readbuf); 
		for (i=0; i<strlen(readbuf);i++)
		{

//			if ((int)readbuf[i] == -87 && (int)readbuf[i+1] == -90)  //招商证券使用
			if ((int)readbuf[i] == -93 && (int)readbuf[i+1] == -4)  //南京证券
			{
				readbuf[i] = ' ';
				readbuf[i+1] = ' ';				
			}			
		}
		readbuf[strlen(readbuf)] = '\n';	
		of_file	<< readbuf;
	}

	of_file.close();
	
	return;
	
}



// get fin data from sina data
void uc_BaseData::vf_BaoCun_F10_Shouru_Lirun(void)
{
	char file_lr[64], file_zc[64], readbuf[512];
	ut_Shouru_lirun	 tmp[128];	
	int size_tmp, size_max = 64;
	int i, date;

	if (gongsi_name[0] == '8')
		return;
	
	memset(&tmp, 0, sizeof tmp);
	size_tmp = 0;

	// filename
	sprintf(file_lr, "..\\data\\fin_sou\\%s1", gongsi_name);	
	sprintf(file_zc, "..\\data\\fin_sou\\%s2", gongsi_name);	
	if (MYFile_FileIsExist(file_lr) == NO)
	{
		printf("...file %s not exist!...\n", file_lr);
		return;
	}	
	if (MYFile_FileIsExist(file_zc) == NO)
	{
		printf("...file %s not exist!...\n", file_zc);
		return;
	}	
	ifstream if_zc(file_zc);		
	ifstream if_lr(file_lr);				

	//get date
	if_lr >> readbuf;
	while(1)
	{
		if_lr >> date;			// first date

		if (date == 0)
		{
			goto TAG_save;
		}
		
		if (date == 19700101 )
		{
			if_lr.getline(readbuf, sizeof readbuf);
			break;
		}	

		tmp[size_tmp].date = date;
		size_tmp++;
		
		if (size_tmp >= size_max)
		{
			if_lr.getline(readbuf, sizeof readbuf);
			break;
		}	
	}

	//get danwei of lr table
	i = 0;
	if_lr >> readbuf;
	while(1)
	{
		if_lr >> readbuf;

		if (!strcmp(readbuf, "元"))
			tmp[i].dw = 1;
		else if (!strcmp(readbuf, "千元"))
			tmp[i].dw = 1000;
		else if (!strcmp(readbuf, "百万"))
			tmp[i].dw = 1000000;
			
		i++;
		if ( i >= size_tmp)
		{
			if_lr.getline(readbuf, sizeof readbuf);
			break;
		}	
	}

	//get shouru
	while (1)
	{
		if_lr >> readbuf;

		if (!strcmp(readbuf, "一、营业总收入") || !strcmp(readbuf, "一、营业收入"))
			break;

		if_lr.getline(readbuf, sizeof readbuf);
	}
	
	i = 0;
	while(1)
	{
		if_lr >> tmp[i].shouru;
		tmp[i].shouru = tmp[i].shouru / 10000 * tmp[i].dw;

		i++;
		if ( i >= size_tmp)
		{
			if_lr.getline(readbuf, sizeof readbuf);
			break;
		}	
	}


	//get lirun
	while (1)
	{
		if_lr >> readbuf;

//		if (!strcmp(readbuf, "四、净利润") || !strcmp(readbuf, "归属于母公司的净利润"))
		if (!strcmp(readbuf, "归属于母公司所有者的净利润") || !strcmp(readbuf, "归属于母公司的净利润"))
			break;

		if_lr.getline(readbuf, sizeof readbuf);
	}
	
	i = 0;
	while(1)
	{
		if_lr >> tmp[i].lirun;
		tmp[i].lirun = tmp[i].lirun / 10000 * tmp[i].dw;

		i++;
		if ( i >= size_tmp)
		{
			if_lr.getline(readbuf, sizeof readbuf);
			break;
		}	
	}

	//get gdqy
	//refresha dw
	i = 0;
	if_zc.getline(readbuf, sizeof readbuf);
	if_zc >> readbuf;
	while(1)
	{
		if_zc >> readbuf;

		if (!strcmp(readbuf, "元"))
			tmp[i].dw = 1;
		else if (!strcmp(readbuf, "千元"))
			tmp[i].dw = 1000;
		else if (!strcmp(readbuf, "百万"))
			tmp[i].dw = 1000000;
			
		i++;
		if ( i >= size_tmp)
		{
			if_zc.getline(readbuf, sizeof readbuf);
			break;
		}	
	}


	while (1)
	{
		if_zc >> readbuf;

		if (!strcmp(readbuf, "股东权益合计") || !strcmp(readbuf, "所有者权益(或股东权益)合计"))
			break;

		if_zc.getline(readbuf, sizeof readbuf);
	}

	i = 0;
	while(1)
	{
		if_zc >> tmp[i].gdqy;
		tmp[i].gdqy = tmp[i].gdqy / 10000 * tmp[i].dw;

		i++;
		if ( i >= size_tmp)
		{
			if_zc.getline(readbuf, sizeof readbuf);
			break;
		}	
	}

TAG_save:
	{
		char file_write[64];	
		
	    sprintf(file_write,"..\\data\\fin\\%s_fin.txt",gongsi_name);		
		ofstream of_file(file_write);	 

		sprintf(readbuf,"%12s%16s%16s%16s\n","时间",gongsi_name,"净利润","股东权益"); 
		of_file << readbuf; 

		for (i=size_tmp-1; i>=0; i--)
		{
			sprintf(readbuf,"%12d%16.0f%16.0f%16.0f\n",
				tmp[i].date,
				tmp[i].shouru,
				tmp[i].lirun,
				tmp[i].gdqy
				); 
			of_file << readbuf; 
		}
			
	}	

	return;	
}		


#if 0
// get fin data from f10 data
void uc_BaseData::vf_BaoCun_F10_Shouru_Lirun(void)
{
	char file_source[64];
	char readbuf[1024],writebuf[128];
	float tmp_float;
	int	tmp_int, i,j, date_now, date_num;
	uc_MyString mystring;
	ut_Shouru_lirun ut_tmp_srlr[8];
	int size_tmp_srlr = 0;
    FILE *fp_read;


	/* 
		先要把已有的数据读取出来，这里的数据必须是增量方式处理的，可以包括手工的增加不好的数据
	*/
	{
		sprintf(file_source,"D:\\stock run\\database\\fin\\%s_fin.txt",gongsi_name);	
		ifstream if_file(file_source);	

		if ((fp_read = fopen(file_source,"rb")) != NULL )  //文件存在
		{
			fclose(fp_read);		

			if_file.getline(readbuf, sizeof readbuf); //first line invalid
			while(!if_file.eof())
			{
				if_file >> shouru_lirun[size_shouru_lirun].date;
				if_file >> shouru_lirun[size_shouru_lirun].shouru;
				if_file >> shouru_lirun[size_shouru_lirun].lirun;	
				if_file >> shouru_lirun[size_shouru_lirun].gdqy;	
				
				if_file.getline(readbuf, sizeof readbuf); 

				if (shouru_lirun[size_shouru_lirun].lirun != 0)
					size_shouru_lirun++;
			};
		}
	}

	//标准化F10文件
	F10_File_bzh();
	
	if (*gongsi_name == '6')
	{
		sprintf(file_source,"D:\\new_zszq\\vipdoc\\shf10\\%s_std.txt",gongsi_name);
	}
	else
	{
		sprintf(file_source,"D:\\new_zszq\\vipdoc\\szf10\\%s_std.txt",gongsi_name);
	}


	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[error][%s]源文件不对\n",file_source);
		return;
	}
	fclose(fp_read);
	ifstream if_file(file_source);	

#if 1
	//读取港澳f10的数据
TAG_AGAIN:	
	if (gongsi_name[0] == '3')		//创业板
	{
		while(if_file >> readbuf)
		{
			if (strlen(readbuf) >= 1024 )
			{
				printf("[error]vf_BaoCun_F10_Shouru_Lirun:数据超过1024字节\n");		
				return;
			}			

			//财务数据，但是净资产需要通过利润和净资产收益率来计算
			if (strcmp(readbuf,"【利润构成与盈利能力】")) 
			{	
				continue;
			}			
			
			for (j=0; j<1; j++)						//创业板只有一轮
			{
				i=0;			
				while (if_file >> readbuf)			
				{
					i++;
					if (i == 50) 
						goto TAG_AGAIN;								//如果读取的行数太多，则跳出去					

					if (!strcmp(readbuf,"【5.非经常性损益表】")					//提取完毕结束
					    )	
					{				    
						goto TAG__save;
					}						
						
					if (!strcmp(readbuf,"财务指标(单位)")	)			
					{
						if_file.getline(readbuf, sizeof readbuf);
						mystring.InitString(readbuf);
						date_num = 0;
						
						for (i=0; i<4; i++)  //max 4 date
						{
							tmp_int = mystring.GetNextInt();
							if (tmp_int == -999999) break;
							date_now = tmp_int * 10000;
							tmp_int = mystring.GetNextInt();
							date_now = date_now + tmp_int * 100;
							tmp_int = mystring.GetNextInt();
							date_now = date_now + tmp_int;						

							ut_tmp_srlr[size_tmp_srlr].date = date_now;
							size_tmp_srlr++;
							date_num++;
						}
						
						//主营业务收入,读取各项数据，严格遵守顺序
						while (if_file >> readbuf)														
						{
							if (!strcmp(readbuf,"净资产收益率(%)"))
							{
								if_file.getline(readbuf, sizeof readbuf);
								mystring.InitString(readbuf);
								for (i=0; i<date_num; i++)  //max 4 date
								{
									tmp_float = mystring.GetNextFloat_0();
									if (tmp_float == -999999) break;
									ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqyl = tmp_float / 100;
									ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqy 
										= ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun / ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqyl;
									
								}
								goto TAG__save;
							}
							
							if (!strcmp(readbuf,"主营业务收入(万元)"))
							{
								if_file.getline(readbuf, sizeof readbuf);
								mystring.InitString(readbuf);
								for (i=0; i<date_num; i++)  //max 4 date
								{
									tmp_float = mystring.GetNextFloat_0();
									if (tmp_float == -999999) break;
									ut_tmp_srlr[size_tmp_srlr-date_num+i].shouru = tmp_float;
								}

								continue;
							}
							
							if (!strcmp(readbuf,"净利润(万元)") )
							{
								if_file.getline(readbuf, sizeof readbuf);
								mystring.InitString(readbuf);
								for (i=0; i<date_num; i++)  //max 4 date
								{
									tmp_float = mystring.GetNextFloat_0();
									if (tmp_float == -999999) break;
									ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun = tmp_float;
								}
								continue;
							}
						}
					}
				}
			}	//第二轮结束	
			break;
		}	
	}
	

	//其他板块
	while(if_file >> readbuf)
	{
		if (strlen(readbuf) >= 1024 )
		{
			printf("[error]vf_BaoCun_F10_Shouru_Lirun:数据超过1024字节\n");		
			return;
		}			

		//财务数据，但是净资产需要通过利润和净资产收益率来计算
		if (strcmp(readbuf,"【历年简要财务指标】")) 
		{	
			continue;
		}			
		
		for (j=0; j<2; j++)						//总共2轮数据,最多8个季度的数据
		{
			i=0;			
			while (if_file >> readbuf)			
			{
				i++;
				if (i == 50) 
					goto TAG_AGAIN;								//如果读取的行数太多，则跳出去					

				if (!strcmp(readbuf,"【每股指标】")					//提取完毕结束
				    )	
				{				    
					goto TAG__save;
				}						
					
				if (!strcmp(readbuf,"财务指标(单位)")	)			
				{
					if_file.getline(readbuf, sizeof readbuf);
					mystring.InitString(readbuf);
					date_num = 0;
					
					for (i=0; i<4; i++)  //max 4 date
					{
						tmp_int = mystring.GetNextInt();
						if (tmp_int == -999999) break;
						date_now = tmp_int * 10000;
						tmp_int = mystring.GetNextInt();
						date_now = date_now + tmp_int * 100;
						tmp_int = mystring.GetNextInt();
						date_now = date_now + tmp_int;						

						ut_tmp_srlr[size_tmp_srlr].date = date_now;
						size_tmp_srlr++;
						date_num++;
					}
					
					//主营业务收入,读取各项数据，严格遵守顺序
					while (if_file >> readbuf)														
					{
						if (!strcmp(readbuf,"净资产收益率(%)"))
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqyl = tmp_float / 100;
							}
							continue;
						}
						
						if (!strcmp(readbuf,"主营业务收入(万元)"))
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].shouru = tmp_float;
							}

							continue;
						}
						
						if (!strcmp(readbuf,"净利润(万元)") )
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun = tmp_float;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqy 
									= ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun / ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqyl;
							}

							
							break;  //退出本次循环
						}
					}
				}
			}
		}	//第二轮结束	
		break;
	}	
	

#endif

	
#if 0	
	/* 文件检查完毕,准备开始读取数据 
		读取维塞得的财务数据
	*/
TAG_AGAIN:
	while(if_file >> readbuf)
	{
		if (strlen(readbuf) >= 1024 )
		{
			printf("[error]vf_BaoCun_F10_Shouru_Lirun:数据超过1024字节\n");		
			return;
		}			

		//进入财务指标数据提取循环
		if (strcmp(readbuf,"【历年简要财务指标】") 
			&& strcmp(readbuf,"◆主要财务指标◆") 
			&& strcmp(readbuf,"◆主要财务指标") 			
			&& strcmp(readbuf,"主要财务指标"))	
		{	
			continue;
		}			

		
		for (j=0; j<2; j++)						//总共2轮数据,最多8个季度的数据
		{
			i=0;			
			while (if_file >> readbuf)			
			{
				i++;
				if (i == 50) 
					goto TAG_AGAIN;								//如果读取的行数太多，则跳出去					

				if (!strcmp(readbuf,"◆每股指标◆")					//提取完毕结束
				    || !strcmp(readbuf,"每股指标")
				    || !strcmp(readbuf,"◆每股指标")				    
				    )	
				{				    
					goto TAG__save;
				}						
					
				if (!strcmp(readbuf,"指标\\报告期")						//招商证券
					|| !strcmp(readbuf,"财务指标(单位)") )				//南京证券
				
				{
					if_file.getline(readbuf, sizeof readbuf);
					mystring.InitString(readbuf);
					date_num = 0;
					
					for (i=0; i<4; i++)  //max 4 date
					{
						tmp_int = mystring.GetNextInt();
						if (tmp_int == -999999) break;
						date_now = tmp_int * 10000;
						tmp_int = mystring.GetNextInt();
						date_now = date_now + tmp_int * 100;
						tmp_int = mystring.GetNextInt();
						date_now = date_now + tmp_int;						

						ut_tmp_srlr[size_tmp_srlr].date = date_now;
						size_tmp_srlr++;
						date_num++;
					}
					
					//主营业务收入,读取各项数据，严格遵守顺序
					while (if_file >> readbuf)														
					{
						if (!strcmp(readbuf,"归属母公司股东权益"))
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].gdqy= tmp_float;
							}
						}
						if (!strcmp(readbuf,"主营业务收入(万元)")
							|| !strcmp(readbuf,"营业收入(万元)")) //如果两者都存在，则营业收入在后面进行覆盖
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].shouru = tmp_float;
							}
						}
						
						if (!strcmp(readbuf,"净利润(万元)") )
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun = tmp_float;
							}
						}

						if ( !strcmp(readbuf,"归属母公司净利润(万元)") )
						{
							if_file.getline(readbuf, sizeof readbuf);
							mystring.InitString(readbuf);
							for (i=0; i<date_num; i++)  //max 4 date
							{
								tmp_float = mystring.GetNextFloat_0();
								if (tmp_float == -999999) break;
								ut_tmp_srlr[size_tmp_srlr-date_num+i].lirun = tmp_float;
							}
							break; // get lirun OK, break this turn		
						}
					}
				}
			}
		}	//第二轮结束	
		break;
	};

#endif 
	
TAG__save:

	/* //为了便于增加新数据，以新读取到的数据为准.需要
	   增加是老数据优先还是新数据优先的控制。对于老数据优先，则可以人工修改相关数据，并且得到保存；
	   对于新数据优先，则可以用于提取新数据。

	   另外，为了控制数据，对新于某个日期的数据将不输出 -- 股票软件输入数据都是分步的，并且难做啊
	*/
	{
		int date, is_newfirst;
		char file_write[64];	
		int date_line = 29999999; 
		

		if ( size_tmp_srlr ==  0) return; //不用保存了
		
	    sprintf(file_write,"D:\\stock run\\database\\fin\\%s_fin.txt",gongsi_name);		
		ofstream of_file(file_write);	 

		sprintf(writebuf ,"%12s%16s%16s%16s\n","时间",gongsi_name,"净利润","股东权益"); 
		of_file << writebuf; 

//		is_newfirst = 1;    //新的先输出
		is_newfirst = 0;		

		/*
			如果新的数据比老的数据多，则老的数据 不需要
		*/
		if (size_tmp_srlr > size_shouru_lirun)
		{
			size_shouru_lirun = 0;
		}
		

		if (is_newfirst == 1)
		{
		
			date = ut_tmp_srlr[size_tmp_srlr-1].date;

			for (i=0; i<size_shouru_lirun; i++)
			{
				if (shouru_lirun[i].date >= date) break;

				if (shouru_lirun[i].date >= date_line) continue;
				
				sprintf(writebuf ,"%12d%16.0f%16.0f%16.0f\n",
					shouru_lirun[i].date,
					shouru_lirun[i].shouru,
					shouru_lirun[i].lirun,
					shouru_lirun[i].gdqy
					); 
				of_file << writebuf; 
			}
		
			for (i=size_tmp_srlr-1; i>=0; i--)				//从前往后输出
			{

				if (ut_tmp_srlr[i].date >= date_line) continue;
			
				sprintf(writebuf ,"%12d%16.0f%16.0f%16.0f\n",
					ut_tmp_srlr[i].date,
					ut_tmp_srlr[i].shouru,
					ut_tmp_srlr[i].lirun,
					ut_tmp_srlr[i].gdqy				
					); 
				of_file << writebuf; 
			}
		}

		
		else
		{
			for (i=0; i<size_shouru_lirun; i++)
			{

				if (shouru_lirun[i].date >= date_line) continue;
				
				sprintf(writebuf ,"%12d%16.0f%16.0f%16.0f\n",
					shouru_lirun[i].date,
					shouru_lirun[i].shouru,
					shouru_lirun[i].lirun,
					shouru_lirun[i].gdqy
					); 
				of_file << writebuf; 
			}
			
			if (size_shouru_lirun > 0)
				date = shouru_lirun[size_shouru_lirun-1].date;
			else
				date = 0;
				

			for (i=size_tmp_srlr-1; i>=0; i--)				//从前往后输出
			{
				if (date >= ut_tmp_srlr[i].date) continue;			//旧数据里面有了

				if (ut_tmp_srlr[i].date >= date_line) continue;				

				if ( i == 0 || i == 1)
				{
					if (ut_tmp_srlr[i].lirun == 0 || ut_tmp_srlr[i].gdqy == 0)
						continue;
				}
				
				sprintf(writebuf ,"%12d%16.0f%16.0f%16.0f\n",
					ut_tmp_srlr[i].date,
					ut_tmp_srlr[i].shouru,
					ut_tmp_srlr[i].lirun,
					ut_tmp_srlr[i].gdqy 			
					); 
				of_file << writebuf; 
			}
		}
	}
	
    return ;
}
#endif

int uc_BaseData::vf_SaveBiaoZhunDayK_SINA(char *flie_source)
{
    char file_dest[64];
    FILE *fp_read;
    ut_SINA_DayK dayk;
	char tmp[256];
	int i;
		
    sprintf(file_dest,"..\\data\\dayk\\%s_dayk.txt",gongsi_name);
    ofstream of_file(file_dest);

	if ((fp_read = fopen(flie_source,"rb")) == NULL) 
	{
		printf("[error][%s]源文件不对\n",flie_source);
		return -1;
	}

    printf("[vf_SaveBiaoZhunDayK_SINA]...[%s]...\n", gongsi_name);	
	
	while (!feof(fp_read)) 
	{
        memset(&dayk, 0,sizeof(ut_SINA_DayK));
	    i = fread(&dayk, sizeof(ut_SINA_DayK), 1,fp_read);

		if (i == 0 )
			break;

        {
    	    // 时间 开盘价 最高价 最低价 收盘价 金额 成交量
    		sprintf(tmp, "%12d%12.2f%12.2f%12.2f%12.2f%12.2f%12.2f\n", 
    		    dayk.date, (float)dayk.open/100, (float)dayk.high/100,(float)dayk.low/100,
    		    (float)dayk.close/100, dayk.money/10000, (float)dayk.mount/10000);
    		of_file << tmp;    		    
        }

        
    }

	sprintf(tmp, "[END]\n");
	of_file << tmp;
	
	of_file.close();
	fclose(fp_read);	


    return 0;
    
}



//从大智慧更新所有的权息数据
/*
int     uc_BaseData::pf_BaoCunQuanXi(void)
{
    FILE *fp_read;
    ut_DZH_ChuQuan ut_chuquan;
    char file_gongsi[16], file_write[64], file_back[64];
	char tmp[256];
	struct tm *today;
	char date_str[10];
	time_t long_time;

	int t_int, i;
	float t_float;

	if ((fp_read = fopen("D:\\new_zszq\\T0002\\hq_cache\\gbbq","rb")) == NULL) 
	{
		printf("[error][%s]源文件不对\n",DIR_QuanXiFILE);
		return -1;
	}
	
	fread(&tmp, 2, 1, fp_read);
	while (!feof(fp_read)) 
	{
		fread(&tmp, 4, 1, fp_read);


		memcpy(&t_int, tmp, 4);
		t_int = ~t_int;

		memcpy(&t_float, &t_int, 4);	
		

		i = 0;
	}

	return 1;
}
*/




int     uc_BaseData::pf_BaoCunQuanXi(void)
{
    FILE *fp_read;
    ut_DZH_ChuQuan ut_chuquan;
    ut_DZH_ChuQuan qx[128];
    int size_qx;
    char file_gongsi[16], file_write[64], file_qx_cx[64];
	char tmp[256];
	struct tm *today;
	char date_str[10];
	time_t long_time;
	int i, j, min_date, min_index, qx_cx_date_max;

	if ((fp_read = fopen(DIR_QuanXiFILE,"rb")) == NULL) 
	{
		printf("[pf_BaoCunQuanXi]---------------------[%s]源文件不对\n",DIR_QuanXiFILE);
		return -1;
	}

    //前面12个字节不需要，达到第一个公司的开始.后面是8个字节的公司名sz002024，后面是8个字节的全0
    fread(&ut_chuquan, 12, 1, fp_read); 
    fread(&file_gongsi, 16, 1, fp_read); 
    
    sprintf(file_write,"%s\\%s.txt",DIR_QuanXiSaveDir,file_gongsi+2); //去掉前面的sz或者sh
    ofstream of_file(file_write);
    size_qx = 0;
    
	while (!feof(fp_read)) 
	{
        fread(&ut_chuquan, 4, 1, fp_read);
        if ((*((int*)(&ut_chuquan))) == 0xFFFFFFFF) // 下一个公司的开始
        {
            printf("[pf_BaoCunQuanXi]-----------------保存%s权息数据\n",file_gongsi);
            
			//先要把前面基准的qx_cx数据弄过来，先保存
			{
	            sprintf(file_qx_cx,"D:\\stock run\\database\\quanxi_cx\\%s.txt",file_gongsi+2);

				if (MYFile_FileIsExist(file_qx_cx) == NO)
					goto _TAG_SAVE;
	            
				ifstream if_file_cx(file_qx_cx);	            
    	        qx_cx_date_max = 0;

				if_file_cx >> tmp;
			    while (strcmp(tmp, "[END]"))
				{
					ut_chuquan.date = atoi(tmp);
			        if_file_cx >> ut_chuquan.SongGu;
			        if_file_cx >> ut_chuquan.PeiGu;
					if_file_cx >> ut_chuquan.PeiGuJia;
			        if_file_cx >> ut_chuquan.Fenhong;

					qx_cx_date_max = ut_chuquan.date;

		        	sprintf(tmp,"%12d%12.2f%12.2f%12.2f%12.2f\n",
			        	ut_chuquan.date,
			         	ut_chuquan.SongGu,
			         	ut_chuquan.PeiGu, 
			         	ut_chuquan.PeiGuJia, 
			         	ut_chuquan.Fenhong);
					of_file << tmp;   	

			        if_file_cx.getline(tmp, sizeof tmp);
					if_file_cx >> tmp;		
			    }
            }

_TAG_SAVE:            
			//20131101:按照从小到大保存权息数据
			for (i=0; i<size_qx; i++)
			{
				qx[i].flag = NO;
			}
			for (i=0; i<size_qx; i++)
			{
				min_date = 99999999;				
				for (j=0; j<size_qx; j++)
				{
					if (qx[j].date < min_date && qx[j].flag == NO)
					{
						min_date = qx[j].date;
						min_index = j;
					}
				}

				//输出
				if (min_date != 99999999)
				{
					if (qx[min_index].date > qx_cx_date_max)
					{
		        	sprintf(tmp,"%12d%12.2f%12.2f%12.2f%12.2f\n",
			        	qx[min_index].date,
			         	qx[min_index].SongGu,
			         	qx[min_index].PeiGu, 
			         	qx[min_index].PeiGuJia, 
			         	qx[min_index].Fenhong);
					of_file << tmp;   	
					}
					qx[min_index].flag = YES;
				}	
			}
			size_qx = 0;
			

            //关闭前面的文件，打开新的文件
            sprintf(tmp, "[END]\n");
	        of_file << tmp;
            of_file.close();
            
            fread(&file_gongsi, 16, 1, fp_read); 
            sprintf(file_write,"%s\\%s.txt",DIR_QuanXiSaveDir,file_gongsi+2); //去掉前面的sz或者sh

			//打开新文件
            of_file.open(file_write);

	        continue;	        
        }
        fread(&(ut_chuquan.SongGu), 4, 4, fp_read);

        //转换一次时间，从秒计算的时间转换到日期时间
        long_time = (long)ut_chuquan.date;
        today = localtime( &long_time);
		if (today != NULL)
			sprintf(date_str,"%04d%02d%02d",today->tm_year+1900,today->tm_mon+1,today->tm_mday);
		ut_chuquan.date = atoi(date_str);

		//临时保存
		memcpy(&qx[size_qx], &ut_chuquan, sizeof ut_chuquan);
		size_qx++;
		
	}

	sprintf(tmp, "[END]\n");
	of_file << tmp;
	
	of_file.close();
	fclose(fp_read);	

	return 0;
}




void   uc_BaseData::Tiqu_tdx_index_Info(void)
{
    char file_read[64], file_write[64], file_write_config[64];
	char buf[256],readbuf[256];
	char name[32], code2[32];
	int code1, tmp1, tmp2, tmp3, i;


    printf("[ok]Get tdx index info......\n");

	//get index info    
    strcpy(file_write,"..\\data\\tdx_index.txt");
    ofstream of_file(file_write);	
    strcpy(file_write_config,"..\\data\\config.txt");
    ofstream of_file_config(file_write_config);	
    

	sprintf(file_read, "c:\\zd_zszq\\T0002\\hq_cache\\tdxzs.cfg");
	ifstream if_file(file_read);

	if_file >> readbuf;
    while (strlen(readbuf) > 5)
	{
		for (i=0; i<(int)strlen(readbuf); i++)
		{
			if (readbuf[i] ==  '|')	 
				readbuf[i] = ' ';
			else if (readbuf[i] ==  ' ')	 
				readbuf[i] = '*';
		}
		
		sscanf(readbuf, "%s %d %d %d %d %s", name, &code1, &tmp1, &tmp2, &tmp3, code2);

		if (code1 >= 880301 && code1 <= 880497 )
		{
	        sprintf(buf,"%10d%16s\n%10s%16d\n",code1, name, code2, code1);
			of_file << buf;	

			if (strlen(code2) == 5)		// 一类行业
			{
				if (code1 != 880497)
				{
			        sprintf(buf,"[HangYe]%12d%12s\n[HangYe_END]\n\n",code1, name);
					of_file_config << buf;				
				}	
			}
		}

		memset(readbuf, 0, sizeof readbuf);
		if_file >> readbuf;
	}

	sprintf(buf,"[END]");
	of_file << buf;	
	of_file.close();

	sprintf(buf,"[File_END]");
	of_file_config << buf;	
	of_file_config.close();
	

	//get gongsi and index info
	{
		char hycode_father[32],hycode_son[32],hyname_father[32],hyname_son[32];
		
	    strcpy(file_write,"..\\data\\tdx_gongsi.txt");
	    ofstream of_file2(file_write);	

		sprintf(file_read, "c:\\zd_zszq\\T0002\\hq_cache\\tdxhy.cfg");
		ifstream if_file2(file_read);

		if_file2 >> readbuf;
	    while (strlen(readbuf) > 5)
		{
			for (i=0; i<(int)strlen(readbuf); i++)
			{
				if (readbuf[i] ==  '|')	 
					readbuf[i] = ' ';
				else if (readbuf[i] ==  ' ')	 
					readbuf[i] = '*';
			}
			
			sscanf(readbuf, "%d %s %s", &tmp1, name, code2);
    		printf("[Tiqu_tdx_index_Info]...[%s]\n", name);			
			
			if (name[0] == '9')
			{
				memset(readbuf, 0, sizeof readbuf);
				if_file2 >> readbuf;
				continue;
			}
			
			if (strlen(code2) == 5)
			{
				strcpy(hycode_father, code2);
				strcpy(hycode_son, code2);
			}
			else
			{
				strcpy(hycode_father, code2);
				strcpy(hycode_son, code2);
				hycode_father[strlen(hycode_father)-2] = '\0';
			}

			strcpy(hycode_father, GetTdxHangYeName(hycode_father));
			strcpy(hycode_son, GetTdxHangYeName(hycode_son));			

			strcpy(hyname_father, GetTdxHangYeName(hycode_father));
			strcpy(hyname_son, GetTdxHangYeName(hycode_son));			

	        sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",name, GetGongsiName(name),
	        	hycode_father, hycode_son,  Is_rzrq(name),hyname_father,hyname_son);
			of_file2 << buf;	

			memset(readbuf, 0, sizeof readbuf);
			if_file2 >> readbuf;
		}

		
     	for (i=880300;i<=880399;i++)
    	{
    		sprintf(readbuf,"%06d", i);

  			printf("[Tiqu_tdx_index_Info]...[%s]\n", readbuf);		    		
  			sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",readbuf, GetTdxHangYeName(readbuf),
	        	readbuf, readbuf, 0,readbuf, readbuf);
			of_file2 << buf;  		
 
    	}    

     	for (i=880400;i<=880499;i++)
    	{
    		sprintf(readbuf,"%06d", i);

  			printf("[Tiqu_tdx_index_Info]...[%s]\n", readbuf);		    		    		
  			sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",readbuf, GetTdxHangYeName(readbuf),
	        	readbuf, readbuf, 0, readbuf, readbuf);
			of_file2 << buf;  		
    	}    

		sprintf(buf,"[END]");
		of_file2 << buf;	
	}
	

	return;    
}

void   uc_BaseData::Tiqu_HYinfo(void)
{

	ut_TDX_HangYe  tmp_buf;
    char file_dest[64], file_source[64];
    FILE *fp_read;
    int i, ret;


	printf("[提取行业公司数据]......\n");	
    sprintf(file_dest,"..\\data\\hangye_gongsi.txt");
    ofstream of_file(file_dest);

    sprintf(file_source,"c:\\zd_zszq\\T0002\\hq_cache\\block_gn.dat");
	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_HYinfo]源文件不对\n");
		return;
	}

	memset(&tmp_buf, 0, sizeof tmp_buf);
	for (i=0; i<0x18; i++)
	{
		fread(&tmp_buf, 1, 16, fp_read);
	}
	fread(&tmp_buf, 1, 2, fp_read);	//15个无效数据	


	while (1) 
	{
		ret  =	fread(&tmp_buf, 1, 13, fp_read);
		if (ret == 0)
			break;
			
		fread(&(tmp_buf.gongsi), 1, 2800, fp_read);

		
		//输出到文件
		of_file << tmp_buf.name;  of_file << "\n";
		for (i=0; i<400; i++)
		{
			if (tmp_buf.gongsi[i][0] != 0)
			{
				of_file << "  "; 
				of_file <<  tmp_buf.gongsi[i]; of_file << "  "; 
				of_file <<  GetGongsiName(tmp_buf.gongsi[i]);
				of_file << "\n";
			}
		}

		of_file << "##\n\n";

    }
	fclose(fp_read);	

	////////////////////////////////////////////////////////////////////////////	
    sprintf(file_source,"c:\\zd_zszq\\T0002\\hq_cache\\block_fg.dat");
	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_HYinfo]源文件不对\n");
		return;
	}

	memset(&tmp_buf, 0, sizeof tmp_buf);
	for (i=0; i<0x18; i++)
	{
		fread(&tmp_buf, 1, 16, fp_read);
	}
	fread(&tmp_buf, 1, 2, fp_read);	//15个无效数据	


	while (1) 
	{
		ret  =	fread(&tmp_buf, 1, 13, fp_read);
		if (ret == 0)
			break;
			
		fread(&(tmp_buf.gongsi), 1, 2800, fp_read);

		
		//输出到文件
		of_file << tmp_buf.name;  of_file << "\n";
		for (i=0; i<400; i++)
		{
			if (tmp_buf.gongsi[i][0] != 0)
			{
				of_file << "  "; 
				of_file <<  tmp_buf.gongsi[i]; of_file << "  "; 
				of_file <<  GetGongsiName(tmp_buf.gongsi[i]);
				of_file << "\n";
			}
		}

		of_file << "##\n\n";

    }

	////////////////////////////////////////////////////////////////////////////	
    sprintf(file_source,"c:\\zd_zszq\\T0002\\hq_cache\\block_zs.dat");
	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_HYinfo]源文件不对\n");
		return;
	}

	memset(&tmp_buf, 0, sizeof tmp_buf);
	for (i=0; i<0x18; i++)
	{
		fread(&tmp_buf, 1, 16, fp_read);
	}
	fread(&tmp_buf, 1, 2, fp_read);	//15个无效数据	


	while (1) 
	{
		ret  =	fread(&tmp_buf, 1, 13, fp_read);
		if (ret == 0)
			break;
			
		fread(&(tmp_buf.gongsi), 1, 2800, fp_read);

		
		//输出到文件
		of_file << tmp_buf.name;  of_file << "\n";
		for (i=0; i<400; i++)
		{
			if (tmp_buf.gongsi[i][0] != 0)
			{
				of_file << "  "; 
				of_file <<  tmp_buf.gongsi[i]; of_file << "  "; 
				of_file <<  GetGongsiName(tmp_buf.gongsi[i]);
				of_file << "\n";
			}
		}

		of_file << "##\n\n";

    }

	


	of_file << "[END]\n\n";
	
	of_file.close();
	fclose(fp_read);	

    return;
    
}


void   uc_BaseData::Tiqu_hs300_Info(void)
{

    char file_dest[64], file_source[64];
    FILE *fp_read;
	char tmp_buf[16];
	int i;


	printf("[Tiqu_hs300_Info]......\n");	
    sprintf(file_dest,"..\\data\\hs300.txt");
    ofstream of_file(file_dest);

    sprintf(file_source,"c:\\zd_zszq\\T0002\\hq_cache\\block_zs.dat");

	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_HYinfo]源文件不对\n");
		return;
	}

	memset(tmp_buf, 0, sizeof tmp_buf);
	while (!feof(fp_read)) 
	{
		//前面200行无效数据
		for (i=0; i<200; i++)
		{
			fread(tmp_buf, 1, 16, fp_read);
		}
		fread(tmp_buf, 1, 12, fp_read);	

		//开始读取有效数据
		for (;;)
		{
			memset(tmp_buf, 0, sizeof tmp_buf);
			fread(tmp_buf, 1, 7, fp_read);	//读取code
			if ( tmp_buf[0] != '\0')
			{
				of_file << tmp_buf;
				of_file << "\n";
			}
			else
			{
				of_file << "[END]\n";
				return;
			}	
		}
    }

	sprintf(tmp_buf, "[END]\n");
	of_file << tmp_buf;
	
	of_file.close();
	fclose(fp_read);	

    return;
    
}

char* uc_BaseData::GetTdxHangYeName(char *key)
{

	static char name[64];
	char file_source[64];
	char readbuf[1024];	

	strcpy(name, "NULL");

	sprintf(file_source,"D:\\stock run\\database\\tdx_index.txt");
	ifstream if_file(file_source);		

	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return name;
	
		if (strcmp(readbuf,key) )
		{	
			if_file.getline(readbuf, sizeof readbuf);
			continue;
		}		
		
		if_file >> name;
		break;
	}	
	
	return name;
}

char* uc_BaseData::GetTdxGongsiHangYe_son(char *key)
{

	static char name[64];
	char file_source[64];
	char readbuf[1024];	

	strcpy(name, "NULL");

	sprintf(file_source,"D:\\stock run\\database\\tdx_gongsi.txt");
	ifstream if_file(file_source);		

	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return name;
	
		if (strcmp(readbuf,key) )
		{	
			if_file.getline(readbuf, sizeof readbuf);
			continue;
		}		
		
		if_file >> name;
		if_file >> name;		
		if_file >> name;				
		break;
	}	
	
	return name;
}


int uc_BaseData::Is_rzrq(char *key)
{

	char file_source[64];
	char readbuf[1024];	


	sprintf(file_source,"D:\\stock run\\database\\rzrq.txt");
	ifstream if_file(file_source);		

	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return 0;
	
		if (!strcmp(readbuf,key) )
		{	
			return 1;
		}		
		if_file.getline(readbuf, sizeof readbuf);
	}	
	
	return 0;
}


int uc_BaseData::Is_hs300(char *code)
{

	char file_source[64];
	char readbuf[1024];	


	sprintf(file_source,"..\\data\\hs300.txt");
	ifstream if_file(file_source);		

	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return NO;
	
		if (!strcmp(readbuf,code) )
		{	
			return YES;
		}		
		if_file.getline(readbuf, sizeof readbuf);
	}	
	
	return NO;
}


char* uc_BaseData::GetGongsiName(char *code)
{

	static char name[64];
	char file_source[64];
	char readbuf[1024];	

	strcpy(name, "NULL");

	sprintf(file_source,"..\\data\\codename.txt");
	ifstream if_file(file_source);		

	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			return name;
			
		if (strcmp(readbuf,code) )
		{	
			if_file.getline(readbuf, sizeof readbuf);
			continue;
		}		
		
		if_file >> name;
		break;
	}	
	
	return name;
}


void   uc_BaseData::Tiqu_880529(void)
{

    char file_dest[64], file_source[64];
    FILE *fp_read;
	char tmp_buf[16];
	int i, invalid_line, invalid_blank;
	

	printf("[Tiqu_880529]......\n");	
    sprintf(file_dest,"..\\mode1\\880529.txt");
    ofstream of_file(file_dest);

    sprintf(file_source,"D:\\new_zszq\\T0002\\hq_cache\\block_gn.dat");

	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_880529]源文件不对\n");
		return;
	}

	invalid_line = 0xa66;
	invalid_blank = 2;

	memset(tmp_buf, 0, sizeof tmp_buf);
	while (!feof(fp_read)) 
	{
		//前面24行无效数据
		for (i=0; i<invalid_line; i++)
		{
			fread(tmp_buf, 1, 16, fp_read);
		}
		fread(tmp_buf, 1, invalid_blank, fp_read);	//15个无效数据

		//开始读取有效数据
		for (;;)
		{
			memset(tmp_buf, 0, sizeof tmp_buf);
			fread(tmp_buf, 1, 7, fp_read);	//读取code
			if ( tmp_buf[0] != '\0')
			{
				of_file << tmp_buf;
				of_file << "\n";
			}
			else
			{
				of_file << "[END]\n";
				return;
			}	
		}
    }

	sprintf(tmp_buf, "[END]\n");
	of_file << tmp_buf;
	
	of_file.close();
	fclose(fp_read);	

    return;
    
}


void   uc_BaseData::Tiqu_tdx_hangye(char *file_from, char *file_to, int line, int blank)
{

    char file_dest[64], file_source[64];
    FILE *fp_read;
	char tmp_buf[16];
	int i, invalid_line, invalid_blank;
	

	printf("[%s]......\n",file_to);	
	
    sprintf(file_dest,"..\\mode1\\%s.txt", file_to);
    ofstream of_file(file_dest);

    sprintf(file_source,"D:\\new_zszq\\T0002\\hq_cache\\%s", file_from);
	if ((fp_read = fopen(file_source,"rb")) == NULL) 
	{
		printf("[Tiqu_HYinfo]源文件不对\n");
		return;
	}

	invalid_line = line;
	invalid_blank = blank;

	memset(tmp_buf, 0, sizeof tmp_buf);
	while (!feof(fp_read)) 
	{
		//前面24行无效数据
		for (i=0; i<invalid_line; i++)
		{
			fread(tmp_buf, 1, 16, fp_read);
		}
		fread(tmp_buf, 1, invalid_blank, fp_read);	//15个无效数据

		//开始读取有效数据
		for (;;)
		{
			memset(tmp_buf, 0, sizeof tmp_buf);
			fread(tmp_buf, 1, 7, fp_read);	//读取code
			if ( tmp_buf[0] != '\0')
			{
				of_file << tmp_buf;
				of_file << "    ";
				of_file << GetGongsiName(tmp_buf);
				of_file << "\n";
				
			}
			else
			{
				of_file << "[END]\n";
				return;
			}	
		}
    }

	sprintf(tmp_buf, "[END]\n");
	of_file << tmp_buf;
	
	of_file.close();
	fclose(fp_read);	

    return;
    
}


void uc_BaseData::Tiqu_hy_for_aliyun(void)
{
    char file_read[64], file_write[64], file_write_config[64];
	char buf[256],readbuf[256];
	char name[32], code2[32];
	int code1, tmp1, tmp2, tmp3, i;


    printf("[ok]Get tdx index info......\n");

	//get index info    
    strcpy(file_write,"..\\data\\tdx_index.txt");
    ofstream of_file(file_write);	
    strcpy(file_write_config,"..\\data\\config.txt");
    ofstream of_file_config(file_write_config);	
    
    strcpy(file_write,"..\\data\\index.txt");
    ofstream of_file_ali(file_write);	 
    sprintf(buf,"code,name,c_code,c_name\n");
    of_file_ali << buf;
    

	sprintf(file_read, "c:\\zd_zszq\\T0002\\hq_cache\\tdxzs.cfg");
	ifstream if_file(file_read);

	if_file >> readbuf;
    while (strlen(readbuf) > 5)
	{
		for (i=0; i<(int)strlen(readbuf); i++)
		{
			if (readbuf[i] ==  '|')	 
				readbuf[i] = ' ';
			else if (readbuf[i] ==  ' ')	 
				readbuf[i] = '*';
		}
		
		sscanf(readbuf, "%s %d %d %d %d %s", name, &code1, &tmp1, &tmp2, &tmp3, code2);

		if (code1 >= 880301 && code1 <= 880497 )
		{
	        sprintf(buf,"%10d%16s\n%10s%16d\n",code1, name, code2, code1);
			of_file << buf;	

			if (strlen(code2) == 5)		// 一类行业
			{
				if (code1 != 880497)
				{
			        sprintf(buf,"[HangYe]%12d%12s\n[HangYe_END]\n\n",code1, name);
					of_file_config << buf;				
				}	
			}
		}

		memset(readbuf, 0, sizeof readbuf);
		if_file >> readbuf;
	}

	sprintf(buf,"[END]");
	of_file << buf;	
	of_file.close();

	sprintf(buf,"[File_END]");
	of_file_config << buf;	
	of_file_config.close();
	

	//get gongsi and index info
	{
		char hycode_father[32],hycode_son[32],hyname_father[32],hyname_son[32];
		
	    strcpy(file_write,"..\\data\\tdx_gongsi.txt");
	    ofstream of_file2(file_write);	

		sprintf(file_read, "c:\\zd_zszq\\T0002\\hq_cache\\tdxhy.cfg");
		ifstream if_file2(file_read);

		if_file2 >> readbuf;
	    while (strlen(readbuf) > 5)
		{
			for (i=0; i<(int)strlen(readbuf); i++)
			{
				if (readbuf[i] ==  '|')	 
					readbuf[i] = ' ';
				else if (readbuf[i] ==  ' ')	 
					readbuf[i] = '*';
			}
			
			sscanf(readbuf, "%d %s %s", &tmp1, name, code2);
    		printf("[Tiqu_tdx_index_Info]...[%s]\n", name);			
			
			if (name[0] == '9')
			{
				memset(readbuf, 0, sizeof readbuf);
				if_file2 >> readbuf;
				continue;
			}
			
			if (strlen(code2) == 5)
			{
				strcpy(hycode_father, code2);
				strcpy(hycode_son, code2);
			}
			else
			{
				strcpy(hycode_father, code2);
				strcpy(hycode_son, code2);
				hycode_father[strlen(hycode_father)-2] = '\0';
			}

			strcpy(hycode_father, GetTdxHangYeName(hycode_father));
			strcpy(hycode_son, GetTdxHangYeName(hycode_son));			

			strcpy(hyname_father, GetTdxHangYeName(hycode_father));
			strcpy(hyname_son, GetTdxHangYeName(hycode_son));			

	        sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",name, GetGongsiName(name),
	        	hycode_father, hycode_son,  Is_rzrq(name),hyname_father,hyname_son);
			of_file2 << buf;

			sprintf(buf,"%s,%s,%s,%s\n",name, GetGongsiName(name),hycode_father,hyname_father);
			of_file_ali << buf;

			memset(readbuf, 0, sizeof readbuf);
			if_file2 >> readbuf;
		}

		
     	for (i=880300;i<=880399;i++)
    	{
    		sprintf(readbuf,"%06d", i);

  			printf("[Tiqu_tdx_index_Info]...[%s]\n", readbuf);		    		
  			sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",readbuf, GetTdxHangYeName(readbuf),
	        	readbuf, readbuf, 0,readbuf, readbuf);
			of_file2 << buf;  		
 
    	}    

     	for (i=880400;i<=880499;i++)
    	{
    		sprintf(readbuf,"%06d", i);

  			printf("[Tiqu_tdx_index_Info]...[%s]\n", readbuf);		    		    		
  			sprintf(buf,"%10s%16s%16s%16s%6d%16s|%s\n",readbuf, GetTdxHangYeName(readbuf),
	        	readbuf, readbuf, 0, readbuf, readbuf);
			of_file2 << buf;  		
    	}    

		sprintf(buf,"[END]");
		of_file2 << buf;	
	}
	

	return;    
}

void   uc_BaseData::pf_Baocun_code_and_name(void)
{
    FILE *fp_read;
    char file_read[64], file_write[64];
	char buf[256];
	ut_DZH_CodeName codename;
	int i;

    printf("[ok]提取代码和公司的对应表......\n");
    strcpy(file_write,"..\\data\\codename.txt");
    ofstream of_file(file_write);
    

    //读取深圳公司的数据
    strcpy(file_read, "c:\\dzh365\\drvdata\\SZ\\FullDatFile.dat");
	if ((fp_read = fopen(file_read,"rb")) == NULL) 
	{
		printf("[error][%s]源文件不对\n",file_read);
        exit(0);
	}
    fread(buf, 1, 118, fp_read); //120个文件头数据不要
	while (!feof(fp_read)) 
    {
        fread(&codename, sizeof codename, 1, fp_read);
        if (*(int*)(&codename) == 0)
            break;

        //调整一下名称中的空格，如果是空格则调整到‘0’，空格在后续的读取中会有问题
        for (i=0; i<16; i++)
        {
            if (codename.code[i] == ' ')
                codename.code[i] = '0';
        }

        if (codename.code[0] == '0' || codename.code[0] == '3')
        {
    		sprintf(buf,"%6s%16s\n",codename.code, codename.name);
	    	of_file << buf;
	    }
    }
    fclose(fp_read);	


    //读取上海的数据
    strcpy(file_read, "c:\\dzh365\\drvdata\\SH\\FullDatFile.dat");
	if ((fp_read = fopen(file_read,"rb")) == NULL) 
	{
		printf("[error][%s]源文件不对\n",file_read);
        exit(0);
	}
    fread(buf, 1, 118, fp_read); //120个文件头数据不要
	while (!feof(fp_read)) 
    {
        fread(&codename, sizeof codename, 1, fp_read);
        if (*(int*)(&codename) == 0)
            break;

        //调整一下名称中的空格，如果是空格则调整到‘0’，空格在后续的读取中会有问题
        for (i=0; i<16; i++)
        {
            if (codename.code[i] == ' ')
                codename.code[i] = '0';
        }
        //如果非6开头的，全部不要
        if (codename.code[0] != '6')
            continue;
            
		sprintf(buf,"%6s%16s\n",codename.code, codename.name);
		of_file << buf;
    }
    fclose(fp_read);	


	sprintf(buf, "[END]\n");
	of_file << buf;
    
	of_file.close();
    return;
    
}
