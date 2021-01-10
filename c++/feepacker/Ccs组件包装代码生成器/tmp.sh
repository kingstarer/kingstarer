svn info
Path: .
URL: http://10.200.30.25:8888/svn/NGCRM/SCODE/c++
Repository Root: http://10.200.30.25:8888/svn/NGCRM
Repository UUID: e53d3330-2dac-0c47-9eca-c75bff66f736
Revision: 11356
Node Kind: directory
Schedule: normal
Last Changed Author: huangcihui
Last Changed Rev: 11356
Last Changed Date: 2011-01-17 09:22:13 +0800 (Mon, 17 Jan 2011)


#!/bin/sh
# 用法: svninfo.sh 代码目录  svn版本信息头文件目录

if [ $# -ne 2 ]
then
	echo "usage:"
	echo "$0 codedir svnheaddir"
	exit
fi

cd $1


svn info > svninfo.$$

awk -F ": " 'BEGIN{
	printf("#ifndef __SVNINFO_H__\n");
	printf("#define __SVNINFO_H__\n\n");
	printf("#define SVN_INFO_STR \"\\nsvn info: \\n\" \\");
}
{	
	if ( $0 == "" )
	{
		printf("\b \n");
		next;
	}
	else
	{
		printf("\n");
	}

	printf("\"%s\\n\" \\", $0);
	if ($1 == "Revision")
	{
		revision = $2;
	}
}
END{
	printf("\n\n#define SVN_VISION \"svn_revision: %s\"\n", revision);
	printf("\n#endif /*__SVNINFO_H__*/\n\n");
}' svninfo.$$ > $2/svninfo.h

rm svninfo.$$