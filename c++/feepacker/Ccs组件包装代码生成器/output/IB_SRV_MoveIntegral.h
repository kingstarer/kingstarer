#ifndef __IB_SRV_MOVEINTEGRAL_H__
#define __IB_SRV_MOVEINTEGRAL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_MoveIntegral.h"

__USING_WILLOW_NS

    
//## ����ת��
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
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, MoveIntegralInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, MoveIntegralInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_MoveIntegral m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_MOVEINTEGRAL_H__ */

