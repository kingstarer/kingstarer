#ifndef __IB_COM_QRYBALANCEFLOWLOG_H__
#define __IB_COM_QRYBALANCEFLOWLOG_H__
#include "IB_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_QryBalanceFlowLogInfo
{
	char Accoid[15]; //帐号
	char StartTime[15]; //开始时间 [ Yyyymmddhh24miss ]
	char EndTime[15]; //结束时间 [ Yyyymmddhh24miss ]
	char qryType[2]; //查询类型 [ 0全部，1只要余额增加 ]
} QryBalanceFlowLogInfo;


typedef struct tag_ALog
{
	char SubjectId[33]; //帐目（科目）代码
	char StartCycle[7]; //科目开始周期
	char EndCycle[7]; //科目结束周期
	char FlowTime[15]; //流动时间 [ Yyyymmddhh24miss ]
	char FlowDirection[2]; //流动方向 [ 0流入，1流出 ]
	char FlowType[33]; //流动类型 [ 传空串 ]
	char FlowTypeName[65]; //变更原因
	char FlowAmt[15]; //流动金额
	char FlowNoBalance[15]; //流动后剩余
	char OperID[11]; //发起人工号
	char Formnum[15]; //流水号 [ 从营业发起的缴费流水号，如果是其他（例如划扣，托收）为空串 ]
	char Source[9]; //来源 [ 空中充值填写代理商，如连连科技写LL，其他变更原因置空 ]
} ALog;

typedef struct tag_QryBalanceFlowLogInfoOut
{
	vector<ALog> ALogArray; //流动信息数组
	char ErrNo[9]; //返回码 [ 0――成功；其他――失败 ]
	char ErrInfo[257]; //返回错误信息
} QryBalanceFlowLogInfoOut;


	
/*余额流动历史（帐目变更日志）查询接口*/
class IB_COM_QryBalanceFlowLog:public IB_COM_BASE
{
public:
    IB_COM_QryBalanceFlowLog()
    {
        m_errPrefix += "IB_COM_QryBalanceFlowLog" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //ErrNo 0-成功 1-没有符合条件的记录 其他-失败
    bool IB_QryBalanceFlowLog( const QryBalanceFlowLogInfo& data, QryBalanceFlowLogInfoOut& res );
private:
    bool setCallInfo(HRecvMsg *send, const QryBalanceFlowLogInfo& data);
    bool getCallResult(HSendMsg *rcv, QryBalanceFlowLogInfoOut& res);
    bool getHDataNodeResult(vector<ALog>& nodeinfo, IN HDataNode* node, const string& destname);
};
#endif /*__IB_COM_QRYBALANCEFLOWLOG_H__*/
