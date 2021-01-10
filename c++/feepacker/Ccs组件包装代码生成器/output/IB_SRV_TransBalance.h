#ifndef __IB_SRV_TRANSBALANCE_H__
#define __IB_SRV_TRANSBALANCE_H__

#include "FeeAgentCommon.h"
#include "IB_COM_TransBalance.h"

__USING_WILLOW_NS

    
//## ���ת��
class CIB_SRV_TransBalance:public FEE_SRV_BASE
{
public:
    CIB_SRV_TransBalance()
    {
        m_errPrefix += "IB_SRV_TransBalance" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, TransBalanceInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, TransBalanceInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_TransBalance m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_TRANSBALANCE_H__ */

