#ifndef __IM_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__
#define __IM_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__

#include "ResAgentCommon.h"
#include "IM_COM_QueryGroupFungiblePayBill.h"

__USING_WILLOW_NS

    
//## ���Ŵ����ʵ���ѯ
class CIM_SRV_QueryGroupFungiblePayBill:public FEE_SRV_BASE
{
public:
    CIM_SRV_QueryGroupFungiblePayBill()
    {
        m_errPrefix += "IM_SRV_QueryGroupFungiblePayBill" + m_errSuffix;
    }

public:

    bool doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg);

private:
    //���÷�����  process������Ҫ
    virtual void setBusinessName();
    bool ParseSrvMsg(HSendMsg *sendmsg, InParam& data);
    bool ParseComMsg(HRecvMsg *rcvmsg, OutParam& data);
    bool ParseArrayToNode(HDataNode *node, const vector<ALog>& nodeinfo);


    
    //��Ҫ�ر�Ĵ�����
    virtual void setErrorXml(HRecvMsg * rcvmsg);

public:
    static char *ClassName(void);
};
#endif /* __IM_SRV_QUERYGROUPFUNGIBLEPAYBILL_H__ */

