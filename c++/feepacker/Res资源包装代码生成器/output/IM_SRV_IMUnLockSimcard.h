#ifndef __IM_SRV_IMUNLOCKSIMCARD_H__
#define __IM_SRV_IMUNLOCKSIMCARD_H__

#include "ResAgentCommon.h"
#include "IM_COM_IMUnLockSimcard.h"

__USING_WILLOW_NS

    
//## SIM卡锁定释放
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
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMUnLockSimcardInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMUnLockSimcardInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMUnLockSimcard m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMUNLOCKSIMCARD_H__ */

