
package main

import (
	"fmt"
	"getHttpInfo/config"
	"getHttpInfo/proxy"
	"github.com/360EntSecGroup-Skylar/excelize"
	log "github.com/htgx/htcomm/logger"
	"sync"
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

	siteArray, err := config.ReadConfig("site.txt")
	if err != nil {
		log.Errorf("获取待查询的网址失败%v", err)
		return
	}


	var wg sync.WaitGroup

	for _, site := range siteArray {
		wg.Add(1)
		log.Debug("site = ", site)
		go func(tmpSite string) {
			defer wg.Done()
			getActive(proxyIp, tmpSite)
		}(site)
	}

	wg.Wait()

}

/**
获取网址历史档案
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