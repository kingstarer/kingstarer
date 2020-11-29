package sqlopr

import (
	"testing"
)

func assertEqual(sql, expect, actuality string, t *testing.T)  {
	if expect != actuality {
		t.Errorf("sql = [%s]", sql)
		t.Errorf("expect [%s] get [%s]\n", expect, actuality)
	}
}

func TestTrimSqlString(t *testing.T) {
	var sql, sqlTrim, sqlExpect string

	sql = "/*字符串起始注释*/ select---node\n" + "*, /*hint*/ 'str' str, 'str1''' str1, 1 + 1, 1/**/a from --\n \ndual/*结束*/;";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = "  select *,     str,   str1, 1 + 1, 1 a from   \ndual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = ""
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/select * from dual*/;";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = "/select * from dual*/;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/**/select * from dual/**/;";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " select * from dual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/*b1.xx*//**/select '' || '' dual.test, from dual/**/;";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = "  select   ||   dual.test, from dual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/**/";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/*";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "--";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "--\na";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " a"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "''";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "''''";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)


	sql = "''''--\n''/**/";
	sqlTrim = TrimSqlString(sql);
	sqlExpect = "    "
	assertEqual(sql, sqlExpect, sqlTrim, t)

}

func myTrimSql(sqlExprs [] *SqlExpr) string {
	result := ""

	for _, val := range(sqlExprs) {
		if val.Type == NORMAL {
			result += val.Expr
		} else {
			result += " "
		}
	}

	return result
}

func TestParseSql(t *testing.T) {

	var sql, sqlTrim, sqlExpect string
	var sqlExprArray []* SqlExpr

	sql = "/*字符串起始注释*/ select---node\n" + "*, /*hint*/ 'str' str, 'str1''' str1, 1 + 1, 1/**/a from --\n \ndual/*结束*/;";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = "  select *,     str,   str1, 1 + 1, 1 a from   \ndual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)


	sql = "/*字符串起始注释*/ select---node\n" + "*, /*hint*/ 'str' str, 'str1''' str1, 1 + 1, 1/**/a from --\n \ndual/*结束*/;";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = "  select *,     str,   str1, 1 + 1, 1 a from   \ndual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = ""
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/select * from dual*/;";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = "/select * from dual*/;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/**/select * from dual/**/;";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " select * from dual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/*b1.xx*//**/select '' || '' dual.test, from dual/**/;";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = "  select   ||   dual.test, from dual ;"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/**/";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "/*";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "--";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "--\na";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " a"
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "''";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)

	sql = "''''";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = " "
	assertEqual(sql, sqlExpect, sqlTrim, t)


	sql = "''''--\n''/**/";
	sqlExprArray = ParseSql([] byte(sql), 0, len(sql));
	sqlTrim = myTrimSql(sqlExprArray)
	sqlExpect = "    "
	assertEqual(sql, sqlExpect, sqlTrim, t)
}
