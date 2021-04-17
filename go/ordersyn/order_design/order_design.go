package order_design

import (
	"github.com/emirpasic/gods/maps/treemap"
	log "github.com/htgx/htcomm/logger"
	"ordersyn/order_current"
)

type OrderDesign struct{
	//待买订单 Map<int, int>
	OrderToBuy *treemap.Map
	//待卖订单 Map<int,int>
	OrderToSell *treemap.Map
}

var gOrderDesign OrderDesign

//清空所有订单
func ClearAllOrder() {
	gOrderDesign = OrderDesign {}
	gOrderDesign.OrderToSell = treemap.NewWithIntComparator()
	gOrderDesign.OrderToBuy = treemap.NewWithIntComparator()
}


func (m *OrderDesign) add(buyOrSellFlag byte, expectPrice, orderNum int) error {
	var orderMapToAdd *treemap.Map;

	if buyOrSellFlag == order_current.ORDER_BUY {
		orderMapToAdd = m.OrderToBuy;
	} else {
		orderMapToAdd = m.OrderToSell;
	}

	log.Debugf("新增规划订单 价格%d 数量%d", expectPrice, orderNum)
	//设置该价格期望成交的订单数量
	val, ok := orderMapToAdd.Get(expectPrice)
	if ok {
		orderMapToAdd.Put(expectPrice, orderNum + val.(int))
	} else {
		orderMapToAdd.Put(expectPrice, orderNum)
	}


	return nil;
}



/*
增加订单
*/
func Add(buyOrSellFlag byte, expectPrice, orderNum int) error {

	err := gOrderDesign.add(buyOrSellFlag, expectPrice, orderNum)
	if  err != nil {
		log.Errorf("添加到未成交订单失败 %v", err);
		return err
	}

	return nil
}


func init() {
	ClearAllOrder()
}

