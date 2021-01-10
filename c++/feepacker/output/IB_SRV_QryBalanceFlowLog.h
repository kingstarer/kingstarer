#ifndef __IB_SRV_QRYBALANCEFLOWLOG_H__
#define __IB_SRV_QRYBALANCEFLOWLOG_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QryBalanceFlowLog.h"

__USING_WILLOW_NS

	
//## ���������ʷ����Ŀ�����־����ѯ�ӿ�
class CIB_SRV_QryBalanceFlowLog:public FEE_SRV_BASE
{
public:
    CIB_SRV_QryBalanceFlowLog()
    {
        m_errPrefix += "IB_SRV_QryBalanceFlowLog" + m_errSuffix;
    }

public:

	bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
	bool ParseSrvMsg(HSendMsg *sendmsg, QryBalanceFlowLogInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QryBalanceFlowLogInfoOut& data);
    bool ParseArrayToNode(HDataNode *node, const vector<ALog>& nodeinfo);
    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);

public:
	static char *ClassName(void);
};
#endif /* __IB_SRV_QRYBALANCEFLOWLOG_H__ */

