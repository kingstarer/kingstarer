#ifndef __IB_SRV_INTEGRATIONCALFEE_H__
#define __IB_SRV_INTEGRATIONCALFEE_H__

#include "FeeAgentCommon.h"
#include "IB_COM_IntegrationCalFee.h"

__USING_WILLOW_NS

    
//## 智能网算费接口
class CIB_SRV_IntegrationCalFee:public FEE_SRV_BASE
{
public:
    CIB_SRV_IntegrationCalFee()
    {
        m_errPrefix += "IB_SRV_IntegrationCalFee" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IntegrationCalFeeInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IntegrationCalFeeInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_IntegrationCalFee m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_INTEGRATIONCALFEE_H__ */

