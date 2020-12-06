package sqlopr

import (
	"fmt"
	"unsafe"
)

/**
* 将sql里面注释和字符串，用空格替换
*/
func TrimSqlString(sql string) string {
	var result string
	byteSql := TrimSqlByteArray(Str2bytesUnsafe(sql), 0, len(sql))
	result = Bytes2strUnsafe(byteSql)
	return result;
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


/**
*获取字节前面二进制1个数
*/
func getPreNotZeroCount(ch byte) int {
	i := 0;

	for ch & 0x80 != 0 {
		i++
		ch = ch << 1
	}

	return i
}

func getCharSafe(s []byte, rangeEnd int, pos int) byte  {
	if pos < rangeEnd {
		return s[pos]
	} else {
		return 0
	}
}

const (
	NORMAL = 0
	MULTI = 1
	LINE = 2
	STRING = 3
)


/**
* 获取字符串或者注释的右边界位置（不包含）
* rangeEnd是数组边界
 */
func seekToNext(sql []byte, begPos int, rangeEnd int, charType int) int {
	i := begPos;

	switch charType {
	case MULTI:
		for ; i < rangeEnd; i++ {
			ch := sql[i]
			chNext := getCharSafe(sql, rangeEnd, i+ 1)

			if ch == '*' && chNext == '/' {
				i = i + 1;
				break;
			}
		}

		break
	case LINE:
		for ; i < rangeEnd; i++ {
			ch := sql[i]

			if ch == '\n' {
				break;
			}
		}

		break
	case STRING:
		for ; i < rangeEnd; i++ {
			ch := sql[i]
			chNext := getCharSafe(sql, rangeEnd, i+ 1)

			//sql字符串里面连续的单引号被认为是' 则不是字符串结束标志
			if ch == '\'' && chNext == '\'' {
				i = i + 1;
				continue;
			} else if ch == '\'' {
				break;
			}
		}

		break
	default:
		break;
	}

	var result int
	//一直找到字符串末尾还没有找到结束符的情况
	if i == rangeEnd {
		result = i;
	} else {
		//返回结束符下一个位置
		result = i + 1
	}

	return result;
}


/**
* 将字节数组里面注释和字符串，用空格替换 rangeBeg和rangeEnd是数组元素起始位置 左闭右开
*/
func TrimSqlByteArray(sql []byte, rangeBeg int, rangeEnd int) []byte {
	sqlLength := rangeEnd - rangeBeg - 1;
	//删除注释或者字符串后 用空格填充 必免因删除导致粘连改变sql语义
	const chPad = ' '

	//结果切片，预分配空间为入参sql长度一半
	result := make([] byte, 0, sqlLength / 2)

	//本字符类型
	var charType int = NORMAL;
	for i := rangeBeg; i < rangeEnd; i++ {
		/*
		*utf8编码不影响判断
		//跳过非英文字符
		if sql[i] & 0x80 != 0 {
			//utf8编码：UTF-8是一种变长字节编码方式。对于某一个字符的UTF-8编码，如果只有一个字节则其最高二进制位为0；
			//如果是多字节，其第一个字节从最高位开始，连续的二进制位值为1的个数决定了其编码的位数，其余各字节均以10开头。
			//UTF-8最多可用到6个字节。 这里不考虑异常，因为go的字符串基本都是标准utf8编码
			i += getPreNotZeroCount(sql[i]) - 1
			continue;
		}
		*/

		//本字符类型 预设为普通字符
		charType = NORMAL
		ch := sql[i]

		//下一个字符
		var chNext byte;
		chNext = getCharSafe(sql, rangeEnd, i + 1)

		//非有效sql内容结束位置
		endPos := 0

		if ch == '-' && chNext == '-' {
			//单行注释
			charType = LINE
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 2, rangeEnd, charType)
		} else if ch == '/' && chNext == '*' {
			//多行注释
			charType = MULTI
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 2, rangeEnd, charType)
		} else if ch == '\'' {
			//字符串
			charType = STRING
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 1, rangeEnd, charType)
		}

		//如果字符是非有效字符 则用空格代替 否则保持原样
		if charType == NORMAL {
			result = append(result, ch)
		} else {
			result = append(result, chPad)
			i = endPos - 1
		}
	}

	return result;
}

type SqlExpr struct {
	Type int
	Expr string
}


func (t *SqlExpr) String() string {
	var typeName string
	if t.Type == NORMAL {
		typeName = "NORMAL"
	} else {
		typeName = "OTHER"
	}

	return fmt.Sprintf("{Type:%s, Expr:%s}\n", typeName, t.Expr)
}


func getSqlExpr(charType int, sql []byte, rangeBeg int, rangeEnd int) *SqlExpr {
	result := new (SqlExpr)
	result.Type = charType;
	result.Expr = string (sql[rangeBeg : rangeEnd])
	return result
}

/**
* 拆分字节数组里面有效内容，注释和字符串 注意如果rangeBeg == rangeEnd 返回的是空数组
 */
func ParseSql(sql []byte, rangeBeg int, rangeEnd int) []*SqlExpr {
	//结果切片，预分配空间8
	const capacity = 8
	result := make([] *SqlExpr, 0, capacity)

	//本字符类型
	var charType int = NORMAL;
	//标记首个有效字符起始位置
	begPos := rangeBeg

	for i := rangeBeg; i < rangeEnd; i++ {
		//下一个字符
		var chNext byte;
		//本字符类型 预设为普通字符
		charType = NORMAL
		endPos := 0
		ch := sql[i]
		chNext = getCharSafe(sql, rangeEnd, i + 1)

		if ch == '-' && chNext == '-' {
			//单行注释
			charType = LINE
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 2, rangeEnd, charType)
		} else if ch == '/' && chNext == '*' {
			//多行注释
			charType = MULTI
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 2, rangeEnd, charType)
		} else if ch == '\'' {
			//字符串
			charType = STRING
			//下标移到非有效字符的最后
			endPos = seekToNext(sql, i + 1, rangeEnd, charType)
		}

		//如果字符是非有效字符 则用空格代替 否则保持原样
		if charType == NORMAL {
			continue
		} else {
			//把非有效字符之前的部分截取出来添加到返回列表
			if i > begPos {
				result = append(result, getSqlExpr(NORMAL, sql, begPos, i))
				begPos = i
			}

			//把非有效部分截取出来 添加到返回列表
			result = append(result, getSqlExpr(charType, sql, i, endPos))
			begPos = endPos
			i = endPos - 1
		}
	}

	//如果语句是以有效内容结尾 则需要把起始位置到结尾部分截取出来 添加到返回列表
	if charType == NORMAL && rangeEnd > begPos {
		result = append(result, getSqlExpr(NORMAL, sql, begPos, rangeEnd))
	}

	return result;
}
