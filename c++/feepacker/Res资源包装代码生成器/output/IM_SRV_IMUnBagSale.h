#ifndef __IM_SRV_IMUNBAGSALE_H__
#define __IM_SRV_IMUNBAGSALE_H__

#include "ResAgentCommon.h"
#include "IM_COM_IMUnBagSale.h"

__USING_WILLOW_NS

    
//## 充值券包销售回退
class CIM_SRV_IMUnBagSale:public RES_SRV_BASE
{
public:
    CIM_SRV_IMUnBagSale()
    {
        m_errPrefix += "IM_SRV_IMUnBagSale" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMUnBagSaleInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMUnBagSaleInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMUnBagSale m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMUNBAGSALE_H__ */

