#ifndef __IB_SRV_QRYSERVINTEGRAL_H__
#define __IB_SRV_QRYSERVINTEGRAL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QryServIntegral.h"

__USING_WILLOW_NS

    
//## ��ͻ�������Ϣ
class CIB_SRV_QryServIntegral:public FEE_SRV_BASE
{
public:
    CIB_SRV_QryServIntegral()
    {
        m_errPrefix += "IB_SRV_QryServIntegral" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, QryServIntegralInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QryServIntegralInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_QryServIntegral m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_QRYSERVINTEGRAL_H__ */

