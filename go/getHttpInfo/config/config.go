package config

import (
	"errors"
	"fmt"
	"github.com/BurntSushi/toml"
	"github.com/htgx/htcomm"
	"io/ioutil"
	"os"
)

type LogConf struct {
	LogPath  string
	LogLevel string
	LogName  string
	StdoutEnableFlag bool
}

type Api struct {
	AddrApi    string
	IpApi string
	WhiteApi string
}

type StGetHttpInfo struct {
	LogCfg     LogConf
	Api Api
}

var CfgGetHttpInfo *StGetHttpInfo

func LoadConfig(confFile string) error {
	CfgGetHttpInfo = &StGetHttpInfo{}
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

	if _, err := toml.Decode(htcomm.Bytes2strUnsafe(str), &CfgGetHttpInfo); err != nil {
		return errors.New(fmt.Sprintf("open file error %v", err))
	}

	/*
		//json
		if err = json.Unmarshal(str, CfgGetHttpInfo); err != nil {
			return errors.New(fmt.Sprintf("Unmarshal error: %v", err))
		}
	*/

	return nil
}
