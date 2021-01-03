
package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
)

//sock5代理
func socksproxy() {
	urli := url.URL{}
	//设置一个http代理服务器格式
	urlproxy, _ := urli.Parse("http://测试ip:端口")
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
	response, _ := client.Do(rqt)
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
	urlproxy, _ := urli.Parse("http://测试ip:端口")
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
	response, _ := client.Do(rqt)
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

//本机IP
func httplocal() {
	//设置一个http客户端
	client := &http.Client{}
	//访问地址http://myip.top
	rqt, err := http.NewRequest("GET", "http://myip.top", nil)
	if err != nil {
		println("接口获取IP失败!")
		return
	}
	//添加一个识别信息
	rqt.Header.Add("User-Agent", "Lingjiang")
	//处理返回结果
	response, _ := client.Do(rqt)
	defer response.Body.Close()
	//读取内容
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		return
	}
	//显示获取到的IP地址
	fmt.Println("本机:", string(body))
	return

}

func main() {
	//本机不使用代理访问方法
	httplocal()
	//使用http代理访问方法
	httpproxy()
	//使用socks5代理访问方法
	socksproxy()
}


