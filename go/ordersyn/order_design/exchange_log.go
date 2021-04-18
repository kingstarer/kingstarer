package order_design

//交易日志结构体
type ExchangeLog struct {
	//M,111222333,S,4,110
	OperType     byte  //操作类型 A X M
	OrderId int64
	BuyOrSellFlag     byte  //买卖标志（B买S卖）
	ExpectPrice int
	OrderNum int
}
