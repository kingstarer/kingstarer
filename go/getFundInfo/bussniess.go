
package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"getFundInfo/proxy"
	"strings"
)

//{"fundcode":"506002","name":"易方达科创板两年定开混合","jzrq":"2021-11-25","dwjz":"1.3224","gsz":"1.3244","gszzl":"0.15","gztime":"2021-11-26 15:00"}
type JsonFundInfo struct {
	fundcode string //基金代码
	name string //基金名称
	jzrq string //净值日期
	dwjz string //净值
	gszzl string //估值增涨率
	gztime string //估值时间
}

var FundParsed JsonFundInfo;

func parseFundJson(strJson string) (err error) {
	if err = json.Unmarshal([]byte(strJson), &FundParsed); err != nil {
		return errors.New(fmt.Sprintf("解析东财返回基金信息失败 error:%v;\n输入Json信息为:%s",
			err, strJson))
	}

	//fmt.Println("获取到的基金净值数据：", FundInfoObject)

	return
}


func GetFundValue(fundCode string) (err error) {
	body, err := proxy.HttpGetFund(fundCode)
	if err != nil {
		body = fmt.Sprintf("error:%v", err)
	} else {
		body = strings.TrimPrefix(body, "jsonpgz(");
		body = body[0 : len(body) - 2]

		//解析json
		err = parseFundJson(body)
		if err != nil {
			return err
		}

	}

	return nil
}
