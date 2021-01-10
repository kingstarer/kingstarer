#ifndef __IM_SRV_IMLOCKSIMCARD_H__
#define __IM_SRV_IMLOCKSIMCARD_H__

#include "ResAgentCommon.h"
#include "IM_COM_IMLockSimcard.h"

__USING_WILLOW_NS

    
//## SIM������
class CIM_SRV_IMLockSimcard:public RES_SRV_BASE
{
public:
    CIM_SRV_IMLockSimcard()
    {
        m_errPrefix += "IM_SRV_IMLockSimcard" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMLockSimcardInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMLockSimcardInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMLockSimcard m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMLOCKSIMCARD_H__ */

