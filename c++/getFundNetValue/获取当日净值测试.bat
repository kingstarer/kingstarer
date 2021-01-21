rem 如果您的网络需要配置代理可在脚本开头添加：set http_proxy=192.138.0.1:8080

rem 上面的ip和端口需要替换成实际的代理服务器地址和端口
rem curl  --compressed  "http://fund.eastmoney.com/Data/Fund_JJJZ_Data.aspx?t=1&lx=1&letter=&gsid=&text=&sort=bzdm,asc&page=1,29999&dt=1433429154383&atfc=&onlySale=0" > jz.js

Release\getFundNetValue.exe  "http://fund.eastmoney.com/Data/Fund_JJJZ_Data.aspx?t=1&lx=1&letter=&gsid=&text=&sort=bzdm,asc&page=1,29999&dt=1433429154383&atfc=&onlySale=0" 1  > getFundNetValue.log 2>&1 



