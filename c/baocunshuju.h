/*
此类用来标准化数据，将各个交易程序的数据进行标准化，放到对应的公司目录下，包括公司和行业。
数据的来源可能是sina的行情软件，也可能是大智慧/招商证券等行情软件
输出的数据:
1)标准日线数据；-Day.txt

此对象的运行单位是公司，或者行业

*/


#ifndef _BAOCUNSHUJU_H_
#define  _BAOCUNSHUJU_H_

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




typedef struct {
    int date;
    float SongGu;
    float PeiGu;
    float PeiGuJia;
    float Fenhong;
	int flag;
}ut_DZH_ChuQuan;




//sina证券行情软件的K线数据结构
typedef struct {
    int date;
    
    int open;
    int high;
    int low;
    int close;

    float money;
    int mount;
    int yestoday_close;

} ut_SINA_DayK;


typedef struct {
    char code[10];
    char name[16];    
    char res[42];
} ut_DZH_CodeName;


typedef struct {
    char name[9];
    int  num;
    char gongsi[400][7];    
} ut_TDX_HangYe;




class uc_BaseData {

public:
	char  	        gongsi_name[10];                   //公司或者行业
	int           	data_type;            //数据类型；	
	ut_Shouru_lirun shouru_lirun[128];
	int				size_shouru_lirun;
	
	int  			pf_BaoCunDayK(void);
	int             pf_BaoCunQuanXi(void);
	int     		pf_BaoCunQuanXi_365(char *filename);
	int     		pf_BaoCunQuanXi_CX(void);

	void            pf_Baocun_code_and_name(void);	
	void			pf_BaoCun_F10_data(void);	
	void			pf_BaoCun_F10_gb(void);
	void            Tiqu_tdx_index_Info(void);
	char* 			GetTdxHangYeName(char *key);
	char* 			GetTdxGongsiHangYe_son(char *key);
	char* 			GetGongsiName(char *code);
	void   			Tiqu_HYinfo(void);
	void			Tiqu_sbzc_Info(void);
	void			Tiqu_hs300_Info(void);
	int 			Is_rzrq(char*);
	int 			Is_hs300(char *);

	void   			Tiqu_880529(void);
	void   			Tiqu_tdx_hangye(char *file_from, char *file_to, int line, int blank);
	void 			Tiqu_hy_for_aliyun(void);
	

	


private:
	int 			vf_BaoCunShuJu_SINA(void);
	int 			vf_BaoCunShuJu_DZH(void);	
	int             vf_GetDealSourceFileName_SINA(char *);	
	int             vf_SaveBiaoZhunDayK_SINA(char *);
	void 			F10_File_bzh(void);
	void			vf_BaoCun_F10_Shouru_Lirun(void);
	void			Compute_Shouru_lirun(void);
	void			vf_Get_F10_Shouru_Lirun(void);	
	
} ;


#endif
