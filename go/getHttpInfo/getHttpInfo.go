
package main

import (
	"fmt"
	"getHttpInfo/config"
	"getHttpInfo/proxy"
	log "github.com/htgx/htcomm/logger"
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
	str, _ := proxy.HttpProxyGet(proxyIp, "http://myip.top")
	fmt.Println("httpproxyTest:", str)
}


