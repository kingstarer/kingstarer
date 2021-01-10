#ifndef __IB_SRV_QUERYBILL_H__
#define __IB_SRV_QUERYBILL_H__

#include "FeeAgentCommon.h"
#include "IB_COM_QueryBill.h"

__USING_WILLOW_NS

    
//## ���ʵ��ӿ�
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
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, QueryBillInfo& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, QueryBillInfoOut& data);

    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);
    
    IB_COM_QueryBill m_cicsOpr;
public:
    static char *ClassName(void);
};
#endif /* __IB_SRV_QUERYBILL_H__ */

