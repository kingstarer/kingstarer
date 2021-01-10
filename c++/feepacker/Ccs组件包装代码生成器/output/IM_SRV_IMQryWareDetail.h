#ifndef __IM_SRV_IMQRYWAREDETAIL_H__
#define __IM_SRV_IMQRYWAREDETAIL_H__

#include "FeeAgentCommon.h"
#include "IM_COM_IMQryWareDetail.h"

__USING_WILLOW_NS

    
//## ���Ŵ����ʵ���ѯ
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
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, IMQryWareDetailInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, IMQryWareDetailInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IM_COM_IMQryWareDetail m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_IMQRYWAREDETAIL_H__ */

