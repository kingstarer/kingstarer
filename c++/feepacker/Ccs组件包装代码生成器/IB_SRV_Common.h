#ifndef __CC_SRV_<COMMON>_H__
#define __CC_SRV_<COMMON>_H__

#include "FeeAgentCommon.h"
#include "CC_COM_<Common>.h"

__USING_WILLOW_NS

	
//## <组件名称>
class CCC_SRV_<Common>:public FEE_SRV_BASE
{
public:
    CCC_SRV_<Common>()
    {
        m_errPrefix += "CC_SRV_<Common>" + m_errSuffix;
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
    
    CC_COM_<Common> m_cicsOpr;
public:
	static char *ClassName(void);
};
#endif /* __CC_SRV_<COMMON>_H__ */

