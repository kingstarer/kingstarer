package input
import (
	"fmt"
	"ordersyn/order_current"
	"ordersyn/order_design"
)
import log "github.com/htgx/htcomm/logger"

/*负责输入订单信息和输出待成交订单分布情况*/

const OperTypeAdd = "A"
const OperTypeDel = "X"
const OperTypeMod = "M"

func getOrderCurrent() []string {

	fmt.Println("输入已提交订单明细（格式为：A,100001,B,9,1000），空行结束")
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



func getOrderDesign() []string {

	fmt.Println("输入规划订单明细（格式为：A,0,S,4,110），空行结束")
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
func DualInputCurrentOrder(inputStrSlice []string) error {

	for _, inputStr := range inputStrSlice {

		var operType string //操作类型（新增或者删除）
		var orderId int64 //订单id
		var orderType string //订单类型（买或者卖）
		var num int //买卖数量
		var expectPrice int//预期交易价格
		var err error

		_, err = fmt.Sscanf(inputStr, "%1s,%v,%1s,%v,%v", &operType, &orderId, &orderType, &num, &expectPrice)
		if err != nil || (operType != OperTypeAdd && operType != OperTypeDel) {
			log.Fatalf("解析订单失败：%v 可能是输入的订单格式不符合要求：%v", err, inputStr)
		}

		if operType == OperTypeAdd {
			err = order_current.Add(orderId, orderType[0], expectPrice, num)
			if err != nil {
				log.Fatalf("增加订单%v失败 %v 可能是订单号出现重复", orderId, err)
			}
		} else {
			err = order_current.Del(orderId, orderType[0], expectPrice)
			if err != nil {
				//有错误现在都是直接退出
				log.Fatalf("删除订单%v失败 %v 可能是订单不存在", orderId, err)
			}
			log.Debugf("del order ok %v", orderId)
		}
	}

	return nil
}


//将屏幕输入和输出分离，方便测试
func DualInputOrderDesign(inputStrSlice []string) error {

	for _, inputStr := range inputStrSlice {

		var operType string //操作类型（新增或者删除）
		var orderId int64 //订单id
		var orderType string //订单类型（买或者卖）
		var num int //买卖数量
		var expectPrice int//预期交易价格
		var err error

		_, err = fmt.Sscanf(inputStr, "%1s,%v,%1s,%v,%v", &operType, &orderId, &orderType, &num, &expectPrice)
		if err != nil || (operType != OperTypeAdd && operType != OperTypeDel) {
			log.Fatalf("解析订单失败：%v 可能是输入的订单格式不符合要求：%v", err, inputStr)
		}

		if operType == OperTypeAdd || operType == OperTypeDel {
			err = order_design.Add(orderType[0], expectPrice, num)
			if err != nil {
				log.Fatalf("设置规划订单%v失败 %v ", orderId, err)
			}
		}
	}

	return nil
}

//从屏蔽读取订单并进行撮合，最终输出待成交订单分布情况
func InputOrderAndDual() {
	/*从屏幕读取输入，输入格式类似这样
	A,100000,S,1,1075
	A,100001,B,9,1000
	直接遇到空行
	输出待成交订单分布情况*/

	//获取屏幕输入字符串（已提交的订单）
	log.Debugf("to getOrderCurrent")
	inputStrSlice := getOrderCurrent()
	//将输入的字符串进行处理
	log.Debugf("to DualInputCurrentOrder")
	err := DualInputCurrentOrder(inputStrSlice)
	if err != nil {
		fmt.Println("输入已提交订单失败", err)
		return
	}


	//获取屏幕输入字符串（规划的订单）
	log.Debugf("to getOrderDesign")
	inputStrSlice = getOrderDesign()
	//将输入的字符串进行处理
	log.Debugf("to DualInputOrderDesign")
	err = DualInputOrderDesign(inputStrSlice)
	if err != nil {
		fmt.Println("输入规划订单失败", err)
		return
	}

	//将提交订单转换成规划订单
	log.Debugf("to OrderSyn")
	changeList, err := order_design.OrderSyn()
	if err != nil {
		fmt.Println("获取修改记录失败", err)
		return
	}

	for _, val := range(changeList) {
		//M,111222333,S,4,110
		fmt.Printf("%c,%v,%c,%d,%d\n", val.OperType, val.OrderId, val.BuyOrSellFlag, val.OrderNum, val.ExpectPrice)
	}
}
