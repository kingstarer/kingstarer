#ifndef __IB_SRV_QRYSERVINTEGRAL_H__
#define __IB_SRV_QRYSERVINTEGRAL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QryServIntegral.h"

__USING_WILLOW_NS

    
//## 查客户积分信息
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
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, QryServIntegralInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QryServIntegralInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_QryServIntegral m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_QRYSERVINTEGRAL_H__ */

