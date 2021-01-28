package proxy

import (
	"fmt"
	"getHttpInfo/config"
	http2 "getHttpInfo/http"
	"github.com/htgx/htcomm/logger"
	"io/ioutil"
	"net/http"
	"net/url"
	"regexp"
)

//sock5代理
func Socksproxy() {
	urli := url.URL{}
	//设置一个http代理服务器格式
	urlproxy, _ := urli.Parse("http://8.210.11.115:13746")
	//设置一个http客户端
	client := &http.Client{
		Transport: &http.Transport{ //设置代理服务器
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
		fmt.Println("Socksproxy 请求失败 err:", err)
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

//http代理 返回状态码 请求结果 错误信息
func HttpProxyGet(ipAndPort string, getPath string) (int, string, error) {
	var data string
	var statusCode int = http.StatusInternalServerError

	urli := url.URL{}
	//设置一个http代理服务器格式
	agentUrl := fmt.Sprintf("http://%s", ipAndPort)
	urlproxy, err := urli.Parse(agentUrl)
	if err != nil {
		logger.Debug("Parse fail %v", err)
		return statusCode, "", err
	}

	//设置一个http客户端
	client := &http.Client{
		Transport: &http.Transport{ //设置代理服务器
			Proxy: http.ProxyURL(urlproxy),
		},
	}
	logger.Debugf("urlproxy: %v getPath = %v", agentUrl, getPath)

	//访问地址http://myip.top
	rqt, err := http.NewRequest("GET", getPath, nil)
	if err == nil {
		//添加一个识别信息
		rqt.Header.Add("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3833.0 Safari/537.36")
		//处理返回结果
		response, err_rep := client.Do(rqt)
		if err_rep != nil {
			logger.Error("client.Do fail")
			err = err_rep
		} else {
			defer response.Body.Close()
			//读取内容
			body, errs := ioutil.ReadAll(response.Body)
			err = errs
			data = string(body)
			statusCode = response.StatusCode
		}
	}

	return statusCode, data, err
}

//获取本机对外ip地址
func GetLocalIp() (string, error) {
	var result string
	var err error

	//测试时发现偶尔获取不到本机对外ip 所以增加重试机制
	for i := 0; i < 3; i++ {
		logger.Debug("从远程获取本机ip地址")
		result, err = http2.HttpGetLocal(config.CfgGetHttpInfo.Api.IpApi)
		if err == nil {
			break
		}
		logger.Error("获取对外ip失败，需要重试")
	}

	if err != nil {
		return "", err
	}

	logger.Debug("获取本机ip成功", result)
	reg := regexp.MustCompile("\\d+\\.\\d+\\.\\d+\\.\\d+")
	result = reg.FindString(result)

	logger.Info("本机ip", result)
	return result, nil
}

var setWhiteFlag = false

//获取本机对外ip地址
func GetProxyIp() (string, error) {
	var result string

	if !setWhiteFlag {
		logger.Debug("第一次获取ip前需要设置白名单")
		localIp, err := GetLocalIp()
		if err != nil {
			return "", err
		}

		logger.Info("本机对外ip为 ", localIp)
		_, err = http2.HttpGetLocal(config.CfgGetHttpInfo.Api.WhiteApi + localIp)
		if err != nil {
			return "", err
		}

		setWhiteFlag = true
	}

	logger.Debug("获取代理服务器地址")
	result, err := http2.HttpGetLocal(config.CfgGetHttpInfo.Api.AddrApi)
	if err != nil {
		return "", err
	}

	logger.Debug("获取代理服务器成功", result)

	reg := regexp.MustCompile("\\d+\\.\\d+\\.\\d+\\.\\d+:\\d+")
	result = reg.FindString(result)

	return result, nil
}
