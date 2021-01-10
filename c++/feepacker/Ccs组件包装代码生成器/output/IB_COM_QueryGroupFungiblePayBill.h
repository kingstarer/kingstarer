#ifndef __IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#define __IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#include "IB_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_QueryGroupFungiblePayBillInfo
{
    char Region[7]; //��������
    char OrgID[33]; //��λ����
    char OperID[33]; //����Ա����
    char ResType[33]; //��Ʒ���
    char ResSerial[33]; //��Ʒ���
} QueryGroupFungiblePayBillInfo;


typedef struct tag_CustMemberBill
{
    char ServNum[33]; //�����Ĳ�Ʒ����
    char Name[65]; //�����Ĳ�Ʒ����
    char FungibleRate[65]; //�������
    char RealPayFee[15]; //ʵ�ʴ������
} CustMemberBill;

typedef struct tag_BillAccount
{
    char GroupID[15]; //���ű��� [ �˲������������ֵͬ ]
    char Cycle[7]; //��������
    char Totalfee[15]; //�ܷ��ã����з�����ĺϼƣ� [ CustMemberBill �и�����õĺϼ� ]
    vector<CustMemberBill> CustMemberBillArray; //������ϸ
} BillAccount;

typedef struct tag_QueryGroupFungiblePayBillInfoOut
{
    vector<BillAccount> BillAccountArray; //�����ʵ�����
    char SourceNo[15]; //��ѯ��ˮ�� [ ���ֶ�ֻ���������ֶ�Flag��Ϊ��ʱ������ ]
    char ErrNo[9]; //������ [ 0�D�D�ɹ��� -1�D�Dʧ�� 1000����ʼ�첽��ѯ 1001���첽�ȴ� ]
    char ErrInfo[257]; //���ش�����Ϣ
} QueryGroupFungiblePayBillInfoOut;


    
/*���Ŵ����ʵ���ѯ*/
class IB_COM_QueryGroupFungiblePayBill:public IB_COM_BASE
{
public:
    IB_COM_QueryGroupFungiblePayBill()
    {
        m_errPrefix += "IB_COM_QueryGroupFungiblePayBill" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //ErrNo 0-�ɹ� 1-û�з��������ļ�¼ ����-ʧ��
    bool IB_QueryGroupFungiblePayBill( const QueryGroupFungiblePayBillInfo& data, QueryGroupFungiblePayBillInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<BillAccount>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<CustMemberBill>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<BillAccount>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<CustMemberBill>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QueryGroupFungiblePayBillInfo& data);
    bool getCallResult(HSendMsg *rcv, QueryGroupFungiblePayBillInfoOut& res);
    


};
#endif /*__IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__*/

