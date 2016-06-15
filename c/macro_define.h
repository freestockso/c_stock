#ifndef _MACRO_DEFINE_
#define  _MACRO_DEFINE_




#define		STR_FH		"\n----------------------------------------------------------------------------------------------\n"
#define		STR_5KG	"     "
#define		STR_10KG	"          "


#define    YES   	1
#define    NO		0
#define    INVALID_INDEX						-1
#define    INVALID_CONFIG 					-999


//交易数据来源定义
#define _SINA_  1						//sina的数据
#define _DZH_   2						//dzh的数据


//K线数据类型
#define GONGSITYPE_GONGSI       1		//公司
#define GONGSITYPE_HANGYE       2		//行业
#define GONGSITYPE_ZHISHU       3		//指数
#define GONGSITYPE_NULL          4	//无效公司


//公司的各种最大参数
#define GONGSI_MaxLineDayK          	2048    			//日K线的最大容量，大概8年的数据
#define GONGSI_MaxLineQuanXi        	64      			//权息数据的最大容量，大概5年左右的数据，32次
#define GONGSI_MinLineDatK			120
#define MAX_HANGYE						128
#define MAX_GONGSI_IN_HANGYE			128
#define GONGSi_MaxLineGb				256


//各种路径定义
#define DIR_QuanXiFILE      "c:\\dzh365\\download\\PWR\\full.pwr"
#define DIR_QuanXiSaveDir   "..\\data\\qx"



//动作定义:
#define 	A_NULL             9999 
#define 	A_d2bin			100
#define     A_gDK        		101  
#define     A_gQX           	102	
#define     A_gHY           	103
#define     A_gHY4ali        	104
#define     A_rFINList			201 
#define		A_cHYF				202 

#define		A_m0				0
#define     A_m1               	1	
#define		A_m2				2	




#endif
