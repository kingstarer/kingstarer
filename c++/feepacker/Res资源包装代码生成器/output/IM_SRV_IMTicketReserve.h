#ifndef __IM_SRV_IMTICKETRESERVE_H__
#define __IM_SRV_IMTICKETRESERVE_H__

#include "ResAgentCommon.h"
#include "IM_COM_IMTicketReserve.h"

__USING_WILLOW_NS

    
//## 充值券包资源预定
class CIM_SRV_IMTicketReserve:public RES_SRV_BASE
{
public:
    CIM_SRV_IMTicketReserve()
    {
        m_errPrefix += "IM_SRV_IMTicketReserve" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMTicketReserveInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMTicketReserveInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMTicketReserve m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMTICKETRESERVE_H__ */

