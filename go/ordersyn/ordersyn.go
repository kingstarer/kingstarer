package main

import (
	"fmt"
	log "github.com/htgx/htcomm/logger"
	"ordersyn/config"
	"ordersyn/input"
)

//20210417 hch 本程序主要实现功能是输入已提交订单和规划提交的订单信息
//输出具体修改指令
//https://www.cnblogs.com/kingstarer/
func main() {

	err := config.LoadConfig("config.toml")
	if err != nil {
		fmt.Printf("加载配置文件失败：%v\n", err)
		return
	}

	log.MustInitLog(config.CfgProgram.LogCfg.LogName, config.CfgProgram.LogCfg.LogLevel,
		config.CfgProgram.LogCfg.LogPath, config.CfgProgram.LogCfg.StdoutEnableFlag)
	log.Info("init ok")

	input.InputOrderAndDual()
}
