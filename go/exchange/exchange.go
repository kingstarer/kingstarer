package main

import (
	"exchange/config"
	"exchange/input"
	"exchange/order"
	"fmt"
	log "github.com/htgx/htcomm/logger"
)

//20210410 hch 本程序主要实现功能是输入卖卖订单并进行撮合
//输出最后未成交的订单分布情况
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

	var od order.Order
	od.OrderId = 10
	log.Infof("od = %v", od)

	input.InputOrderAndDual()
}
