package htcomm

import (
	"errors"
	//"fmt"
	"os"
	"unsafe"
)


//将字符串中的${env_name}转换成对应的环境变量值
//'${'到'}'之间的字符会做为env_name 如果解析出错会返回nil和错误原因，例如出现'${'但没有'}'
//支持\逃逸符，\$会被解释成$，\\会被解释成\
func ChangeEnvBytesToBytes(bufIn []byte) (bufOut []byte, err error){
	//bufIn := []byte(strIn)
	bufOut = make([]byte, 0, len(bufIn))

	//lastIdx标记上次未拷贝进bufOut的字符位置
	var i, lenIn, lastIdx int
	lenIn = len(bufIn)

	for i = 0; i < lenIn; i++ {
		switch bufIn[i] {
		case '$':
			//只处理${的情况，如果只是单纯的$不处理
			if i + 1 < lenIn && bufIn[i + 1] == '{' {
				//碰到变量开始符号$，首先把上个变量位置到这个字符之间的内容追加到bufOut
				bufOut = append(bufOut, bufIn[lastIdx : i]...)
				//再把变量解析结果追加到bufOut 返回后i指向}字符位置
				i, bufOut, err = appendEnv(bufOut, bufIn, i)

				//一般只有${}不完整时会报错
				if err != nil {
					return
				}

				//标记下一个需要拷进bufOut的字符位置
				lastIdx = i + 1
			}
			break;
		case '\\':
			if lenIn == i + 1 {
				continue
			}

			if bufIn[i + 1] == '$' {
				bufOut = append(bufOut, bufIn[lastIdx : i]...)
				i++
				lastIdx = i
			} else if bufIn[i + 1] == '\\' {
				bufOut = append(bufOut, bufIn[lastIdx : i + 1]...)
				i++
				lastIdx = i + 1
			}
			break;
		}
	}

	//把最后没有发现变量的字符串内容追加到bufOut
	if (lastIdx < i) {
		bufOut = append(bufOut, bufIn[lastIdx : i]...)
	}

	return bufOut, nil
}


//将字符串中的${env_name}转换成对应的环境变量值
//'${'到'}'之间的字符会做为env_name 如果解析出错会返回nil和错误原因，例如出现'${'但没有'}'
//支持\逃逸符，\$会被解释成$，\\会被解释成\
func ChangeEnvToBytes(str string) (bufOut []byte, err error){
	return ChangeEnvBytesToBytes(Str2bytesUnsafe(str))
}


//go语言Must开头的函数，如果调用失败直接抛异常。
func MustChangeEnvToString(bufIn string) (strOut string){
	bufOut, err := ChangeEnvToBytes(bufIn)
	if (err != nil) {
		panic(err)
	}

	strOut = Bytes2strUnsafe(bufOut)
	return
}

func MustChangeEnvToBytes(bufIn string) (bufOut []byte){
	bufOut, err := ChangeEnvToBytes(bufIn)
	if (err != nil) {
		panic(err)
	}

	return
}


func MustChangeEnvBytesToBytes(bufIn []byte) (bufOut []byte){
	bufOut, err := ChangeEnvBytesToBytes(bufIn)
	if (err != nil) {
		panic(err)
	}

	return
}

//把${env_name}转换成对应的环境变量值 并追加到bufOut idx指标变量开始符$的位置
func appendEnv(bufOut []byte, bufIn []byte, idxIn int) (int, []byte, error) {
	bufInLen := len(bufIn)
	//检查$字符后面是否跟着{
	if (bufInLen == idxIn + 1) || bufIn[idxIn + 1] != '{' {
		return 0, nil, errors.New("$后面没有{")
	}

	//把idxIn指向${后面的字符
	idxIn += 2

	//从该字符开始找}，如果找不到则报错
	var idxOut int
	for idxOut = idxIn; idxOut < bufInLen; idxOut++ {
		if bufIn[idxOut] == '}' {
			break
		}
	}

	if idxOut == bufInLen {
		return  0, nil, errors.New("${后面没有}")
	}

	//取出${到}之间的字符做为变量名

	//如果变量名称是空的，则当变量值为空处理
	if idxIn == idxOut {
		return idxOut, bufOut, nil
	}

	//取出idx + 2 ~ idxOut - 1之间的字符做为变量名 从环境变量取出它的值追加到bufOut
	envName := string(bufIn[idxIn : idxOut])
	//fmt.Print("envName", envName)
	bufOut = append(bufOut, [] byte(os.Getenv(envName))...)
	return idxOut, bufOut, nil
}


/*高效的string和byte切片互转函数，但是依赖于go底层实现，不安全*/
/*来源：https://blog.csdn.net/gochenguowei/article/details/100904453*/
func Bytes2strUnsafe(b []byte) string {
	return *(*string)(unsafe.Pointer(&b))
}

func Str2bytesUnsafe(s string) []byte {
	x := (*[2]uintptr)(unsafe.Pointer(&s)) // 获取s的起始地址开始后的两个 uintptr 指针
	h := [3]uintptr{x[0], x[1], x[1]}      // 构造三个指针数组
	return *(*[]byte)(unsafe.Pointer(&h))
}

