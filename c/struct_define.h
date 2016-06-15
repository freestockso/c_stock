#ifndef    _STRUCT_DEFINE_
#define	    _STRUCT_DEFINE_

#define MAX_JY_RECORD 						512

//fin for gongsi and HY
typedef struct {
	int   	date;				//时间yyyymm，季度统计
	float 	shouru;			//收入			
	float 	lirun;			//净利润
	float 	gdqy;				//股东权益
	int 	dw;				//单位
	
	float 	tb_shouru;		//收入同比增加
	float 	tb_lirun;		//利润同比增加
	float 	gdqyl;			//股东权益率

	int  	gongsi_num_in_hangye;		//行业中的公司总数，在行业中使用, invalid in gongsi fin
}ut_Shouru_lirun;


//do not consider RongQuan
typedef struct {
	char 			code[10];
	char			hy_code[10];
	char			name[16];	
	int				type;
	int  			state; 					//状态
	int     		is_exist_today;		//更新每天操作文件的时候进行标记
	int 			is_caozuo_today;		//今天是否进行了操作，用于今天操作的输出
	int				days_hold;

	//交易情况
	unsigned int 	buy_date;
	int				buy_type;
	float 			buy_money;
	float			buy_price;				//除权价格，close价格
	float			buy_price2;

	unsigned int 	sale_date;	
	float			sale_money;	
	float			sale_price;			//除权价格，close价格
	float			sale_price2;	

	//市值 如果是融券，则是负值，代表属于负债部分
	float 			value_gongsi;
	float   		syl;

	float 			next_index;
}ut_JY_record;


typedef struct {
	unsigned int 	date;
	int 			jzzs_index;

	//资产负债情况
	float			zong_zi_chan;					//总资产
	float 			jing_zi_chan;					//净资产
	float   		jing_zi_chan_max;				//最大净资产，用于计算最大回撤
	float			rong_zi_fu_zhai;				//融资负债			
	float			rong_quan_fu_zhai;			//融券负债，用于计算利息，注意融券负债计算利息部分是根据第一天融券价格计算，而非动态价格计算
													//但是从计算方便的角度，暂时按照统一计算，差别应该不会很大	
	float 			ke_yong_cash;					//可用现金，包括自有现金，融资现金，融券现金等
	float			jin_zii_chan_fu_zhai_bi;		//净资产负债比
	float			ke_rong_zi_jin;				//可融资金
	float			ke_rong_zheng_quan;			//可融证券


	//持股和买卖情况	
	int   			num_hold_stock_normal;			//现持有股票数目，包括融券的股票，融券的股票进行反向计算
	int   			num_hold_stock_rq;				//现持有股票数目，包括融券的股票，融券的股票进行反向计算
	ut_JY_record stock[MAX_JY_RECORD];
	int				stock_wait_head;
	int				stock_wait_tail;
	

	//收益情况
	float			zdhc;							//最大回撤
	float			nh_syl;							//年化收益率


	//交易统计
	int 			times_jy;						//交易次数，按照买卖一次算一次
	int				total_hold_days;				//总持有交易日数
	float			total_syl;

	//盈利交易统计
	int 			times_jy_1;						//交易次数，按照买卖一次算一次
	int				total_hold_days_1;				//总持有交易日数
	float			total_syl_1;
	

	//亏损交易统计
	int 			times_jy_0;						//交易次数，按照买卖一次算一次
	int				total_hold_days_0;				//总持有交易日数
	float			total_syl_0;



	int 			times_jy_k;					//交易次数，按照买卖一次算一次
	int				total_hold_days_k;			//总持有交易日数
	float 			total_syl_k;

	float			zj_syxl_duo;						//资金使用效率
	float			zj_syxl_kong;						//资金使用效率	
	int				moni_days;

	int  			times_sale;					//应当卖出次数
	int 			times_buy;						//应当买入次数
	int 			times_sale_fail;			
	int				times_buy_fail;
	int 			times_sale_succ_xjd;			//成功卖出次数
	int				times_buy_succ;			//成功买入次数	
	int 			times_sale_bcaus_p120;
	int				times_sale_bcaus_QD;
	
}ut_JY_info;


// stock for selected
typedef struct {
	int 	version;
	int 	type;
	char 	code[8];
	char 	name[16];	
	int		ok_jsm;
	int		ok_yc;
	int		ok_stoplong;
	int 	ok_blacklist;
	float 	qd;
	float 	deal_money;
	char 	hy_code[8];
	int 	ok_hy;

	int 	flag;
} ut_StockSelect;			


#endif
