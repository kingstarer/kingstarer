package order

import (
	"errors"
	"exchange/snowflake"
	log "github.com/htgx/htcomm/logger"
	"time"
)
const ORDER_BUY = 'B'
const ORDER_SELL = 'S'

const ORDER_STATE_INIT = 0
const ORDER_STATE_PARTIAL = 1
const ORDER_STATE_ALL = 2
const ORDER_STATE_CANCLE = 3
const ORDER_STATE_PARTIAL_CANCLE = 4
//如果以后新增状态，需要注意调整Del函数


type Order struct  {
	goodId int64 //商品编号id 目前没有用
	/*为方便引用成员变量，所有属性都是公开的，但不要直接修改成员，而是通过成员函数修改*/
	OrderId           int64 //订单id
	OrderTime         int64 //下单时间戳
	BuyOrSellFlag     byte  //买卖标志（B买S卖）
	ExpectPrice       int   //预期价格
	OrderNum          int   //数量（下单时的数量）
	ExpectExchangeNum int   //待成交数量 初始化时与OrderNum保持一致，在部分成交或者成交后减去对应的值
	OrderState        byte  //订单状态（0未成交 1部分成交 2完全成交 3未成交撤销 4 部分成交撤销）
	//这里暂时不需要记录订单成交明细
}

var g_orderMap map[int64]*Order

/*
 * 生成工单结构休
 * 现在是无参的，以后可能要增加商品id
 */
func NewOrder()(*Order, error) {
	// 生成一个新工单
	return &Order{
		goodId : 1, //目前没有区分商品编号
	}, nil
}

//清空所有订单
func ClearAllOrder() {
	clearExchangeOrder()
	g_orderMap = map[int64]*Order{}
}

/*
 * 生成卖单
 */
func newSellOrder(expectPrice, orderNum int)(*Order, error) {
	// 生成一个新节点
	return &Order{
		OrderId : snowflake.GetNextId(),
		OrderTime : time.Now().Unix(),
		BuyOrSellFlag : 'S',
		ExpectPrice : expectPrice,
		OrderNum : orderNum,
	}, nil
}


/*
 * 生成买单
 */
func newBuyOrder(expectPrice, orderNum int)(*Order, error) {
	// 生成一个新节点
	return &Order{
		OrderId : snowflake.GetNextId(),
		OrderTime : time.Now().Unix(),
		BuyOrSellFlag : 'B',
		ExpectPrice : expectPrice,
		OrderNum : orderNum,
	}, nil
}

/*
增加订单
*/
func (m *Order) Add(orderId int64, buyOrSellFlag byte, expectPrice, orderNum int) error {
	//检查订单号是否已被使用 如果是则报错
	 _, existFlag := g_orderMap[orderId]
	 if existFlag {
	 	return errors.New("该订单已存在")
	 }

	 //将订单信息登记起来
	g_orderMap[orderId] = m
	m.OrderId = orderId
	m.OrderTime = time.Now().Unix()
	m.ExpectPrice = expectPrice
	m.OrderNum = orderNum
	//初始化时待成交数量与订单数量是一致的
	m.ExpectExchangeNum = m.OrderNum

	if buyOrSellFlag == ORDER_BUY {
		m.BuyOrSellFlag = ORDER_BUY
	} else {
		m.BuyOrSellFlag = ORDER_SELL
	}

	err := addOrderToExchangeList(m)
	if  err != nil {
		log.Errorf("添加到未成交订单失败 %v", err);
		return err
	}

	return nil
}

/*
根据订单id删除订单 需要该订单尚未完全成交
返回nil代表成功 否则代表失败
*/
func (m *Order) Del(orderId int64) error {
	log.Debugf("订单删除预检查 %v", orderId)

	//检查订单号是否已被使用 如果是则报错
	orderGet, existFlag := g_orderMap[orderId]
	if !existFlag {
		return errors.New("该订单不存在")
	}
	m = orderGet

	//检查订单是否可撤销
	if orderGet.OrderState == ORDER_STATE_ALL ||
		orderGet.OrderState == ORDER_STATE_CANCLE ||
		orderGet.OrderState == ORDER_STATE_PARTIAL_CANCLE {
		return errors.New("该订单已经完全交易或撤销")
	}

	//从未交易订单里面删除订单
	err := delOrderFromExchangeList(m)
	if  err != nil {
		log.Errorf("从未交易订单里面删除订单失败 %v", err);
		return err
	}

	//从订单信息里面移除
	delete(g_orderMap, m.OrderId)

	return nil
}


func init() {
	ClearAllOrder()
}

