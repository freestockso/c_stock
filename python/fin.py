import urllib
import urllib2

tdxstockfile = "c:/fin/list.txt"
destpath = "c:/fin/"

def down_stock_info(prepath, afterpath, localpath, localname):
    def downstock(stock):
        srcfile  = prepath + stock + afterpath
        destfile = localpath + stock + localname
        urllib.urlretrieve(srcfile, destfile)
        print("Download " + srcfile + " -> " + destfile)
        return
    return downstock
	
import time
def sleep(sec):
    time.sleep(sec)
    return

down_stock_profit   = down_stock_info("http://money.finance.sina.com.cn/corp/go.php/vDOWN_ProfitStatement/displaytype/4/stockid/", "/ctrl/all.phtml", destpath, "1")
down_stock_balance   = down_stock_info("http://money.finance.sina.com.cn/corp/go.php/vDOWN_BalanceSheet/displaytype/4/stockid/", "/ctrl/all.phtml", destpath, "2")

def download_tdxstock(filename):
    fp = open(filename, "r")
    textlist = fp.readlines()
    i = 1
    for line in textlist:
        scode = line[0:6]
        sfirst = scode[0:1]
		
        if ((sfirst == '0') or (sfirst == '3') or (sfirst == '6')):
            down_stock_profit(scode)
            #i = (i+1)%8
            sleep(i)
            down_stock_balance(scode)    
            #i = (i+1)%8
            sleep(i)
    return

def download_tdxstock_from_broken(filename, nextcode):
    fp = open(filename, "r")
    textlist = fp.readlines()
    i = 1
    keepgo = 0
    for line in textlist:
        scode = line[0:6]
        sfirst = scode[0:1]
        if (keepgo == 0):
            if (scode == nextcode):
                keepgo = 1
            else:
                continue
        if ((sfirst == '0') or (sfirst == '3') or (sfirst == '6')):
            down_stock_profit(scode)
            i = (i+1)%8
            sleep(i)
    return

download_tdxstock(tdxstockfile)
#download_tdxstock_from_broken(tdxstockfile, '000063')
