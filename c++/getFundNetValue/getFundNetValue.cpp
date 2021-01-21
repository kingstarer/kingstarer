// getFundNetValue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include "tool.h"
using namespace std;

const char *g_urlName[] = {"unkowurl", "eastmoney", "howbuy", "fund123", "jjmmw", "qq"};

URL_TYPE getUrlType(const string& url)
{
	string urlLower = url;
	transform(urlLower.begin(), urlLower.end(), urlLower.begin(), ::tolower);
	if (urlLower.find("eastmoney.com") != string::npos || url.find("1234567.com") != string::npos )
	{
		return eastmoney;
	}
	else if (urlLower.find("howbuy.com") != string::npos)
	{
		return howbuy;
	}
	else if (urlLower.find("fund123.cn") != string::npos)
	{
		return fund123;
	}
	else if (urlLower.find("jjmmw.com") != string::npos)
	{
		return jjmmw;
	}
	else if (urlLower.find("qq.com") != string::npos)
	{
		return qq;
	}
	else
	{
		return unkowurl;
	}
}


//获取基金网净值数据
size_t getFoundNetValueGz(const string& strHtml, vector<string>& vecResult, URL_TYPE urlType, /*const */map<string, string>& mpConfig)
{
	string strNetValue;
	assert(strHtml.size() != 0);

	vecResult.clear();

	string strStart = mpConfig["allNetValueBegin"], strEnd = mpConfig["allNetValueEnd"];
	const size_t begPos = strHtml.find(strStart);
	const size_t endPos = strHtml.find(strEnd);
	char bufferResult[2046];
	char gbkBuffer[1024]; 

	if ((begPos == string::npos))
	{
		fprintf(stderr, "find strStart err\n");
		return 0;
	}

	strNetValue = strHtml.substr(begPos + strStart.size());

	vector<string> vecFunds;
	vector<string> vecCells;
	if (0 == splitToVector(strNetValue.c_str(), mpConfig["netValueSplit"].c_str(), vecFunds))
	{
		fprintf(stderr, "split funds err\n");
		return 0;
	}

	size_t todayNetValuePos = atoi(mpConfig["todayNetValuePos"].c_str());
	size_t yesterdayNetValuePos = atoi(mpConfig["yesterdayNetValuePos"].c_str());
	size_t increasePos = atoi(mpConfig["increasePos"].c_str());
	size_t fundSnPos = atoi(mpConfig["fundSnPos"].c_str());
	size_t fundNamePos = atoi(mpConfig["fundNamePos"].c_str());
	size_t maxPos = max(todayNetValuePos, max(yesterdayNetValuePos, max(increasePos, max(fundSnPos, fundNamePos))));
	bool bUtf8Flag = (mpConfig["utf2gbk"] == "1");
	bool bRemoveHeadTailFlag = (mpConfig["trimChar"].size() != 0);
	const char *fundName;

	for (size_t i = 0; i < vecFunds.size(); i++)
	{
		if (maxPos > splitToVector(vecFunds[i].c_str(), mpConfig["cellSplit"].c_str(), vecCells))
		{
			fprintf(stderr, "split cells err %d\n", (int)i);
			continue;
		}

		if (bRemoveHeadTailFlag)
		{
			for (size_t j = 0; j < vecCells.size(); j++)
			{
				removeCharHeadTail(vecCells[j], mpConfig["trimChar"][0]);
			}
		}

		if (mpConfig["leftTrimNameChar"].size() != 0)
		{
			string& fundName = vecCells[fundNamePos - 1];
			size_t pos = fundName.find(mpConfig["leftTrimNameChar"]);
			if (pos != string::npos)
			{
				fundName = fundName.substr(pos + mpConfig["leftTrimNameChar"].size());
			}
		}

		if (vecCells[increasePos - 1].size() == 0)
		{
			vecCells[increasePos - 1] = "0";
		}

		// 		for (size_t j = 0; j < vecCells.size(); j++)
		// 		{
		// 			fprintf(stderr, "%s ", vecCells[j].c_str());
		// 		}
		// 		fprintf(stderr, "\n");
		fundName = vecCells[fundNamePos - 1].c_str();
		if (bUtf8Flag)
		{
			UTF8ToGBK(fundName, gbkBuffer, sizeof(gbkBuffer));
			fundName = gbkBuffer;
		}

		//对比	序号	基金代码	基金名称(华安黄金易ETF联接A	基金吧	档案) 估算值 估算增长率  昨日净值
		sprintf(bufferResult, "\t%d\t%s\t%s\t---\t---\t%s\t%s\t%s",
			(int) i, 
			getValueMh(vecCells[fundSnPos - 1]).c_str(), 
			getValueMh(fundName).c_str(),
			getValueMh(vecCells[todayNetValuePos - 1]).c_str(), 
			getValueMh(vecCells[increasePos - 1]).c_str(), 
			getValueMh(vecCells[yesterdayNetValuePos - 1]).c_str());
		vecResult.push_back(bufferResult);
	}

	return vecResult.size();
}





//获取基金网净值数据
size_t getFoundNetValue(const string& strHtml, vector<string>& vecResult, URL_TYPE urlType, /*const */map<string, string>& mpConfig, map<string, string>& mpConfigQdii)
{
	string strNetValue;
	assert(strHtml.size() != 0);

	vecResult.clear();

	int changeQdiiFundFlag = atoi(mpConfigQdii["changeQdiiFundFlag"].c_str());
	fprintf(stderr, "changeQdiiFundFlag = %s\n", mpConfigQdii["changeQdiiFundFlag"].c_str());
	set<string> qdiiSet;
	//如果配置了qdii基金使用昨日净值代替今日净值
	if (changeQdiiFundFlag == 1)
	{
		//需要先获取qdii基金列表 后面可以考虑拆成函数
		string html;
		int tmpRet = getHtml(mpConfigQdii["url"], html);
		if (tmpRet != 0) {
			fprintf(stderr, "注意：获取qdii基金列表失败，将无视changeQdiiFundFlag配置");
			changeQdiiFundFlag = 0;
		}
		else 
		{
			html = substrByKeyWord(html, mpConfigQdii["dataBegin"], mpConfigQdii["dataEnd"]);
			//fprintf(stderr, "html = %s\n", html.c_str());

			vector<string> vecTr;
			size_t totalQdiiNum = splitToVector(html.c_str(), "<tr id=\"tr", vecTr);
			fprintf(stderr, "totalQdiiNum = %d\n", totalQdiiNum);
			for (size_t i = 0; i < totalQdiiNum; ++i)
			{
				string qdiiSn = substrByKeyWord(vecTr[i], "<td class=\"bzdm\">", "</td>");
				qdiiSet.insert(qdiiSn);
				//fprintf(stderr, "qdiiSn = %s\n", qdiiSn.c_str());
			}
		}
	}

	string strStart = mpConfig["allNetValueBegin"], strEnd = mpConfig["allNetValueEnd"];
	const size_t begPos = strHtml.find(strStart);
	const size_t endPos = strHtml.find(strEnd);
	char bufferResult[2046];
	char gbkBuffer[1024]; 

	if ((begPos == string::npos))
	{
		fprintf(stderr, "find strStart err\n");
		return 0;
	}

	strNetValue = strHtml.substr(begPos + strStart.size());

	vector<string> vecFunds;
	vector<string> vecCells;
	if (0 == splitToVector(strNetValue.c_str(), mpConfig["netValueSplit"].c_str(), vecFunds))
	{
		fprintf(stderr, "split funds err\n");
		return 0;
	}

	size_t todayNetValuePos = atoi(mpConfig["todayNetValuePos"].c_str());
	size_t yesterdayNetValuePos = atoi(mpConfig["yesterdayNetValuePos"].c_str());
	size_t increasePos = atoi(mpConfig["increasePos"].c_str());
	size_t fundSnPos = atoi(mpConfig["fundSnPos"].c_str());
	size_t fundNamePos = atoi(mpConfig["fundNamePos"].c_str());
	size_t maxPos = max(todayNetValuePos, max(yesterdayNetValuePos, max(increasePos, max(fundSnPos, fundNamePos))));
	bool bUtf8Flag = (mpConfig["utf2gbk"] == "1");
	bool bRemoveHeadTailFlag = (mpConfig["trimChar"].size() != 0);
	const char *fundName;

	for (size_t i = 0; i < vecFunds.size(); i++)
	{
		if (maxPos > splitToVector(vecFunds[i].c_str(), mpConfig["cellSplit"].c_str(), vecCells))
		{
			fprintf(stderr, "split cells err %d\n", (int)i);
			continue;
		}

		if (bRemoveHeadTailFlag)
		{
			for (size_t j = 0; j < vecCells.size(); j++)
			{
				removeCharHeadTail(vecCells[j], mpConfig["trimChar"][0]);
			}
		}

		if (mpConfig["leftTrimNameChar"].size() != 0)
		{
			string& fundName = vecCells[fundNamePos - 1];
			size_t pos = fundName.find(mpConfig["leftTrimNameChar"]);
			if (pos != string::npos)
			{
				fundName = fundName.substr(pos + mpConfig["leftTrimNameChar"].size());
			}
		}

		if (vecCells[increasePos - 1].size() == 0)
		{
			vecCells[increasePos - 1] = "0";
		}

		if (mpConfig["increaseIsPercent"] != "1")
		{
			//char valueBuf[32];
			//sprintf(valueBuf, "%.2f%%", atof(vecCells[increasePos - 1].c_str()) * 100);
			//vecCells[increasePos - 1] = valueBuf;
			vecCells[increasePos - 1] = vecCells[increasePos - 1] + "%";
		}

// 		for (size_t j = 0; j < vecCells.size(); j++)
// 		{
// 			fprintf(stderr, "%s ", vecCells[j].c_str());
// 		}
// 		fprintf(stderr, "\n");
		fundName = vecCells[fundNamePos - 1].c_str();
		if (bUtf8Flag)
		{
			UTF8ToGBK(fundName, gbkBuffer, sizeof(gbkBuffer));
			fundName = gbkBuffer;
		}

		//如果基金是qdii基金 则需要用昨天基金代替今天净值
		if (changeQdiiFundFlag && qdiiSet.find(vecCells[fundSnPos - 1]) != qdiiSet.end()) 
		{
			vecCells[todayNetValuePos - 1] = vecCells[yesterdayNetValuePos - 1];
			vecCells[yesterdayNetValuePos - 1] = "---";
		}

		sprintf(bufferResult, "\t%d	%s	%s	---	---	%s	今日累计净值	%s	昨日累计净值	净值增长值	%s",
			(int) i, vecCells[fundSnPos - 1].c_str(), fundName,
			vecCells[todayNetValuePos - 1].c_str(), vecCells[yesterdayNetValuePos - 1].c_str(), vecCells[increasePos - 1].c_str());
		vecResult.push_back(bufferResult);
	}

	return vecResult.size();
}




/*
读取基金净值获取方法的配置，把结果存到map
*/
void readConfig(const string& url, bool bIsGzFlag, OUT map<string, string>& mpConfig) 
{

	char nbuffer[1024];
	char *keyWord[] = {"url", "utf2gbk", "allNetValueBegin", "allNetValueEnd", "netValueSplit",
		"cellSplit", "todayNetValuePos", "yesterdayNetValuePos", "increasePos", "increaseIsPercent", 
		"fundSnPos", "fundNamePos", "trimChar", "leftTrimNameChar", "referer", "user-agent"};
	string strUrlName = g_urlName[getUrlType(url)];
	if (bIsGzFlag)
	{
		strUrlName += "_gz";
	}

	for (size_t i = 0; i < sizeof(keyWord) / sizeof(keyWord[0]); i++)
	{
		memset(nbuffer, 0, sizeof(nbuffer));
		GetPrivateProfileString(strUrlName.c_str(), 
			keyWord[i], 
			"", 
			nbuffer, 
			sizeof(nbuffer), 
			"./found.ini");
		fprintf(stderr, "[%s] = [%s]\n", keyWord[i], nbuffer);
		mpConfig[keyWord[i]] = nbuffer;
	}
}






//获取场内基金净值
size_t getCnFoundNetValue(vector<string>& vecResult, map<string, string>& mpConfigCn)
{
	string strNetValue;
	int tmpRet = getHtml(mpConfigCn["url"], strNetValue);
	if (tmpRet != 0) {
		fprintf(stderr, "注意：获取场内基金列表失败");
		return 0;
	}
	

	strNetValue = substrByKeyWord(strNetValue, mpConfigCn["dataBegin"], mpConfigCn["dataEnd"]);

	vector<string> vecFunds;
	vector<string> vecCells;
	if (0 == splitToVector(strNetValue.c_str(), "<tr bgcolor=\"#F5FFFF\"", vecFunds))
	{
		fprintf(stderr, "split funds err\n");
		return 0;
	}

	size_t nSn = vecResult.size();
	//第一行是标题栏
	for (size_t i = 1; i < vecFunds.size(); i++, nSn++)
	{
		splitToVector(vecFunds[i].c_str(), "</td><td", vecCells);

		if (vecCells.size() <= 10) {
			continue;
		}

		string fundCode = substrByKeyWord(vecCells[3].c_str(), ">", "no end");
		string fundName = substrByKeyWord(vecCells[4].c_str(), "\">", "<");
		//今天的净值
		string fundValueToday = substrByKeyWord(vecCells[6].c_str(), "ping'>", "<");;
		//昨天的净值
		string fundValueYesterday = substrByKeyWord(vecCells[8].c_str(), "ping'>", "<");
		//增涨率
		string increase = substrByKeyWord(vecCells[11].c_str(), "'>", "<");;

		char bufferResult[2046];
		sprintf(bufferResult, "\t%d	%s	%s	---	---	%s	今日累计净值	%s	昨日累计净值	净值增长值	%s",
			(int) nSn, fundCode.c_str(),fundName.c_str(),
			fundValueToday.c_str(), fundValueYesterday.c_str(), increase.c_str());
		vecResult.push_back(bufferResult);
	}

	return vecResult.size();
}


/*
读取qdii基金净值获取方法的配置，把结果存到map
*/
void readConfigQdii(OUT map<string, string>& mpConfig) 
{
	char nbuffer[1024];
	char *keyWord[] = {"url", "changeQdiiFundFlag", "dataBegin", "dataEnd"};
	string strUrlName = "qdii";

	for (size_t i = 0; i < sizeof(keyWord) / sizeof(keyWord[0]); i++)
	{
		memset(nbuffer, 0, sizeof(nbuffer));
		GetPrivateProfileString(strUrlName.c_str(), 
			keyWord[i], 
			"", 
			nbuffer, 
			sizeof(nbuffer), 
			"./found.ini");
		fprintf(stderr, "[%s] = [%s]\n", keyWord[i], nbuffer);
		mpConfig[keyWord[i]] = nbuffer;
	}
}



/*
读取场内基金净值获取方法的配置，把结果存到map
*/
void readConfigCn(OUT map<string, string>& mpConfig) 
{

	char nbuffer[1024];
	char *keyWord[] = {"url", "getCnFundFlag", "dataBegin", "dataEnd"};
	string strUrlName = "cn";

	for (size_t i = 0; i < sizeof(keyWord) / sizeof(keyWord[0]); i++)
	{
		memset(nbuffer, 0, sizeof(nbuffer));
		GetPrivateProfileString(strUrlName.c_str(), 
			keyWord[i], 
			"", 
			nbuffer, 
			sizeof(nbuffer), 
			"./found.ini");
		fprintf(stderr, "[%s] = [%s]\n", keyWord[i], nbuffer);
		mpConfig[keyWord[i]] = nbuffer;
	}
}


int getFundHtml(const string& url, IN map<string, string> mpConfig, OUT string& strHtml) 
{
	string strCmd = "curl --compressed \"";
	strCmd += url;
	strCmd += '"';
	if (mpConfig["referer"] != "")
	{
		strCmd += " --referer \"" + mpConfig["referer"] + '"';
	}

	if (mpConfig["user-agent"] != "")
	{
		strCmd += " --user-agent \"" + mpConfig["user-agent"] + '"';
	}


	strCmd += " -o fund.html";
	system(strCmd.c_str());

	FILE *fp = fopen("fund.html", "r");
	if (fp == NULL)
	{
		perror("无法打开临时文件，请确认文件夹是否只读。");
		return -1;
	}

	char buffer[4097];
	size_t nRead; 
	while ((nRead = fread(buffer, 1, sizeof(buffer) - 1, fp)) != 0)
	{
		buffer[nRead] = '\0';
		strHtml += buffer;
	}
	fclose(fp); 

	if (strHtml == "")
	{
		fprintf(stderr, "获取基金净值失败，请检查是否网络连接有问题。\n");
		return -2;
	}

	return 0;
}





int _tmain(int argc, _TCHAR* argv[])
{
	bool bIsGzFlag = false; //是否打算计算估值
	string url;

	if (argc > 1)
	{
		url = argv[1];

		if (argc >= 4)
		{
			bIsGzFlag = true;
		}
	}
	else
	{
		url = "http://stock.finance.qq.com/fund/jzzx/kfs.js?0.7591561504062185";
	}

	map<string, string> mpConfig;
	//20210117 hch 增加qdii和场内基金特殊处理功能
	map<string, string> mpConfigQdii; // qdii基金配置
	map<string, string> mpConfigCn; // 场内基金配置

	fprintf(stderr, "读取配置\n");
	readConfig(url, bIsGzFlag, mpConfig);
	readConfigQdii(mpConfigQdii);
	readConfigCn(mpConfigCn);

	string strHtml;
	fprintf(stderr, "读取基金净值\n");
	int nRet = getFundHtml(url, mpConfig, strHtml);
	if (nRet != 0) 
	{
		// 读取基金净值网页失败则退出
		fprintf(stderr, "读取基金净值失败\n");
		return nRet;
	}

	fprintf(stderr, "读取基金净值成功\n");
	vector<string> vecResult;
	size_t fundCnt = 0;
	if (!bIsGzFlag)
	{
		fprintf(stderr, "getFoundNetValue\n");
		fundCnt = getFoundNetValue(strHtml, vecResult, getUrlType(url), mpConfig, mpConfigQdii);
		getCnFoundNetValue(vecResult, mpConfigCn);
	}
	else
	{
		fprintf(stderr, "getFoundNetValueGz\n");
		fundCnt = getFoundNetValueGz(strHtml, vecResult, getUrlType(url), mpConfig);
	}

	if (fundCnt == 0)
	{
		fprintf(stderr, "获取基金净值失败，请发邮件通知作者更新软件：hch1986@21cn.com\n");
		return -3;
	}

	FILE *fp = fopen("fund.txt", "w");
	if (fp == NULL)
	{
		perror("无法写入文件，请确认文件夹是否只读。");
		return -4;
	}

	strHtml = "";
	for (size_t i = 0; i < vecResult.size(); i++)
	{
		strHtml += vecResult[i] + "\n";
	}

	fwrite(strHtml.c_str(), 1, strHtml.size(), fp);
	fclose(fp);

	//第二个参数为1时才写入剪贴板
	if (argc > 2 && argv[2][0] == '1')
	{
		if (!setClipboard(strHtml))
		{
			perror("写入剪贴板失败，请手动复制");
			system("run fund.txt");
			return -5;
		}
	}

	return 0;
}

