rem �������������Ҫ���ô�����ڽű���ͷ��ӣ�set http_proxy=192.138.0.1:8080

rem �����ip�Ͷ˿���Ҫ�滻��ʵ�ʵĴ����������ַ�Ͷ˿�
rem curl  --compressed  "http://fund.eastmoney.com/Data/Fund_JJJZ_Data.aspx?t=1&lx=1&letter=&gsid=&text=&sort=bzdm,asc&page=1,29999&dt=1433429154383&atfc=&onlySale=0" > jz.js

Release\getFundNetValue.exe  "http://fund.eastmoney.com/Data/Fund_JJJZ_Data.aspx?t=1&lx=1&letter=&gsid=&text=&sort=bzdm,asc&page=1,29999&dt=1433429154383&atfc=&onlySale=0" 1  > getFundNetValue.log 2>&1 



