#ifndef __IB_SRV_CCCREATEACCTINFO_H__
#define __IB_SRV_CCCREATEACCTINFO_H__

#include "FeeAgentCommon.h"
#include "IB_COM_CCCreateAcctInfo.h"

__USING_WILLOW_NS

    
//## ������־��ѯ�ӿ�
class CIB_SRV_CCCreateAcctInfo:public FEE_SRV_BASE
{
public:
    CIB_SRV_CCCreateAcctInfo()
    {
        m_errPrefix += "IB_SRV_CCCreateAcctInfo" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, CCCreateAcctInfoInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, CCCreateAcctInfoInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_CCCreateAcctInfo m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_CCCREATEACCTINFO_H__ */

