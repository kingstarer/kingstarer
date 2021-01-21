// getFundNetValue.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <algorithm>
using namespace std;

extern URL_TYPE getUrlType(const string& url);

//��ȡ��ֵ
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


	//ÿ���������ݴ�<tr id=
	size_t pos = strHtml.find(strStart, strHtml.find("������") + 1);

	bool endFlag = false;
	while (pos != string::npos && !endFlag)
	{
		size_t endpos = strHtml.find(strStart, pos + 1);

		//���һ������֮��û��<tr id=��������Ҫ���ҽ�����־
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
			//û�����ݵĽڵ㲻����\t �硰<node></node>��
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

					//�������������һ����վ����Ϊutf8
					if (urlType != eastmoney)
					{
						//utf8һ������ռ�����ֽ� ������Ŀǰ������ַ��������ַ����ֽ����轫���������ֽ�Ҳһ��������
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
		//��utf8����ת��gbk����
		if( (urlType != eastmoney) 
			&& !UTF8ToGBK(buffer, gbkBuffer, sizeof(gbkBuffer)) )
		{
			fprintf(stderr, "��ҳ����ת��ʧ�ܣ��뷢�ʼ�֪ͨ���߸��������hch1986@21cn.com\n");
			vecResult.clear();
			break;
		}

		if (urlType == eastmoney)
		{
			//Ϊ�˱��������ֵ��ʽ��ͬ������ȥ���������
			vecResult.push_back(stringReplace(buffer, "����ͼ\t", ""));
		}
		else
		{
			vecResult.push_back( stringReplace(gbkBuffer, "\t����\t", "\t") );
		}
	}


	return vecResult.size();
}


//��ȡ��������ֵ����
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

	//ÿ���������ݴ�<tr id=r
	size_t pos = strHtml.find(strStart, strHtml.find("������") + 1);

	bool endFlag = false;
	while (pos != string::npos && !endFlag)
	{
		size_t endpos = strHtml.find(strStart, pos + 1);

		//���һ������֮��û��<tr id=��������Ҫ���ҽ�����־
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
				//û�����ݵĽڵ㲻����\t �硰<node></node>��
				if (contentFlag && buffer[i - 1] != '\t')
				{
					buffer[i++] = '\t';
					//Ϊ�˱��ֲ�ͬ��վ����ֵ��ʽ��ͬ����������������
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
				//�������������һ����վ����Ϊutf8
				if (urlType != eastmoney)
				{
					//utf8һ������ռ�����ֽ� ������Ŀǰ������ַ��������ַ����ֽ����轫���������ֽ�Ҳһ��������
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
		//��utf8����ת��gbk����
		if( (urlType != eastmoney) 
			&& !UTF8ToGBK(buffer, gbkBuffer, sizeof(gbkBuffer)) )
		{
			fprintf(stderr, "��ҳ����ת��ʧ�ܣ��뷢�ʼ�֪ͨ���߸��������hch1986@21cn.com\n");
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
			vecResult.push_back(stringReplace(stringReplace(stringReplace(gbkBuffer, "-\t-", "---\t---"), "\n", ""), "\t����\t", "\t"));
		}
		else 
		{
			vecResult.push_back(gbkBuffer);
		}

	}


	return vecResult.size();
}




extern bool setClipboard(const string& str);

//�ɵ�ʵ�֣�����û����
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
		perror("�޷�����ʱ�ļ�����ȷ���ļ����Ƿ�ֻ����");
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
		fprintf(stderr, "��ȡ����ֵʧ�ܣ������Ƿ��������������⡣\n");
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
		fprintf(stderr, "��ȡ����ֵʧ�ܣ��뷢�ʼ�֪ͨ���߸��������hch1986@21cn.com\n");
		return -3;
	}

	fp = fopen("fund.txt", "w");
	if (fp == NULL)
	{
		perror("�޷�д���ļ�����ȷ���ļ����Ƿ�ֻ����");
		return -4;
	}

	strHtml = "";
	for (size_t i = 0; i < vecResult.size(); i++)
	{
		strHtml += vecResult[i] + "\r\n";
	}

	fwrite(strHtml.c_str(), 1, strHtml.size(), fp);
	fclose(fp);

	//�ڶ�������Ϊ1ʱ��д�������
	if (argc > 2 && argv[2][0] == '1')
	{
		if (!setClipboard(strHtml))
		{
			perror("д�������ʧ��");
			system("run fund.txt");
			return -5;
		}
	}

	return 0;
}

