#ifndef __IB_SRV_QUERYBILL_H__
#define __IB_SRV_QUERYBILL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QueryBill.h"

__USING_WILLOW_NS

    
//## 查帐单接口
class CIB_SRV_QueryBill:public FEE_SRV_BASE
{
public:
    CIB_SRV_QueryBill()
    {
        m_errPrefix += "IB_SRV_QueryBill" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, QueryBillInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QueryBillInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_QueryBill m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_QUERYBILL_H__ */

