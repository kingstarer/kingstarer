
package main

import (
	"fmt"
	"getHttpInfo/config"
	"getHttpInfo/proxy"
	log "github.com/htgx/htcomm/logger"
	"io/ioutil"
	"strings"
	"sync/atomic"
	"time"
)

func main() {
	err := config.LoadConfig("config.toml")
	if err != nil {
		fmt.Printf("加载配置文件失败：%v\n", err)
		return
	}

	log.MustInitLog(config.CfgGetHttpInfo.LogCfg.LogName, config.CfgGetHttpInfo.LogCfg.LogLevel,
		config.CfgGetHttpInfo.LogCfg.LogPath, config.CfgGetHttpInfo.LogCfg.StdoutEnableFlag)
	log.Info("init ok")

	var localIp string;
	if localIp, err = proxy.GetLocalIp(); err != nil {
		log.Debug("GetLocalIp fail!", err)
		return
	}
	log.Info("本机ip ", localIp)

	proxyIp, err := proxy.GetProxyIp()
	log.Info("proxyIp ", proxyIp)


	//使用socks5代理访问方法
	proxy.Socksproxy()

	//别人成功的测试
	statusCode, str, _ := proxy.HttpProxyGet(proxyIp, "http://myip.top")
	fmt.Printf("code = %v httpproxyTest:%v\n", statusCode, str)
}



//检测数据并处理
//需要处理的数据 data
//返回 异常信息
func getTitle(data string) error {
	lines := strings.Split(data, "\r\n")
	//TODO:获取年份、标题等数据
	fmt.Println(lines)
	return nil
}

//获取配置
//指定的路径 path
//返回以行为单位的string类型配置数组
func ReadConfig(path string) []string {
	dat, err := ioutil.ReadFile(path)
	check(err)
	return strings.Split(string(dat), "\r\n")
}


/*
支线测试服务器
103.96.149.85:33890
administrator
82p3hdtf
*/

package main

import (
"fmt"
"io/ioutil"
"log"
"os"
"strings"
"sync/atomic"
"time"

"github.com/Luxurioust/excelize"
"github.com/Unknwon/goconfig"
)

var cfg *goconfig.ConfigFile
var index uint64        //代理取值下标
var siteArray []string  //需要操作的站点
var proxyArray []string //代理地址集合
var userAgent []string  //头信息集合

var ptl = fmt.Println

//初始化资源
func init() {
	siteArray = ReadConfig("site.txt")                   //站点数组
	proxyArray = ReadConfig("host.txt")                  //代理数组
	userAgent = ReadConfig("UserAgent.txt")              //头信息
	config, err := goconfig.LoadConfigFile("config.ini") //加载配置文件
	if err != nil {
		fmt.Println("读取配置文件异常")
		os.Exit(-1)
	}
	cfg = config
}

func main() {

	//TODO:1初始化资源 系统自动调用 init 方法

	//TODO:2设置缓冲区
	jobs := make(chan string, len(siteArray))
	results := make(chan siteModel, len(siteArray))

	//TODO:3启动线程池
	MaxThreadCount, _ := cfg.Int("thread", "count")
	for i := 0; i < MaxThreadCount; i++ {
		go worker(i, jobs, results)
	}

	//TODO:4填充数据
	for _, sitePath := range siteArray { //循环站点数据
		jobs <- sitePath
	}
	//TODO:5整理数据
	charList := "CDEFGHIJKLMNOPQRSTUVWXYZ"
	dataMap := make(map[string]string)
	for i := 0; i < len(siteArray); i++ {
		model := <-results
		ptl(model)
		cell1 := fmt.Sprintf("A%d", i+2)
		cell2 := fmt.Sprintf("B%d", i+2)
		dataMap[cell1] = model.SitePath
		dataMap[cell2] = model.TimeSpan
		for j := 0; j < len(model.Title); j++ {
			charX := charList[j]
			cellX := fmt.Sprintf("%s%d", string(charX), i+2)
			dataMap[cellX] = model.Title[j]
		}
		fmt.Println()
	}
	//TODO:6保存数据
	saveData(dataMap)
}


//站点信息
type siteModel struct {
	SitePath string   //站点名称
	TimeSpan string   //时间间隔
	Title    []string //标题数组
}


func saveData(data map[string]string) {
	xlsx := excelize.NewFile()
	index := xlsx.NewSheet("正常记录")

	//写入数据
	head := map[string]string{
		"A1": "域名",
		"B1": "域名历史",
		"C1": "标题&时间",
	}

	for k, v := range head {
		xlsx.SetCellValue("正常记录", k, v)
	}
	for k, v := range data {
		xlsx.SetCellValue("正常记录", k, v)
	}

	xlsx.SetActiveSheet(index)

	//保存文件到指定路径
	t := time.Now()
	fileName := fmt.Sprintf("%d%d%d%d%d.xlsx", t.Year(), t.Month(), t.Day(), t.Hour(), t.Minute())
	err := xlsx.SaveAs(fileName)
	if err != nil {
		log.Fatal(err)
	}
}

//获取代理ip
//返回一个代理ip
func getNewProxy() string {
	if index > (uint64)(len(proxyArray)) {
		index = 0
	}
	ip := proxyArray[index]     //当前代理IP地址
	atomic.AddUint64(&index, 1) //下标自增1
	return ip
}

//工作线程
//线程ID id
//需要处理的数据集合 jobs
//返回数据集合 results
func worker(id int, jobs <-chan string, results chan<- siteModel) {

	for sitePath := range jobs {
		msg := fmt.Sprintf("线程%d-正在处理站点%s", id, sitePath)
		ptl(msg)
		var model siteModel
		model.SitePath = sitePath
		getTimeMap(id, sitePath, &model)
		results <- model
	}
}

//获取最后一条记录
func getLastInfo() {

}

//获取最后一条信息并跳转至页面
func getLastInfoAndShow() {

}

//获取timemap
//站点地址 sitePath
//代理地址 proxyIP
func getTimeMap(id int, sitePath string, model *siteModel) {
	var proxyIP string
	var dat string
	var msg string
	var error_data string
	error_data, _ = cfg.GetValue("select", "error_data")

	//无限循环获取当前站点信息
	for {
		proxyIP = getNewProxy() //获取代理
		timeMapUrl := "http://web.archive.org/web/timemap/link/" + sitePath
		data, err := httpproxy(proxyIP, timeMapUrl) //获取数据
		if err != nil {                             //是否有异常，该层异常为网络层异常，需要切换代理
			msg = fmt.Sprintf("线程%d-获取站点%s异常，更换代理ip", id, timeMapUrl)
			ptl(msg)
			ptl(err)
		} else { //获取到数据
			dat = data
			if dat == error_data {
				msg = fmt.Sprintf("线程%d-已获取站点%s信息，但数据异常，更换代理IP", id, timeMapUrl)
				ptl(msg)
			} else {
				tempList := strings.Split(data, "\n")
				if len(tempList) > 3 {
					startTime := tempList[3][28:32]
					endTime := tempList[len(tempList)-2][28:32]
					model.TimeSpan = fmt.Sprintf("%s-%s", startTime, endTime)
				}
				msg = fmt.Sprintf("线程%d-已获取站点%s历史列表", id, timeMapUrl)
				ptl(msg)
				break
			}
		}
	}
}

//检测数据并处理
//需要处理的数据 data
//返回 异常信息
func getTitle(data string) error {
	lines := strings.Split(data, "\r\n")
	//TODO:获取年份、标题等数据
	fmt.Println(lines)
	return nil
}

//获取配置
//指定的路径 path
//返回以行为单位的string类型配置数组
func ReadConfig(path string) []string {
	dat, err := ioutil.ReadFile(path)
	check(err)
	return strings.Split(string(dat), "\r\n")
}

//检测系统级异常 不处理其他逻辑
//异常信息 e
func check(e error) {
	if e != nil {
		panic(e)
	}
}


