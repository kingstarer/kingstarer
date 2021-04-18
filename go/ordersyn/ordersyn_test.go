package main

import (
	"ordersyn/config"
	"ordersyn/input"
	"fmt"
	log "github.com/htgx/htcomm/logger"
	"ordersyn/order_current"
	"ordersyn/order_design"
	"strings"
	"testing"
)

func TestAll(t *testing.T) {

	err := config.LoadConfig("config.toml")
	if err != nil {
		fmt.Printf("加载配置文件失败：%v\n", err)
		return
	}

	log.MustInitLog(config.CfgProgram.LogCfg.LogName, config.CfgProgram.LogCfg.LogLevel,
		config.CfgProgram.LogCfg.LogPath, config.CfgProgram.LogCfg.StdoutEnableFlag)
	log.Info("init ok")

	var od order_current.Order
	od.OrderId = 10
	log.Infof("od = %v", od)

	var orderStrCurrentArray = [...]string {`A,111222333,S,5,110
A,111222334,S,3,110
A,111222335,B,10,90
A,111222335,B,2,90
A,111222335,B,6,85`, `A,111222333,S,5,110
A,111222334,S,3,110
A,111222335,B,10,90
A,111222335,B,2,90
A,111222335,B,6,85
A,111222336,S,5,111`, `A,111222333,S,5,110
A,111222334,S,3,110
A,111222335,B,10,90
A,111222335,B,2,90
A,111222335,B,6,85
A,111222336,S,5,111
A,111222336,S,5,130
A,111222337,S,25,130
A,111222353,S,5,109`}
	var orderStrDesignArray = [...]string {`A,0,S,4,110
A,0,S,3,110
A,0,S,8,108
A,0,B,10,90
A,0,B,2,90
A,0,B,6,85`, `A,0,S,4,110
A,0,S,3,110
A,0,S,8,108
A,0,B,10,90
A,0,B,2,90
A,0,B,6,85`, `A,0,S,4,110
A,0,S,3,110
A,0,B,8,108
A,0,S,10,109
A,0,B,10,90
A,0,B,2,90
A,0,B,6,85
A,0,S,15,130`}
	var expectResultArray = [...]string {`M,111222333,S,4,110
A,999888777,S,8,108`, `X,111222336,S,5,111
M,111222333,S,4,110
A,999888777,S,8,108`, `X,111222336,S,5,130
M,111222337,S,15,130
X,111222336,S,5,111
M,111222333,S,4,110
M,111222353,S,10,109
A,999888777,B,8,108`}

	for idx, _ := range orderStrCurrentArray {
		order_current.ClearAllOrder()
		order_design.ClearAllOrder()

		fmt.Println("test",  idx)
		orderCurrentSlice := strings.Split(orderStrCurrentArray[idx], "\n")
		orderDesignSlice := strings.Split(orderStrDesignArray[idx], "\n")
		errCurrent := input.DualInputCurrentOrder(orderCurrentSlice)
		errDesign := input.DualInputOrderDesign(orderDesignSlice)
		changeList, errSyn := order_design.OrderSyn()
		if errCurrent != nil || errDesign != nil || errSyn != nil {
			t.Errorf("test %d fail! %v %v %v", idx, errCurrent, errDesign, errSyn)
		}

		realResult := ""
		for _, val := range(changeList) {
			//M,111222333,S,4,110
			realResult += fmt.Sprintf("%c,%v,%c,%d,%d\n", val.OperType, val.OrderId, val.BuyOrSellFlag, val.OrderNum, val.ExpectPrice)
		}
		if len(changeList) > 0 {
			//删除末尾换行符
			realResult = strings.TrimSpace(realResult)
		}

		expectResult := expectResultArray[idx]
		if realResult != expectResult {
			t.Errorf("test %d expect [%s] get [%s]", idx, expectResult, realResult)
		}
	}

}
