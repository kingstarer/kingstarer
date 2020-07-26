package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
)
/*

{
	"Data": {
		"LSJZList": [
			{
				"FSRQ": "2020-02-20",
				"DWJZ": "5.0409",
				"LJJZ": "5.9309",
				"SDATE": null,
				"ACTUALSYI": "",
				"NAVTYPE": "1",
				"JZZZL": "2.60",
				"SGZT": "限制大额申购",
				"SHZT": "开放赎回",
				"FHFCZ": "",
				"FHFCBZ": "",
				"DTYPE": null,
				"FHSP": ""
			}
		],
		"FundType": "002",
		"SYType": null,
		"isNewType": false,
		"Feature": "211"
	},
	"ErrCode": 0,
	"ErrMsg": null,
	"TotalCount": 14,
	"Expansion": null,
	"PageSize": 3
	00,
	"PageIndex": 1
}
*/

type JsonFundList struct {
	Data JsonData
	TotalCount int
	ErrCode int
	ErrMsg string
}

type JsonData struct {
	LSJZList []JsonFundInfo `json:"LSJZList"`//历史净值列表
	isNewType bool `json:"isNewType"`
	Feature string
}

type JsonFundInfo struct {
	FSRQ string //净值日期
	DWJZ string //单位净值
	LJJZ string //累计净值
	JZZZL string //净值增涨率
}

//用来存储从东方财富获取的基金数据
var FundInfoObject JsonFundList

func GetFundValue(fundCode, startDate, endDate string) (err error) {
	if len(startDate) != 8 || len(endDate) != 8 {
		return errors.New(fmt.Sprintf("开始结束日期必须是yyyymmdd格式"))
	}

	//拼装请求网址
	realStartDate := fmt.Sprintf("%s-%s-%s", startDate[0 : 4], startDate[4 : 6], startDate[6 : 8])
	realEndDate := fmt.Sprintf("%s-%s-%s", endDate[0 : 4], endDate[4 : 6], endDate[6 : 8])
	realUrl := fmt.Sprintf(FundConfig.URL, fundCode, realStartDate, realEndDate)

	//准备发送请求
	client := &http.Client{}
	reqest, err := http.NewRequest("GET", realUrl, nil)
	if err != nil {
		return err
	}

	//必须设置好http头 不然返回会失败
	reqest.Header.Add("Referer", "http://fundf10.eastmoney.com/")
	reqest.Header.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36")
	//获取出返回的报文
	fmt.Println("开始获取数据:", realUrl)
	resp, err := client.Do(reqest)
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
	}

	strBody, bodyLen := string(body), len(body)

	//取出返回报文里面的Json
	nJsonStartPos := strings.Index(strBody, FundConfig.JsonStartKey) + len(FundConfig.JsonStartKey)
	nJsonEndPos := strings.LastIndex(strBody, FundConfig.JsonEndKey)

	if nJsonStartPos >= bodyLen || nJsonEndPos <= 0 || nJsonStartPos >= nJsonEndPos {
		return errors.New(fmt.Sprintf("返回报文中截取Json失败 起始位置%d %d", nJsonStartPos, nJsonEndPos))
	}

	//解析json
	err = parseFundJson(body[nJsonStartPos : nJsonEndPos])
	if err != nil {
		return err
	}

	return nil
}


func parseFundJson(strJson []byte) (err error) {
	/*
	testStrJson := `{
		"Data": {
			"LSJZList": [
				{
					"FSRQ": "2020-02-20",
					"DWJZ": "5.0409",
					"LJJZ": "5.9309",
					"SDATE": null,
					"ACTUALSYI": "",
					"NAVTYPE": "1",
					"JZZZL": "2.60",
					"SGZT": "限制大额申购",
					"SHZT": "开放赎回",
					"FHFCZ": "",
					"FHFCBZ": "",
					"DTYPE": null,
					"FHSP": ""
				}
			],
			"FundType": "002",
			"SYType": null,
			"isNewType": false,
			"Feature": "211"
		},
		"ErrCode": 0,
		"ErrMsg": null,
		"TotalCount": 14,
		"Expansion": null,
		"PageSize": 300,
		"PageIndex": 1
	}`
	*/

	if err = json.Unmarshal([]byte(strJson), &FundInfoObject); err != nil {
		return errors.New(fmt.Sprintf("解析东财返回基金信息失败 error:%v;\n输入Json信息为:%s",
			err, strJson))
	}

	//fmt.Println("获取到的基金净值数据：", FundInfoObject)

	return
}

