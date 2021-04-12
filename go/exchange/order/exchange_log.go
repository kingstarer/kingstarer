package order

//交易日志结构体
type ExchangeLog struct{
	OrderIdSell int64
	OrderIdBuy int64
	DualPrice int
	DualNum int
}
