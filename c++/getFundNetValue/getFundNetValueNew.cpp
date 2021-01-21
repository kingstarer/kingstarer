// getFundNetValue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <algorithm>
using namespace std;

extern URL_TYPE getUrlType(const string& url);

//获取估值
size_t getFoundNetValueGzNew(const string& strHtml, vector<string>& vecResult, URL_TYPE urlType)
{
	assert(strHtml.size() != 0);

	vecResult.clear();

	string strStart, strEnd;
	if (urlType == eastmoney || urlType == unkowurl)
	{
		strStart = "<input type=\"checkbox\"";
		strEnd = "</table>";
	}
	else if (urlType == fund123)
	{
		strStart = "<input type=checkbox";
		strEnd = "</table>";
	}
	else
	{
		return 0;
	}


	//每个基金数据从<tr id=
	size_t pos = strHtml.find(strStart, strHtml.find("增长率") + 1);

	bool endFlag = false;
	while (pos != string::npos && !endFlag)
	{
		size_t endpos = strHtml.find(strStart, pos + 1);

		//最后一个基金之后没有<tr id=，所以需要另找结束标志
		if (endpos == string::npos)
		{
			endFlag = true;

			endpos = strHtml.find(strEnd, pos + 1);
			if (endpos == string::npos)
			{
				break;
			}
		}

		char buffer[4096];
		size_t i = 0; 
		buffer[i++] = '\t';

		bool contentFlag = false;
		int nCnt = 0;
		for (i = 1; pos < endpos; pos++)
		{
			//没有内容的节点不增加\t 如“<node></node>”
			if (strHtml[pos] == '<')
			{
				if (contentFlag && buffer[i - 1] != '\t')
				{
					buffer[i++] = '\t';
					nCnt++;
				}
				else if (urlType == fund123 && nCnt == 4)
				{
					nCnt++;
				}

				contentFlag = false;
			}
			else if (strHtml[pos] == '>')
			{
				contentFlag = true;
			}
			else
			{
				if (!contentFlag) continue;

				if (strHtml[pos] == '\r' || strHtml[pos] == '\n')
				{
					continue;
				}

				if (urlType != fund123 || nCnt != 4)
				{
					buffer[i++] = strHtml[pos];

					//除了天天基金网一般网站编码为utf8
					if (urlType != eastmoney)
					{
						//utf8一个汉字占三个字节 若发现目前处理的字符是中文字符首字节则需将后面两个字节也一并处理了
						if (strHtml[pos] & (1 << 7))
						{
							buffer[i++] = strHtml[++pos];
							buffer[i++] = strHtml[++pos];
						}
					}
				}
			}
		}

		buffer[i] = '\0';
		char gbkBuffer[4096];
		//将utf8编码转成gbk编码
		if( (urlType != eastmoney) 
			&& !UTF8ToGBK(buffer, gbkBuffer, sizeof(gbkBuffer)) )
		{
			fprintf(stderr, "网页编码转换失败，请发邮件通知作者更新软件：hch1986@21cn.com\n");
			vecResult.clear();
			break;
		}

		if (urlType == eastmoney)
		{
			//为了保持与基金净值格式相同，特意去掉这段文字
			vecResult.push_back(stringReplace(buffer, "估算图\t", ""));
		}
		else
		{
			vecResult.push_back( stringReplace(gbkBuffer, "\t购买\t", "\t") );
		}
	}


	return vecResult.size();
}


//获取基金网净值数据
size_t getFoundNetValueNew(const string& strHtml, vector<string>& vecResult, URL_TYPE urlType)
{
	assert(strHtml.size() != 0);

	vecResult.clear();

	string strStart, strEnd;
	if (urlType == howbuy)
	{
		strStart = "onclick=\"move(this);\" value=";
		strEnd = "<a href=\"javascript:void(0)\"";
	}
	else if (urlType == eastmoney || urlType == unkowurl || urlType == jjmmw)
	{
		strStart = "<tr id=";
		strEnd = "</table>";
	}
	else if (urlType == fund123)
	{
		strStart = "<input type=checkbox";
		strEnd = "</table>";
	}

	//每个基金数据从<tr id=r
	size_t pos = strHtml.find(strStart, strHtml.find("增长率") + 1);

	bool endFlag = false;
	while (pos != string::npos && !endFlag)
	{
		size_t endpos = strHtml.find(strStart, pos + 1);

		//最后一个基金之后没有<tr id=，所以需要另找结束标志
		if (endpos == string::npos)
		{
			endpos = strHtml.find(strEnd, pos + 1);
			if (endpos == string::npos)
			{
				break;
			}
			endFlag = true;
		}

		char buffer[4096];
		size_t i = 0; 
		buffer[i++] = '\t';

		bool contentFlag = false;
		int nCnt = 0;
		for (i = 1; pos < endpos; pos++)
		{
			if (strHtml[pos] == '<')
			{
				//没有内容的节点不增加\t 如“<node></node>”
				if (contentFlag && buffer[i - 1] != '\t')
				{
					buffer[i++] = '\t';
					//为了保持不同网站基金净值格式相同，特意加上这段文字
					nCnt++;
					if (urlType != eastmoney)
					{
						if ( (nCnt == 4 && urlType == fund123) || (nCnt == 3 && urlType == howbuy)
							|| (nCnt == 3 && urlType == jjmmw))
						{
							buffer[i++] = '*';
							buffer[i++] = '\t';
							buffer[i++] = '*';
							buffer[i++] = '\t';
						}
					}
				}

				contentFlag = false;
			}
			else if (strHtml[pos] == '>')
			{
				contentFlag = true;
			}
			else
			{
				if (!contentFlag) continue;

				if (strHtml[pos] == '\r' || strHtml[pos] == '\n')
				{
					continue;
				}


				buffer[i++] = strHtml[pos];
				//除了天天基金网一般网站编码为utf8
				if (urlType != eastmoney)
				{
					//utf8一个汉字占三个字节 若发现目前处理的字符是中文字符首字节则需将后面两个字节也一并处理了
					if (strHtml[pos] & (1 << 7))
					{
						buffer[i++] = strHtml[++pos];
						buffer[i++] = strHtml[++pos];
					}
				}

			}
		}

		buffer[i] = '\0';
		char gbkBuffer[4096];
		//将utf8编码转成gbk编码
		if( (urlType != eastmoney) 
			&& !UTF8ToGBK(buffer, gbkBuffer, sizeof(gbkBuffer)) )
		{
			fprintf(stderr, "网页编码转换失败，请发邮件通知作者更新软件：hch1986@21cn.com\n");
			vecResult.clear();
			break;
		}

		if (urlType == eastmoney)
		{
			vecResult.push_back(buffer);
		}
		else if (urlType == jjmmw || urlType == howbuy)
		{
			vecResult.push_back(stringReplace(stringReplace(gbkBuffer, "--\t--", "---\t---"), "\n", ""));
		}
		else if (urlType == fund123)
		{
			vecResult.push_back(stringReplace(stringReplace(stringReplace(gbkBuffer, "-\t-", "---\t---"), "\n", ""), "\t购买\t", "\t"));
		}
		else 
		{
			vecResult.push_back(gbkBuffer);
		}

	}


	return vecResult.size();
}




extern bool setClipboard(const string& str);

//旧的实现，现在没用了
int _tmainNew(int argc, _TCHAR* argv[])
{
	string url;
	if (argc > 1)
	{
		string strCmd = "curl --compressed \"";
		strCmd += argv[1];
		strCmd += "\" > fund.html";
		url = argv[1];
		system(strCmd.c_str());
	}
	else
	{
		url = "http://fund.eastmoney.com/fund.html ";
		system("curl --compressed http://fund.eastmoney.com/fund.html > fund.html");
	}

	FILE *fp = fopen("fund.html", "r");
	if (fp == NULL)
	{
		perror("无法打开临时文件，请确认文件夹是否只读。");
		return -1;
	}

	string strHtml;
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

	vector<string> vecResult;
	size_t fundCnt = 0;
	if (argc < 4)
	{
		fundCnt = getFoundNetValueNew(strHtml, vecResult, getUrlType(url));
	}
	else
	{
		fundCnt = getFoundNetValueGzNew(strHtml, vecResult, getUrlType(url));
	}

	if (fundCnt == 0)
	{
		fprintf(stderr, "获取基金净值失败，请发邮件通知作者更新软件：hch1986@21cn.com\n");
		return -3;
	}

	fp = fopen("fund.txt", "w");
	if (fp == NULL)
	{
		perror("无法写入文件，请确认文件夹是否只读。");
		return -4;
	}

	strHtml = "";
	for (size_t i = 0; i < vecResult.size(); i++)
	{
		strHtml += vecResult[i] + "\r\n";
	}

	fwrite(strHtml.c_str(), 1, strHtml.size(), fp);
	fclose(fp);

	//第二个参数为1时才写入剪贴板
	if (argc > 2 && argv[2][0] == '1')
	{
		if (!setClipboard(strHtml))
		{
			perror("写入剪贴板失败");
			system("run fund.txt");
			return -5;
		}
	}

	return 0;
}

