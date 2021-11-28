
package main

import (
	"fmt"
	"getFundInfo/config"
	"getFundInfo/proxy"
	"github.com/360EntSecGroup-Skylar/excelize"
	"github.com/axgle/mahonia"
	log "github.com/htgx/htcomm/logger"
	"net/http"
	"strings"
	"time"
)



func IndexHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "封基查询")
}

func GetFundHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Access-Control-Allow-Origin", "*")

//	fmt.Fprintln(w, `{
//	"total": 1,
//	"rows": [{
//		"firstname": "hch",
//		"lastname": "yes",
//		"phone": "13560453764",
//		"email": "135"
//	}]
//}
//`)

	fmt.Fprintln(w, ` [{
		"firstname": "hch",
		"lastname": "yes",
		"phone": "135604537684",
		"email": "135"
	}]
`)
}


func GetFundHandlerGz(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	body, err := proxy.HttpGetFund("506002")
	if err != nil {
		body = fmt.Sprintf("error:%v", err)
	} else {
		body = strings.TrimPrefix(body, "jsonpgz(");
		body = body[0 : len(body) - 2]
	}

	fmt.Fprintln(w, "[" + body + "]");
}



func GetFundClose(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	body, err := proxy.HttpGet("http://fund.eastmoney.com/CXXFBS_dwjz.html")
	if err != nil {
		body = fmt.Sprintf("error:%v", err)
	}

	dec := mahonia.NewDecoder("gbk")

	body = dec.ConvertString(body)

	fmt.Fprintln(w, body);
}


func main() {
	err := config.LoadConfig("config.toml")
	if err != nil {
		fmt.Printf("加载配置文件失败：%v\n", err)
		return
	}

	log.MustInitLog(config.CfgGetHttpInfo.LogCfg.LogName, config.CfgGetHttpInfo.LogCfg.LogLevel,
		config.CfgGetHttpInfo.LogCfg.LogPath, config.CfgGetHttpInfo.LogCfg.StdoutEnableFlag)
	log.Info("init ok")


	http.HandleFunc("/", IndexHandler)
	http.HandleFunc("/getFund", GetFundHandler)
	http.HandleFunc("/getFundGz", GetFundHandlerGz)
	http.HandleFunc("/getFundClose", GetFundClose)
	err = http.ListenAndServe("127.0.0.1:8100", nil)

	log.Info("end %v", err)
}

/**
获取基金当前净值数据
 */
func getActive(proxyIp, site string) {
	log.Debugf("开始处理网址%v", site)

	code, data, err := proxy.HttpProxyGet(proxyIp, config.CfgGetHttpInfo.Api.ArchiveApi + site)

	if err != nil {
		log.Errorf("getActive fail %v", err)
		return
	}

	log.Infof("code = %d data = %s", code, data)

	dataMap := make(map[string]string)
	dataMap["A2"] = site

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

	if data != nil {
		for k, v := range data {
			xlsx.SetCellValue("正常记录", k, v)
		}
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