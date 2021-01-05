package htcomm

import (
	"bytes"
	"os"
	"testing"
)

func TestChangeEnvToBufNoVar(t *testing.T) {
	str := "test"
	buf, err := ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(str))!= 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = ""
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(str))!= 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "1111111111111"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(str)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}


	str = "afsf{}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(str)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\$`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte("$")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\$\a`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`$\a`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\\\${`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`\${`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\{\$\\${}`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`\{$\`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte("\\")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\\a`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`\a`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\\`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`\`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "\\${test}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte("${test}")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `\${test}\\${}\$$`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`${test}\$$`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `$`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`$`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `$\`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`$\`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `$\\`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`$\`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = `$\$`
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	//fmt.Println("buf len is ", len(buf))
	if bytes.Compare(buf, [] byte(`$$`)) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}
}

func TestChangeEnvToBufHaveNullVar(t *testing.T) {
	var str string
	var err error
	var buf []byte

	str = "${test_no_var}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "${test_no_var}$"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	str = "${test_no_var}${"
	buf, err = ChangeEnvToBytes(str)
	if err == nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	str = "${test_no_var${}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "${test_no_var}${}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

}



func TestChangeEnvToBufHaveVar(t *testing.T) {
	var str string
	var err error
	var buf []byte

	os.Setenv("test_no_var", "test_no_var")
	str = "${test_no_var}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("test_no_var")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "${test_no_var}\\$"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("test_no_var$")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	str = "${test_no_var}${}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("test_no_var")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	os.Setenv("test_no_var1", "test")
	str = "${test_no_var}${test_no_var1}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("test_no_vartest")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	os.Setenv("test_no_var", "")
	str = "${test_no_var}${}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}

	os.Setenv("test_no_var", "")
	str = "${test_no_var}${test_no_var1}"
	buf, err = ChangeEnvToBytes(str)
	if err != nil {
		t.Error("ChangeEnvToBytes err:", str, err)
	}

	if bytes.Compare(buf, [] byte("test")) != 0 {
		t.Errorf("ChangeEnvToBytes fail: %s vs %s", str, buf)
	}
}
