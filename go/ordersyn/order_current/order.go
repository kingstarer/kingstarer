package order_current

import (
	"errors"
	listType "github.com/emirpasic/gods/lists/doublylinkedlist"
	"github.com/emirpasic/gods/maps/treemap"
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
	//这里暂时不需要记录订单成交明细
}


type OrderCurrent struct{
	//待买订单 Map<int, List<Order*>>
	OrderToBuy *treemap.Map
	//待卖订单 Map<int, List<Order*>>
	OrderToSell *treemap.Map
}

func (m *OrderCurrent) add(order *Order) error {
	var orderMapToAdd *treemap.Map;

	if order.BuyOrSellFlag == ORDER_BUY {
		orderMapToAdd = m.OrderToBuy;
	} else {
		orderMapToAdd = m.OrderToSell;
	}

	//根据订单价格找到对应的列表 如果不存在则新增列表（双向列表）
	var list *listType.List
	val, ok := orderMapToAdd.Get(order.ExpectPrice)
	if !ok {
		log.Debugf("create new list")
		list = listType.New()
		orderMapToAdd.Put(order.ExpectPrice, list)
	} else {
		log.Debugf("use exist list")
		list = val.(*listType.List)
	}

	haveSameOrderFlag := false
	list.Each(func (idx int, val interface{}) {
		orderAdded := val.(* Order)
		if orderAdded.OrderId == order.OrderId {
			haveSameOrderFlag = true
			//相同价格且订单相同 则将订单数量进行累加
			orderAdded.OrderNum += order.OrderNum
		}
	})

	if !haveSameOrderFlag {
		log.Debugf("add order.OrderId = %v", order.OrderId)
		list.Add(order)
	}

	return nil;
}


func (m *OrderCurrent) del(buyOrSellFlag byte, orderId int64, expectPrice int) error {

	var orderMapToAdd *treemap.Map;

	if buyOrSellFlag == ORDER_BUY {
		orderMapToAdd = m.OrderToBuy;
	} else {
		orderMapToAdd = m.OrderToSell;
	}


	//根据订单价格找到对应的列表 如果不存在则直接返回
	var list *listType.List
	val, ok := orderMapToAdd.Get(expectPrice)
	if !ok {
		return nil
	}

	list = val.(*listType.List)
	//找到列表中有相同id的订单 并删除
	haveSameOrderFlag := false
	idxFounded := -1
	list.Each(func (idx int, val interface{}) {
		orderAdded := val.(* Order)
		if orderAdded.OrderId == orderId {
			haveSameOrderFlag = true
			idxFounded = idx
			//TODO: 不知道如何停止遍历
		}
	})

	if haveSameOrderFlag {
		list.Remove(idxFounded)
	}

	return nil
}


func (m *OrderCurrent) modify(buyOrSellFlag byte, orderId int64, expectPrice, orderNum int) error {
	var orderMapToAdd *treemap.Map;

	if buyOrSellFlag == ORDER_BUY {
		orderMapToAdd = m.OrderToBuy;
	} else {
		orderMapToAdd = m.OrderToSell;
	}

	//根据订单价格找到对应的列表 如果不存在则直接返回
	var list *listType.List
	val, ok := orderMapToAdd.Get(expectPrice)
	if !ok {
		return nil
	}

	list = val.(*listType.List)
	haveSameOrderFlag := false
	//找到列表中有相同id的订单 并将订单数量改为新的订单数量
	list.Each(func (idx int, val interface{}) {
		orderAdded := val.(* Order)
		if orderAdded.OrderId == orderId {
			orderAdded.OrderNum = orderNum
			haveSameOrderFlag = true
			//TODO: 不知道如何停止遍历
		}
	})

	if !haveSameOrderFlag {
		return errors.New("未找到对应的订单")
	}

	return nil
}


/*
 * 生成工单结构休
 * 现在是无参的，以后可能要增加商品id
 */
func newOrder(orderId int64, buyOrSellFlag byte, expectPrice, orderNum int)(*Order, error) {
	// 生成一个新工单
	order := &Order{
		goodId : 1, //目前没有区分商品编号
	}

	//将订单信息登记起来
	order.OrderId = orderId
	order.OrderTime = time.Now().Unix()
	order.ExpectPrice = expectPrice
	order.OrderNum = orderNum

	if buyOrSellFlag == ORDER_BUY {
		order.BuyOrSellFlag = ORDER_BUY
	} else {
		order.BuyOrSellFlag = ORDER_SELL
	}

	return order, nil
}

var gOrdercurrent OrderCurrent

//清空所有订单
func ClearAllOrder() {
	gOrdercurrent = OrderCurrent {}
	gOrdercurrent.OrderToSell = treemap.NewWithIntComparator()
	gOrdercurrent.OrderToBuy = treemap.NewWithIntComparator()
}

func GetOrderCurrent() *OrderCurrent {
	return &gOrdercurrent;
}


/*
增加订单
*/
func Add(orderId int64, buyOrSellFlag byte, expectPrice, orderNum int) error {
	order, _ := newOrder(orderId, buyOrSellFlag, expectPrice, orderNum)

	err := gOrdercurrent.add(order)
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
func Del(orderId int64, buyOrSellFlag byte, expectPrice int) error {
	err := gOrdercurrent.del(buyOrSellFlag, orderId, expectPrice)
	if  err != nil {
		log.Errorf("根据订单id删除订单失败 %v", err);
		return err
	}

	return nil
}


/*
修改订单
*/
func Modify(orderId int64, buyOrSellFlag byte, expectPrice, orderNum int) error {
	err := gOrdercurrent.modify(buyOrSellFlag, orderId, expectPrice, orderNum)
	if  err != nil {
		log.Errorf("根据订单id修改订单失败 %v", err);
		return err
	}

	return nil
}

func init() {
	ClearAllOrder()
}

