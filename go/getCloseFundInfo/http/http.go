package http

import (
	"github.com/htgx/htcomm"
	"github.com/htgx/htcomm/logger"
	"io/ioutil"
	"net/http"
)

//本机IP
func HttpGetLocal(url string) (string, error) {
	//设置一个http客户端
	client := &http.Client{}
	//访问地址
	rqt, err := http.NewRequest("GET", url, nil)
	if err != nil {
		logger.Error("建立http客户端对象失败", url)
		return "", nil
	}

	//添加一个识别信息
	rqt.Header.Add("User-Agent", "Lingjiang")
	//处理返回结果
	response, err := client.Do(rqt)
	if err != nil {
		logger.Error("访问网址失败", url)
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
