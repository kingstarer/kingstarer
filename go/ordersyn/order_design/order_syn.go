package order_design

import (
	listType "github.com/emirpasic/gods/lists/doublylinkedlist"
	"github.com/emirpasic/gods/maps/treemap"
	log "github.com/htgx/htcomm/logger"
	"ordersyn/order_current"
)

//不让循环引用input的常量
const OperTypeAdd = "A"
const OperTypeDel = "X"
const OperTypeMod = "M"

//输出订单修改记录，使得当前订单转换为规划订单
func OrderSyn() ([]*ExchangeLog, error) {
	orderCurrent := order_current.GetOrderCurrent()
	var result []*ExchangeLog
	changeOrderSell, err := orderSynBuyOrSell(order_current.ORDER_SELL, orderCurrent.OrderToSell, gOrderDesign.OrderToSell)
	if err != nil {
		log.Errorf("获取规划卖单同步交易修改记录失败%v", err)
		return nil, err
	}

	result = changeOrderSell
	changeOrderBuy, err := orderSynBuyOrSell(order_current.ORDER_BUY, orderCurrent.OrderToBuy, gOrderDesign.OrderToBuy)
	if err != nil {
		log.Errorf("获取规划买单同步交易修改记录失败%v", err)
		return nil, err
	}
	result = append(result, changeOrderBuy...)

	return result, nil
}


func orderSynBuyOrSell(buyOrSellFlag byte, orderToDualCurrent *treemap.Map, orderToDualDesign *treemap.Map) ([]*ExchangeLog, error) {
	result := make([] *ExchangeLog, 0, orderToDualCurrent.Size())

	//从当前订单数据结构中取出一个价格对应的订单列表
	priceCurrentInterface, orderListCurrentInterface := orderToDualCurrent.Max()
	//循环处理直接到取出空
	for priceCurrentInterface != nil {
		priceCurrent := priceCurrentInterface.(int)
		orderListCurrent := orderListCurrentInterface.(*listType.List)

		orderToDualCurrent.Remove(priceCurrent) //删除处理过的订单避免重复处理
		priceCurrentInterface, orderListCurrentInterface = orderToDualCurrent.Max()
		log.Debugf("to dual priceCurrent %d", priceCurrent)
		//该价格在规划订单数据结构里面是否存在
		numDesignInterface, ok := orderToDualDesign.Get(priceCurrent)
		if !ok {
			//如果不存在 则需要将这些订单全删除
			orderToDualCurrent.Each(func (key, val interface{}) {
				orderCurrent := val.(*order_current.Order)
				exchangeLog := ExchangeLog {
					OperType : OperTypeDel[0],
					OrderId : orderCurrent.OrderId,
					BuyOrSellFlag : orderCurrent.BuyOrSellFlag,
					ExpectPrice : orderCurrent.ExpectPrice,
					OrderNum : orderCurrent.OrderNum,
				}
				result = append(result, &exchangeLog)
			})
			continue
		}
		//将规划订单从map中移除，必免重复处理
		orderToDualDesign.Remove(priceCurrent)

		//获取规划订单数量
		numDesign := numDesignInterface.(int)
		numCurrent := 0 //计算已提交订单数量
		orderListCurrent.Each(func (idx int, val interface{}) {
			numCurrent += val.(*order_current.Order).OrderNum
		})

		numDiff := numDesign - numCurrent
		log.Debugf("price = %d numDesign = %d numCurrent = %d numDiff = %d",
			priceCurrent, numDesign, numCurrent, numDiff)

		//已提交订单数量与规划订单数量一致 不需要处理
		if numDesign == numCurrent {
			continue
		} else if numDesign < numCurrent {
			//已提交订单小于规划订单 直接将第一个订单数量增加差额即可
			orderCurrentInterface, _ := orderListCurrent.Get(0)
			orderCurrent := orderCurrentInterface.(*order_current.Order)

			exchangeLog := ExchangeLog {
				OperType : OperTypeMod[0],
				OrderId : orderCurrent.OrderId,
				BuyOrSellFlag : orderCurrent.BuyOrSellFlag,
				ExpectPrice : orderCurrent.ExpectPrice,
				OrderNum : orderCurrent.OrderNum + numDiff,
			}
			result = append(result, &exchangeLog)
		} else {
			//已提交订单大于规划订单 需要删除部分规划订单
			//遍历已提交订单，进行订单删除或者减少，直到差额变0
			orderListCurrent.Each(func (idx int, val interface{}) {
				if numDiff <= 0 {
					//应该直接终止遍历的 但没这个函数
					return
				}
				orderCurrent := val.(*order_current.Order)

				//如果差额比订单大 直接删除
				if numDiff >= orderCurrent.OrderNum {
					numDiff -= orderCurrent.OrderNum
					exchangeLog := ExchangeLog {
						OperType : OperTypeDel[0],
						OrderId : orderCurrent.OrderId,
						BuyOrSellFlag : orderCurrent.BuyOrSellFlag,
						ExpectPrice : orderCurrent.ExpectPrice,
						OrderNum : orderCurrent.OrderNum,
					}
					result = append(result, &exchangeLog)
				} else {
					//如果差额比订单小 则需要发起修改
					exchangeLog := ExchangeLog {
						OperType : OperTypeMod[0],
						OrderId : orderCurrent.OrderId,
						BuyOrSellFlag : orderCurrent.BuyOrSellFlag,
						ExpectPrice : orderCurrent.ExpectPrice,
						OrderNum : orderCurrent.OrderNum - numDiff,
					}
					result = append(result, &exchangeLog)
					numDiff = 0
					//到这里可以终止遍历了 但没这个函数
				}
			})
		}

	}

	//遍历规划订单数据结构，对该价格订单进行增加操作
	orderToDualDesign.Each(func (key, val interface{}) {
		log.Debugf("to dual priceDesign %d", key.(int))
		exchangeLog := ExchangeLog {
			OperType : OperTypeAdd[0],
			OrderId : 999888777, //这个数字是题目里面的 不知道有啥意义
			BuyOrSellFlag : buyOrSellFlag,
			ExpectPrice : key.(int),
			OrderNum : val.(int),
		}
		result = append(result, &exchangeLog)
	})

	orderToDualDesign.Clear()

	return result, nil
}

