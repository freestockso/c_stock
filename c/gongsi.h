/*
    公司对象的头文件
    一个公司对象对内存的要求大小:
    
    1个K线的大小是64个字节；
    一个公司的K线2048个；
    则 64 * 2048 = 2(6+10) = 64K

    总共有3000个公司需要在market分析，则大概300-400兆左右，问题不是太大；
    
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

#ifndef _GONGSI_H_
#define	_GONGSI_H_


#include "macro_define.h"
#include "struct_define.h"

typedef struct {
	char gscode[8];
	char hycode[8];
} ut_CodeHY;


typedef struct {
		int   			date;
		float           open;
		float           high;
		float           low;
		float           close;
		float           money;      //万元
		float           chuQuanJia;
		float           QD_120_1;
		float			p_man;
		float			p_woman;
		float 		p30d;				//for gongsi and for hangye
		float 		p120d;				//for gongsi and for hangye
		float		yl30;
		float		cj30;				//for gongsi and for hangye
		float 		syl30;
		float		yl250;				//for gongsi and for hangye
		float		cj250;		
		float		syl250;
	} ut_DayK;


typedef struct {
		unsigned int    date;
		float           songGu;
		float           peiGu;
		float           peiGuJia;
		float           fenhong;
	}ut_QuanXi;



class uc_GongSi {
public:

	//基础信息
    char    code[16];
    char    name[16];
    int     type;      
    float	  fenshu;
	uc_GongSi *phangye;			//HY	
	uc_GongSi *pnext_by_fenshu;	//paiming by QD fenshu

	//日线数据和财务数据
	//ut_DayK     *pdayk;           	//外部可访问，便于对比
	ut_DayK     pdayk[GONGSI_MaxLineDayK];
    int     	dayk_size, dayk_size_first;
    
	//QX info
    ut_QuanXi   quanxi[GONGSI_MaxLineQuanXi];
    int     	quanxi_size;

    //fin data
    ut_Shouru_lirun shouru_lirun[128];
	int 			shouru_lirun_size, shouru_lirun_size_first;
	
	float			QDpaiming;
	int				num_gongsi_hy;
	float 			PE;

	//计算数据
    int     flag;				//flag for tmp used		

    int     Init(char *);
	void    SetType(void);
    
    int     readQuanXi(void);	
	void	readFin(void);				//read finance data	
    int     readDK(void);
	int   	readBinData(void);


    void    Cal_Qiangdu(void);          //计算公司强度，相对于基准指数  
    void	Cal_p_woman(void);
    void	Cal_p_man(void);	
    void	Cal_cqj(void);
	void    Cal_pe(void);

    void	Cal_p30d(void);
    void	Cal_p120d(void);    
    void	Cal_syl30(void);
    void	Cal_syl250(void);    

	int 	Is_InGNHY(char *hyname);

    int 	Is_InHY(char *hycode, char* hyname);
    int 	Is_399102(void);
    int 	Is_399101(void);
    int 	Is_399300(void);
	int 	Is_czg(void);
	int 	Is_cxg(void);
	int 	Is_czg_low(void);    


	

	void	Prt_Info(char*);
    int     Prt_QD(void);
	void	Output_view(void);
    
	float	Get_songgu(unsigned int, unsigned int);
	int		Get_IndexByDate(int date);
	char* 	GetStr_p30p120(void);
	char* 	GetStr_p30d_hy(void);
	char* 	GetStr_p30d_trend_hy(void);
	char* 	GetStr_syl30_hy(void);
	char* 	GetStr_syl250_hy(void);
	char* 	GetStr_syl30_one(void);
	char* 	GetStr_syl250_one(void);	
	char* 	GetStr_syl30_czg(void);
	char* 	GetStr_tag_hy(void);
	char* 	GetStr_tag_zs(void);
	char* 	GetStr_fin(void);
	

	char*	GetHYCode(void);
	int 	Get_fin_index(int date);
	float 	Get_today_gb(void);
	



	void    Moni_xz_DaykAndFin(int);		// used in moni deal

    
private:
    
};


#endif
