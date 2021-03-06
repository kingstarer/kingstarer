package order

import (
	listType "github.com/emirpasic/gods/lists/doublylinkedlist"
	"github.com/emirpasic/gods/maps/treemap"
	"fmt"
	log "github.com/htgx/htcomm/logger"
)


type ExchangeOrder struct{
	//待买订单 Map<int, List<int64>>
	OrderToBuy *treemap.Map
	//待卖订单 Map<int, List<int64>>
	OrderToSell *treemap.Map
	//持摄合标志
	ToMatchFlag bool
}

var g_exchangeOrder ExchangeOrder

func clearExchangeOrder() {
	g_exchangeOrder = ExchangeOrder {
	}

	g_exchangeOrder.OrderToBuy = treemap.NewWithIntComparator()
	g_exchangeOrder.OrderToSell = treemap.NewWithIntComparator()
}

/*新增未成交订单*/
func addOrderToExchangeList(order *Order) error {

	//标记需要撮合 下次获取待摄合信息前会自动执行撮合操作
	g_exchangeOrder.ToMatchFlag = true

	//先取出list 能取到则直接追加 否则新增一个
	var mapToDual *treemap.Map
	if order.BuyOrSellFlag == ORDER_BUY {
		mapToDual = g_exchangeOrder.OrderToBuy
	} else {
		mapToDual = g_exchangeOrder.OrderToSell
	}
	log.Debugf("order.BuyOrSellFlag = %c", order.BuyOrSellFlag)

	var list *listType.List
	val, ok := mapToDual.Get(order.ExpectPrice)
	if !ok {
		log.Debugf("create new list")
		list = listType.New()
		mapToDual.Put(order.ExpectPrice, list)
	} else {
		log.Debugf("use exist list")
		list = val.(*listType.List)
	}

	log.Debugf("add order.OrderId = %v", order.OrderId)
	list.Add(order.OrderId)
	return nil
}

/*删除未成交订单*/
func delOrderFromExchangeList(order *Order) error {
	log.Debugf("delOrderFromExchangeList")
	var orderIdListMap *treemap.Map; //根据买卖类型 决定从哪个map里面删除订单
	var list *listType.List; //根据价格找出来的订单列表

	if order.BuyOrSellFlag == 'B' {
		orderIdListMap = g_exchangeOrder.OrderToBuy
	} else {
		orderIdListMap = g_exchangeOrder.OrderToSell
	}

	//根据价格取出该价格下订单列表 目前订单数量较少 不需要考虑索引
	tmpList, foundFlag := orderIdListMap.Get(order.ExpectPrice)
	if !foundFlag {
		//取不到直接返回
		return nil
	}

	//根据订单编号删除订单
	list = tmpList.(*listType.List);
	orderNodeSn := list.IndexOf(order.OrderId)
	list.Remove(orderNodeSn)

	return nil
}

//进行撮合交易  核心函数
func dualMatch(listSell, listBuy *listType.List) error {
	/*
	以B的价格进行成交，成交数量取买和卖数量最小值。
	如果买的数量大于卖的数量，则标记部分成交即可
	（注意打印交易信息，方便后续验证）
	如果卖的数量大于买的数量，则说明订单完全成交，需要从未成交订单里面移除并且对下个订单进行处理
	*/

	//遍历卖单 进行成交
	iteratorSell := listSell.Iterator()
	for iteratorSell.Next() {
		//TODO:这段逻辑有重复 可以考虑提取成函数
		orderIdSell := iteratorSell.Value().(int64)
		orderSell := g_orderMap[orderIdSell]
		numToSell := orderSell.ExpectExchangeNum
		expectSellPrice := orderSell.ExpectPrice

		iteratorBuy := listBuy.Iterator()
		//遍历卖单 进行成交
		for iteratorBuy.Next() {
			orderIdBuy := iteratorBuy.Value().(int64)
			orderBuy := g_orderMap[orderIdBuy]
			numToBuy := orderBuy.ExpectExchangeNum
			expectBuyPrice := orderBuy.ExpectPrice

			if expectBuyPrice < expectSellPrice {
				//买单价格低于卖单 无法成交
				break
			}

			if !dualList(expectBuyPrice, numToSell, numToBuy, orderBuy, orderSell) {
				//卖单全部成交或者没有合适的买单则不需要再处理卖单
				break
			}
		}

		//遍历买单 把完全成交的订单从未交易订单里面删除订单
		dualCompDualOrder(listBuy)
	}

	//遍历卖单 把完全成交的订单从未交易订单里面删除订单
	dualCompDualOrder(listSell)

	return nil
}

func dualCompDualOrder(listBuy *listType.List) {
	iteratorBuy := listBuy.Iterator()
	ok := iteratorBuy.Next()
	for ok {
		orderIdBuy := iteratorBuy.Value().(int64)
		orderBuy := g_orderMap[orderIdBuy]
		numToBuy := orderBuy.ExpectExchangeNum

		idx := iteratorBuy.Index()
		ok = iteratorBuy.Next()
		if numToBuy == 0 {
			//需要先往后移再删除
			log.Debugf("移除完全成交%c单%d id = %v", orderBuy.BuyOrSellFlag, idx, orderIdBuy)
			listBuy.Remove(idx)
		}
	}
}

//将卖单与买单进行撮合成交
//返回true代表交易需要再处理 返回false代表交易不需要再处理
func dualList(expectBuyPrice int, numToSell int, numToBuy int, orderBuy *Order, orderSell *Order) bool {
	result := true

	changeLog := ExchangeLog{
		OrderIdSell: orderSell.OrderId,
		OrderIdBuy:  orderBuy.OrderId,
		DualPrice:   expectBuyPrice,
	}

	log.Debugf("成交价格%d 预期买单 %d 预期卖单 %d", expectBuyPrice, numToBuy, numToSell)

	log.Debugf("卖单情况%v", orderSell)
	log.Debugf("买单情况%v", orderBuy)

	if numToSell == numToBuy {
		//买卖数量一致，最好处理了 把买单和卖单预期交易数量改为0即可
		orderBuy.ExpectExchangeNum = 0
		orderBuy.OrderState = ORDER_STATE_ALL
		orderSell.ExpectExchangeNum = 0
		orderSell.OrderState = ORDER_STATE_ALL
		changeLog.DualNum = numToSell
		//卖单完全成交 可以不需要处理了
		result = false

	} else if numToSell < numToBuy {
		//买单部分成交
		orderBuy.ExpectExchangeNum -= numToSell
		orderBuy.OrderState = ORDER_STATE_PARTIAL
		//卖单完全成交
		orderSell.ExpectExchangeNum = 0
		orderSell.OrderState = ORDER_STATE_ALL
		changeLog.DualNum = numToSell
		//卖单完全成交 可以不需要处理了
		result = false
	} else {
		//买单部分成交
		orderBuy.ExpectExchangeNum = 0
		orderBuy.OrderState = ORDER_STATE_ALL
		//卖单部分成交
		orderSell.ExpectExchangeNum -= numToBuy
		orderSell.OrderState = ORDER_STATE_PARTIAL
		changeLog.DualNum = numToBuy
		//继续处理下个买单 看是否可以成交
		result = true
	}

	log.Debugf("changeLog = %v", changeLog)
	return result
}


func matchExchangeOrder() {
	if !g_exchangeOrder.ToMatchFlag {
		return
	}

	/*
		a 取出最大的买单价格B和最小的卖单价格S，如果B<S，则结束
		b 如果B>=S（有买单价格高于卖单）
		c 遍历价格为B的订单，进行订单成交，成交方法见后面描述。
		d 返回a
	*/
	for {
		//获取最大买单和最小卖单 如果没有的话则不用交易
		tmpResult, listSellInter := g_exchangeOrder.OrderToSell.Min()
		if tmpResult == nil {
			break
		}
		minSell := tmpResult.(int)

		tmpResult, listBuyInter := g_exchangeOrder.OrderToBuy.Max()
		if tmpResult == nil {
			break
		}
		maxBuy := tmpResult.(int)

		//最大的买单价格都没有最小的卖单价格高 就不用再判断了
		if maxBuy < minSell {
			break
		}

		//进行撮合
		log.Debugf("to dualMatch")
		listSell := listSellInter.(*listType.List)
		listBuy := listBuyInter.(*listType.List)
		if dualMatch(listSell, listBuy) != nil {
			log.Errorf("dualMatch error")
		}

		//撮合后如果对应价格没有订单 则将该价格从待撮合订单结构体里面删除
		if listSell.Empty() {
			log.Debugf("该价格位上的卖单完全成交 需要移除")
			g_exchangeOrder.OrderToSell.Remove(minSell)
		}

		if listBuy.Empty() {
			log.Debugf("该价格位上的买单完全成交 需要移除")
			g_exchangeOrder.OrderToBuy.Remove(maxBuy)
		}
	}

	//标记已撮合
	g_exchangeOrder.ToMatchFlag = false;
}

func getOrderListStr(orderMap *treemap.Map) string {
	var result string
	s := orderMap.Values()
	for i := len(s); i > 0; i-- {
		if s[i - 1] == nil {
			continue
		}

		list := s[i - 1].(*listType.List)
		if list.Empty() {
			continue
		}

		t, ok := list.Get(0)
		if !ok {
			continue
		}

		orderId := t.(int64)
		order := g_orderMap[orderId]
		price := order.ExpectPrice
		result += fmt.Sprintf("%d:", price)
		list.Each(func (idx int, val interface{}) {
			orderId := val.(int64)
			order := g_orderMap[orderId]
			result += fmt.Sprintf(" %d", order.ExpectExchangeNum)
		})

		result += "\n"
	}

	return result
}

/*获取待成交订单分布情况*/
func GetExchangeOrder() (string, error) {
	// 如果未摄合，则先进行摄合
	if g_exchangeOrder.ToMatchFlag {
		log.Infof("to matchExchangeOrder")
		matchExchangeOrder()
	}

	result :=  "=================\nASK\n"
	//获取卖单 按价格逆序输出
	result += getOrderListStr(g_exchangeOrder.OrderToSell)
	result += "------------\n"
	//获取买单 按价格逆序输出
	result += getOrderListStr(g_exchangeOrder.OrderToBuy)
	result += "BID\n================="
	/*
	按以下结构返回订单分布情况
	=================
	ASK
	110: 5 10
	105: 6
	------------
	90: 10 2 3
	BID
	=================
	*/
	//"=================\nASK\n110: 5 10\n105: 6\n------------\n90: 10 2 3\nBID\n=================", nil
	return result, nil
}