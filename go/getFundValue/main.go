package main

import (
	"fmt"
	"github.com/atotto/clipboard"
	"io/ioutil"
	"os"
	"strings"
	"time"
)



func main() {
	var startDate, endDate string

	//读取配置 获取基金列表和url信息
	err := GetConfig("setting.json")
	if err != nil {
		fmt.Println("配置错误: ", err)
		os.Exit(-2)
	}

	//每次获取基金净值间隔至少1秒
	if FundConfig.SleepSec <= 0 {
		FundConfig.SleepSec = 1
	}

	//如果命令行有输入参数，则不需要从键输入
	if len(os.Args) == 1 {
		fmt.Println("请输入起始时间(8位数字，例如20190201) 输入信息以空格分隔")
		n, err := fmt.Scan(&startDate, &endDate)
		if err != nil {
			fmt.Println("输入错误: ", err)
			fmt.Println("请检查输入信息是否完整，不同信息之间是否有空格")
			os.Exit(-1)
		}

		_ = n
		//fmt.Printf("输入%d个参数\n", n)
	} else if len(os.Args) == 3 {
		startDate, endDate = os.Args[1], os.Args[2]
	} else {
		fmt.Println("启动参数必须为两个，分别是开始日期和结束日期")
	}

	var strFundValue string; //基金净值

	//遍历基金净值进行处理
	for _, fundCodeOne := range(FundConfig.FundCodeList) {
		fmt.Println("输入信息", fundCodeOne, startDate, endDate)

		err = GetFundValue(fundCodeOne, startDate, endDate)
		if err != nil {
			fmt.Println("获取净值失败: ", err)
			os.Exit(-3)
		}

		if FundInfoObject.ErrCode != 0 {
			fmt.Println("东方财富返回信息异常:", FundInfoObject.ErrCode, FundInfoObject.ErrMsg)
			os.Exit(-4)
		}

		strTmp := fmt.Sprintf("基金代码:%s\n", fundCodeOne)
		fmt.Printf(strTmp)

		fmt.Println("净值日期", "单位净值", "累计净值", "净值增长率")
		for _, jzInfo := range(FundInfoObject.Data.LSJZList) {
			fmt.Println(jzInfo.FSRQ, jzInfo.DWJZ, jzInfo.LJJZ, jzInfo.JZZZL + "%")
			strTmp += strings.Join([]string{jzInfo.FSRQ, jzInfo.DWJZ, jzInfo.LJJZ, jzInfo.JZZZL + "%"}, " ")
			strTmp += "\n"
		}

		strFundValue += strTmp
		//暂停一段时间再继续 避免太频繁访问网站被封了ip
		time.Sleep(time.Duration(FundConfig.SleepSec) * time.Second)
	}

	err = saveFundListValue(strFundValue)
	if err != nil {
		fmt.Println("保存净值信息失败: ", err)
		os.Exit(-5)
	}
	//fmt.Println("url = ", FundConfig.URL)
	//fmt.Println("sleepMs = ", FundConfig.SleepSec)
	//fmt.Printf("startDate Count %d", len(startDate))
}

//测试函数
func getInput() (string, string) {
	return "20200201", "20200220"
}

//把净值信息保存起来
func saveFundListValue(str string) (err error) {

	// 复制内容到剪切板
	if FundConfig.CopyToClipboard {
		err = clipboard.WriteAll(str)
		if err != nil {
			return err
		}
	}

	// 保存到文件
	if FundConfig.OutputFile != "" {
		err = ioutil.WriteFile(FundConfig.OutputFile, [] byte(str),0755)
		if err != nil {
			return err
		}
	}

	return nil
}
