package config

import (
	"bytes"
	"errors"
	"fmt"
	"github.com/BurntSushi/toml"
	"github.com/htgx/htcomm"
	"io/ioutil"
	"os"
	"strings"
)

type LogConf struct {
	LogPath  string
	LogLevel string
	LogName  string
	StdoutEnableFlag bool
}

type Api struct {
	//获取代理服务器ip的网址
	AddrApi    string
	//获取当前机器外网ip的网址
	IpApi string
	//添加代表服务器白名单的网址
	WhiteApi string
	//互联网文档
	ArchiveApi string
}

type StCfgProgram struct {
	LogCfg     LogConf
	Api Api
}

var CfgProgram *StCfgProgram

func LoadConfig(confFile string) error {
	CfgProgram = &StCfgProgram{}
	file, err := os.OpenFile(confFile, os.O_RDONLY, 0666)
	if err != nil {
		return errors.New(fmt.Sprintf("open file error %v", err))
	}
	defer file.Close()

	str, err := ioutil.ReadAll(file)
	if err != nil {
		return errors.New(fmt.Sprintf("read all error: %v", err))
	}

	//替换配置文件里面的${HTGX_HOME}变量 替换失败则panic
	str = htcomm.MustChangeEnvBytesToBytes(str)

	if _, err := toml.Decode(htcomm.Bytes2strUnsafe(str), &CfgProgram); err != nil {
		return errors.New(fmt.Sprintf("open file error %v", err))
	}

	/*
		//json
		if err = json.Unmarshal(str, CfgProgram); err != nil {
			return errors.New(fmt.Sprintf("Unmarshal error: %v", err))
		}
	*/

	return nil
}


//获取配置
//指定的路径 path
//返回以行为单位的string类型配置数组
func ReadConfig(path string) ([]string, error) {
	dat, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	//删除尾部换行
	dat = bytes.Trim(dat, "\r\n")
	return strings.Split(string(dat), "\r\n"), nil
}

