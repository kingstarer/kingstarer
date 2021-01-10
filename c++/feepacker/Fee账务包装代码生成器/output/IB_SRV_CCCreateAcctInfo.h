#ifndef __IB_SRV_CCCREATEACCTINFO_H__
#define __IB_SRV_CCCREATEACCTINFO_H__

#include "FeeAgentCommon.h"
#include "IB_COM_CCCreateAcctInfo.h"

__USING_WILLOW_NS

    
//## 销帐日志查询接口
class CIB_SRV_CCCreateAcctInfo:public FEE_SRV_BASE
{
public:
    CIB_SRV_CCCreateAcctInfo()
    {
        m_errPrefix += "IB_SRV_CCCreateAcctInfo" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, CCCreateAcctInfoInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, CCCreateAcctInfoInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_CCCreateAcctInfo m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_CCCREATEACCTINFO_H__ */

