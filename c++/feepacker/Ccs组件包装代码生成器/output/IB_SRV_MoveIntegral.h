#ifndef __IB_SRV_MOVEINTEGRAL_H__
#define __IB_SRV_MOVEINTEGRAL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_MoveIntegral.h"

__USING_WILLOW_NS

    
//## 积分转移
class CIB_SRV_MoveIntegral:public FEE_SRV_BASE
{
public:
    CIB_SRV_MoveIntegral()
    {
        m_errPrefix += "IB_SRV_MoveIntegral" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, MoveIntegralInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, MoveIntegralInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_MoveIntegral m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_MOVEINTEGRAL_H__ */

