
package main

import (
	"fmt"
	"getHttpInfo/config"
	"github.com/htgx/htcomm"
	log "github.com/htgx/htcomm/logger"
	"io/ioutil"
	"net/http"
	"net/url"
	"regexp"
)

//sock5代理
func socksproxy() {
	urli := url.URL{}
	//设置一个http代理服务器格式
	urlproxy, _ := urli.Parse("http://8.210.11.115:13746")
	//设置一个http客户端
	client := &http.Client{
		Transport: &http.Transport{//设置代理服务器
			Proxy: http.ProxyURL(urlproxy),
		},
	}
	//访问地址http://myip.top
	rqt, err := http.NewRequest("GET", "http://myip.top", nil)
	if err != nil {
		println("接口获取IP失败!")
		return
	}
	//添加一个识别信息
	rqt.Header.Add("User-Agent", "Lingjiang")
	//处理返回结果
	response, err := client.Do(rqt)

	if err != nil {
		fmt.Println("socksproxy 请求失败 err:", err)
		fmt.Println("response:", response)
		return
	}

	defer response.Body.Close()
	//读取内容
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		return
	}
	//显示获取到的IP地址
	fmt.Println("socks5:", string(body))
	return

}

//http代理
func httpproxy() {
	urli := url.URL{}
	//设置一个http代理服务器格式
	urlproxy, _ := urli.Parse("http://8.210.11.115:13746")
	//设置一个http客户端
	client := &http.Client{
		Transport: &http.Transport{//设置代理服务器
			Proxy: http.ProxyURL(urlproxy),
		},
	}

	//访问地址http://myip.top
	rqt, err := http.NewRequest("GET", "http://myip.top", nil)
	if err != nil {
		println("接口获取IP失败!")
		return
	}
	//添加一个识别信息
	rqt.Header.Add("User-Agent", "Lingjiang")
	//处理返回结果
	response, err := client.Do(rqt)

	if err != nil {
		fmt.Println("httpproxy 请求失败 err:", err)
		fmt.Println("response:", response)
		return
	}

	defer response.Body.Close()
	//读取内容
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		return
	}
	//显示获取到的IP地址
	fmt.Println("http:", string(body))
	return

}



//http代理
func httpproxyTest(ipAndPort string, getPath string) (string, error) {
	var data string

	urli := url.URL{}
	//设置一个http代理服务器格式
	agentUrl := fmt.Sprintf("http://%s", ipAndPort)
	urlproxy, _ := urli.Parse(agentUrl)
	//设置一个http客户端
	client := &http.Client{
		Transport: &http.Transport{ //设置代理服务器
			Proxy: http.ProxyURL(urlproxy),
		},
	}
	//访问地址http://myip.top
	rqt, err := http.NewRequest("GET", getPath, nil)
	if err == nil {
		//添加一个识别信息
		rqt.Header.Add("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3833.0 Safari/537.36")
		//处理返回结果
		response, err_rep := client.Do(rqt)
		if err_rep != nil {
			err = err_rep
		} else {
			defer response.Body.Close()
			//读取内容
			body, errs := ioutil.ReadAll(response.Body)
			err = errs
			data = string(body)
		}
	}
	return data, err

}


//本机IP
func HttpGetLocal(url string) (string, error) {
	//设置一个http客户端
	client := &http.Client{}
	//访问地址
	rqt, err := http.NewRequest("GET", url, nil)
	if err != nil {
		log.Error("建立http客户端对象失败", url)
		return "", nil
	}

	//添加一个识别信息
	rqt.Header.Add("User-Agent", "Lingjiang")
	//处理返回结果
	response, err := client.Do(rqt)
	if err != nil {
		log.Error("访问网址失败", url)
		return "", nil
	}

	defer response.Body.Close()
	//读取内容
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		return "", nil
	}
	return htcomm.Bytes2strUnsafe(body), nil
}

//获取本机对外ip地址
func getLocalIp()  (string, error) {
	var result string;
	log.Debug("从远程获取本机ip地址")
	result, err := HttpGetLocal(config.CfgGetHttpInfo.Api.IpApi)
	if err != nil {
		return "", err
	}

	log.Debug("获取本机ip成功", result)
	reg := regexp.MustCompile("\\d+\\.\\d+\\.\\d+\\.\\d+")
	result = reg.FindString(result)

	log.Info("本机ip", result)
	return result, nil
}

var setWhiteFlag = false

//获取本机对外ip地址
func getProxyIp()  (string, error) {
	var result string;

	if !setWhiteFlag {
		log.Debug("第一次获取ip前需要设置白名单")
		localIp, err := getLocalIp()
		if err != nil {
			return "", err
		}

		log.Info("本机对外ip为 ", localIp)
		_, err = HttpGetLocal(config.CfgGetHttpInfo.Api.WhiteApi + localIp)
		if err != nil {
			return "", err
		}

		setWhiteFlag = true
	}


	log.Debug("获取代理服务器地址")
	result, err := HttpGetLocal(config.CfgGetHttpInfo.Api.AddrApi)
	if err != nil {
		return "", err
	}

	log.Debug("获取代理服务器成功", result)

	return result, nil
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

	var localIp string;
	if localIp, err = getLocalIp(); err != nil {
		log.Debug("getLocalIp fail!", err)
		return
	}
	log.Info("本机ip ", localIp)

	proxyIp, err := getProxyIp()
	log.Info("proxyIp ", proxyIp)


	//使用http代理访问方法
	httpproxy()
	//使用socks5代理访问方法
	socksproxy()

	//别人成功的测试
	str, _ := httpproxyTest("8.210.11.115:13746", "http://myip.top")
	fmt.Println("httpproxyTest:", str)
}


