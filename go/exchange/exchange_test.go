package main

import (
	"exchange/config"
	"exchange/order"
	"exchange/input"
	"fmt"
	log "github.com/htgx/htcomm/logger"
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

	var od order.Order
	od.OrderId = 10
	log.Infof("od = %v", od)

	var orderStrArray = [...]string {`A,100000,S,5,110
A,100001,B,10,90
A,100002,S,10,110
A,100003,S,3,105
A,100004,S,7,105
A,100005,B,4,100
A,100006,B,6,100
A,100007,B,2,90
A,100008,B,3,90`, `A,100000,S,5,110
A,100001,B,10,90
A,100002,S,10,110
A,100003,S,3,105
A,100004,S,7,105
A,100005,B,4,100
A,100006,B,6,100
A,100007,B,2,90
A,100008,B,3,90
A,100009,B,4,105`, `A,100000,S,5,110
A,100001,B,10,90
A,100002,S,10,110
A,100003,S,3,105
A,100004,S,7,105
A,100005,B,4,100
A,100006,B,6,100
A,100007,B,2,90
A,100008,B,3,90
A,100009,B,4,105
A,100010,S,23,90`, `A,100000,S,1,1075
A,100001,B,9,1000
A,100002,B,30,975
A,100003,S,10,1050
A,100004,B,10,950
A,100005,S,2,1025
A,100006,B,1,1000
X,100004,B,10,950`, `A,100000,S,1,1075
A,100001,B,9,1000
A,100002,B,30,975
A,100003,S,10,1050
A,100004,B,10,950
A,100005,S,2,1025
A,100006,B,1,1000
X,100004,B,10,950
X,100003,S,10,1050`, `A,100000,S,1,1075
A,100001,B,9,1000
A,100002,B,30,975
A,100003,S,10,1050
A,100004,B,10,950
A,100005,S,2,1025
A,100006,B,1,1000
X,100004,B,10,950
X,100003,S,10,1050
A,100007,S,9,1000
A,100008,S,1,1000`}
	var expectResultArray = [...]string {`=================
ASK
110: 5 10
105: 3 7
------------
100: 4 6
90: 10 2 3
BID
=================`, `=================
ASK
110: 5 10
105: 6
------------
100: 4 6
90: 10 2 3
BID
=================`, `=================
ASK
110: 5 10
105: 6
------------
90: 2
BID
=================`, `=================
ASK
1075: 1
1050: 10
1025: 2
------------
1000: 9 1
975: 30
BID
=================`, `=================
ASK
1075: 1
1025: 2
------------
1000: 9 1
975: 30
BID
=================`, `=================
ASK
1075: 1
1025: 2
------------
975: 30
BID
=================`}

	for idx, _ := range orderStrArray {
		order.ClearAllOrder()
		fmt.Println("test",  idx)
		orderSlice := strings.Split(orderStrArray[idx], "\n")
		realResult := input.DualOrder(orderSlice)
		expectResult := expectResultArray[idx]
		if realResult != expectResult {
			t.Errorf("test %d expect [%s] get [%s]", idx, expectResult, realResult)
		}
	}

}
