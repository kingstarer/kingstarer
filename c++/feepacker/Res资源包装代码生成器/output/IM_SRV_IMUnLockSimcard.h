#ifndef __IM_SRV_IMUNLOCKSIMCARD_H__
#define __IM_SRV_IMUNLOCKSIMCARD_H__

#include "ResAgentCommon.h"
#include "IM_COM_IMUnLockSimcard.h"

__USING_WILLOW_NS

    
//## SIM�������ͷ�
class CIM_SRV_IMUnLockSimcard:public RES_SRV_BASE
{
public:
    CIM_SRV_IMUnLockSimcard()
    {
        m_errPrefix += "IM_SRV_IMUnLockSimcard" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMUnLockSimcardInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMUnLockSimcardInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMUnLockSimcard m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMUNLOCKSIMCARD_H__ */

