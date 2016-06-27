#-*- coding: UTF-8 -*- 

import tushare as ts

df  = ts.get_stock_basics()
del df['area']	
del df['outstanding']
del df['totalAssets']	
del df['liquidAssets']	
del df['fixedAssets']	
del df['reserved']	
del df['reservedPerShare']	
del df['esp']	
del df['bvps']	
del df['timeToMarket']	
del df['name']	
del df['industry']	
del df['pe']	
del df['pb']	

df.to_csv('..\\..\\data\\gb.txt', sep=' ', header=False)