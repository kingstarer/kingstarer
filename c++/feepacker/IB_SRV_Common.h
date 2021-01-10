#ifndef __IB_SRV_<COMMON>_H__
#define __IB_SRV_<COMMON>_H__

#include "FeeAgentCommon.h"
#include "IB_COM_<Common>.h"

__USING_WILLOW_NS

	
//## <组件名称>
class CIB_SRV_<Common>:public FEE_SRV_BASE
{
public:
    CIB_SRV_<Common>()
    {
        m_errPrefix += "IB_SRV_<Common>" + m_errSuffix;
    }

public:

	bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //设置服务名  process函数需要
    virtual void setBusinessName();
	bool ParseSrvMsg(HSendMsg *sendmsg, <inparam>& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, <outparam>& data);

    
    //需要特别的错误报文
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_<Common> m_cicsOpr;
public:
	static char *ClassName(void);
};
#endif /* __IB_SRV_<COMMON>_H__ */

