package input
import (
	"exchange/order"
	"fmt"
)
import log "github.com/htgx/htcomm/logger"

/*负责输入订单信息和输出待成交订单分布情况*/

const operTypeAdd = "A"
const operTypeDel = "X"

func getOrderList() []string {

	fmt.Println("输入订单明细（格式为：A,100001,B,9,1000），空行结束")
	result := make([]string, 0, 100) //预留100行记录
	for {
		var inputStr string
		_, err := fmt.Scanln(&inputStr)

		if err != nil || inputStr == "" {
			//读取失败或者碰到空行时退出
			break
		}

		result = append(result, inputStr)
	}

	return result
}

//将屏幕输入和输出分离，方便测试
func do(inputStrSlice []string) string {

	for _, inputStr := range inputStrSlice {

		theOrder, err := order.NewOrder()
		if err != nil {
			log.Fatalf("生成订单失败 %v", err)
		}

		var operType string //操作类型（新增或者删除）
		var orderId int64 //订单id
		var orderType string //订单类型（买或者卖）
		var num int //买卖数量
		var expectPrice int//预期交易价格

		_, err = fmt.Sscanf(inputStr, "%1s,%v,%1s,%v,%v", &operType, &orderId, &orderType, &num, &expectPrice)
		if err != nil || (operType != operTypeAdd && operType != operTypeDel) {
			log.Fatalf("解析订单失败：%v 可能是输入的订单格式不符合要求：%v", err, inputStr)
		}

		if operType == operTypeAdd {
			err = theOrder.Add(orderId, orderType[0], expectPrice, num)
			if err != nil {
				log.Fatalf("增加订单%v失败 %v 可能是订单号出现重复", orderId, err)
			}
		} else {
			err = theOrder.Del(orderId)
			if err != nil {
				//有错误现在都是直接退出

				log.Fatalf("删除订单%v失败 %v 可能是订单不存在", orderId, err)
			}
		}
	}

	log.Debugf("to GetExchangeOrder")
	//获取未成交订单分布情况
	result, err := order.GetExchangeOrder()
	if err != nil {
		log.Fatalf("获取未成交订单分布情况失败 %v", err)
	}

	return result
}

//从屏蔽读取订单并进行撮合，最终输出待成交订单分布情况
func InputOrderAndDual() {
	/*从屏幕读取输入，输入格式类似这样
	A,100000,S,1,1075
	A,100001,B,9,1000
	直接遇到空行
	输出待成交订单分布情况*/

	//获取屏幕输入字符串
	log.Debugf("to getOrderList")
	inputStrSlice := getOrderList()
	//将输入的字符串进行处理
	log.Debugf("to do dual")
	exchangeOrder := do(inputStrSlice)
	//输出待交易订单分布情况到屏幕
	fmt.Println(exchangeOrder)
}
