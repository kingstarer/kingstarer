#ifndef __IM_SRV_IMQRYWAREDETAIL_H__
#define __IM_SRV_IMQRYWAREDETAIL_H__

#include "FeeAgentCommon.h"
#include "IM_COM_IMQryWareDetail.h"

__USING_WILLOW_NS

    
//## 集团代付帐单查询
class CIM_SRV_IMQryWareDetail:public FEE_SRV_BASE
{
public:
    CIM_SRV_IMQryWareDetail()
    {
        m_errPrefix += "IM_SRV_IMQryWareDetail" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMQryWareDetailInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMQryWareDetailInfoOut& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMQryWareDetail m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMQRYWAREDETAIL_H__ */

