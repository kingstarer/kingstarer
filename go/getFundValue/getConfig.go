package main
import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
)



type StGetFoundConfig struct {
	URL             string   //净值数据来源网址
	SleepSec        int      //获取每次净值后休眠时间 单位毫秒 这是为了防止给东财服务器太大压力
	JsonStartKey    string   //json开始标志
	JsonEndKey      string   //json结束标志
	FundCodeList    []string //基金代码列表
	CopyToClipboard bool     //是否把结果复制到剪贴板
	OutputFile      string   //净值结果输出到文件路径
}

var FundConfig = StGetFoundConfig{}

func GetConfig(confFile string) error {

	file, err := os.OpenFile(confFile, os.O_RDONLY, 0666)
	if err != nil {
		return errors.New(fmt.Sprintf("open file error %v", err))
	}
	defer file.Close()

	str, err := ioutil.ReadAll(file)
	if err != nil {
		return errors.New(fmt.Sprintf("read all error: %v", err))
	}

	if err = json.Unmarshal(str, &FundConfig); err != nil {
		return errors.New(fmt.Sprintf("Unmarshal error: %v", err))
	}

	return nil
}
