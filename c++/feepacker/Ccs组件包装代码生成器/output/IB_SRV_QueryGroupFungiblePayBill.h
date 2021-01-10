#ifndef __IB_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__
#define __IB_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QueryGroupFungiblePayBill.h"

__USING_WILLOW_NS

    
//## 集团代付帐单查询
class CIB_SRV_QueryGroupFungiblePayBill:public FEE_SRV_BASE
{
public:
    CIB_SRV_QueryGroupFungiblePayBill()
    {
        m_errPrefix += "IB_SRV_QueryGroupFungiblePayBill" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, QueryGroupFungiblePayBillInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QueryGroupFungiblePayBillInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_QueryGroupFungiblePayBill m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__ */

