package main

import (
	"exchange/config"
	"exchange/input"
	"exchange/order"
	"fmt"
	log "github.com/htgx/htcomm/logger"
)

func main() {

	err := config.LoadConfig("config.toml")
	if err != nil {
		fmt.Printf("加载配置文件失败：%v\n", err)
		return
	}

	log.MustInitLog(config.CfgProgram.LogCfg.LogName, config.CfgProgram.LogCfg.LogLevel,
		config.CfgProgram.LogCfg.LogPath, config.CfgProgram.LogCfg.StdoutEnableFlag)
	log.Info("init ok")

	var od order.Order
	od.OrderId = 10
	log.Infof("od = %v", od)

	input.InputOrderAndDual()
}
