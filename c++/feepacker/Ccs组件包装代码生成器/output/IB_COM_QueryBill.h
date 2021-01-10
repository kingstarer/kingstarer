#ifndef __IB_COM_QUERYBILL_H__
#define __IB_COM_QUERYBILL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_QueryBillInfo
{
    char acctid[15]; //�ʻ�ID [ �ʻ�ID��Ϊ�գ���Ϊ��ʱsubsid�Ͳ�����Ϊ�� ]
    char subsid[15]; //�û�ID [ �û�ID��Ϊ�գ���Ϊ��ʱacctid�Ͳ�����Ϊ�� ]
    char StartCycle[9]; //��ʼ���� [ ��ΪYYYYMM00���������ʺ���λ�Ͳ���00�� ]
    char EndCycle[9]; //�������� [ ��ΪYYYYMM00���������ʺ���λ�Ͳ���00�� ]
    char Status[2]; //�ʵ�״̬ [ ����,������������ǻ���� ]
    char IsBadDebt[2]; //�Ƿ���ʾ���ʷ��� [ ��2��------�ǣ���ֻ�д��ʷ��ã����ڵ����Ĵ��ʻ��� ]
    char HowBadDebt[2]; //�����Դ����ʷ��� [ ����Ҫ���� ]
    char BillType[33]; //�����ʵ�����ʽ [ 1�D�Dÿ�ʺ�����һ������ֻ�и���ŷ��������ϸ�� ]
    char BillFormatNum[33]; //������ϸ�������ϼ��� [ �ͻ����ʵ�ģ��ţ�custbill_def�ж���֮template_id�� ]
} QueryBillInfo;


typedef struct tag_TipsData
{
    char RealUse[15]; //ʵ��ʹ�� [ ��ǰ����ʹ�õķ��� ]
    char TipsUse[15]; //�ײ��·� [ ʹ��ʡǮ���ŵķ��� ]
} TipsData;

typedef struct tag_SixmonData
{
    char BillCyc[9]; //����
    char Fee[15]; //����
} SixmonData;

typedef struct tag_MonData
{
    char BaseFee[15]; //�»�����
    char IncreFee[15]; //��ֵҵ���
    char ComFee[15]; //ͨ�ŷ�
    char SpFee[15]; //���շ�
    char PaybyFee[15]; //�����˸��� [ �����˸��Ѳ������������ʵ����У��������ָ���ķ������ ]
    char ReceiveFee[15]; //���շ�
    char ViolaFee[15]; //ΥԼ��
    char OtherFee[15]; //����
} MonData;

typedef struct tag_IntDetail
{
    char MonInt[15]; //���»���
    char ConsumInt[15]; //�������ѻ���
    char AwardInt[15]; //���½�������
    char BeforeAvlInt[15]; //����ĩʣ��ɶһ�����
    char AwardedInt[15]; //�����Ѷһ�����
    char DonateInt[15]; //���¾�������
    char LeftInt[15]; //����ĩʣ�����
    char CleanInt[15]; //����������
} IntDetail;

typedef struct tag_BalanceDetail
{
    char BeforeAmt[15]; //���³�����
    char ZSAmt[15]; //����
    char DiscAmt[15]; //�Ż�
    char BackFee[15]; //�˷�
    char PaybyAmt[15]; //���˴���
    char InvalidAmt[15]; //����ʧЧ����
    char DepositAmt[15]; //���³�����
    char ThisMonBack[15]; //���·���
    char ThisMonPay[15]; //�����ۼ��ѽ���
    char Totalfee[15]; //���úϼ�
    char AfterAmt[15]; //����ĩ����
    char ShouldPay[15]; //����ĩӦ��
    char WillBackAmt[15]; //����δ�������
} BalanceDetail;

typedef struct tag_SpDetail
{
    char SpName[65]; //����������
    char BusCode[33]; //�������
    char BusName[65]; //����ҵ������
    char Fee[15]; //���ý��
} SpDetail;

typedef struct tag_FeeDetail
{
    char BillItem[2]; //�Ƿ��ʵ��� [ 1Ϊ�ʵ���Ŀ��0Ϊ���� ]
    char FeeTypeId[33]; //���ô���
    char Feename[65]; //��������
    char Fee[15]; //���ý��
} FeeDetail;

typedef struct tag_Bill
{
    char Accoid[15]; //�ʺ� [ �˲������������ֵͬ��������������Ϊ�գ��Ұ��û���ѯʱ������billtype������1ʱҪ������ʺ� ]
    char Subsoid[15]; //�û��� [ ����Ǹ��û��ʵ��ϲ����ʺ��ʵ�����Ϊ�մ������������BillType��أ� ]
    char Cycle[7]; //��������
    char Totalfee[15]; //�ܷ��ã�������ϸ�ĺϼƣ�
    char ShouldPay[15]; //����Ӧ�� [ ���� ]
    char LeftInt[15]; //����ĩʣ����� [ ���� ]
    char isWrtOff[2]; //���ʱ�ʶ [ 1�Ѿ����ʣ�0û������ ]
    char Paidamt[15]; //�������� [ �����˽�� ]
    char TipsInfo[1001]; //�Ƽ���Ϣ [ ���Ի����ײ��ʷѷ����Ƽ���Ϣ ]
    vector<FeeDetail> FeeDetailArray; //������ϸ
    vector<SpDetail> SpDetailArray; //������Ϣ����ϸ
    vector<BalanceDetail> BalanceDetailArray; //�����ϸ
    vector<IntDetail> IntDetailArray; //������ϸ
    vector<MonData> MonDataArray; //���·��ýṹ����
    vector<SixmonData> SixmonDataArray; //�������������������
    vector<TipsData> TipsDataArray; //ʡǮ������������
} Bill;

typedef struct tag_QueryBillInfoOut
{
    vector<Bill> BillArray; //�ʵ�����
    char ErrNo[9]; //������ [ 0�D�D�ɹ��� �����D�Dʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} QueryBillInfoOut;


    
/*���ʵ��ӿ�*/
class IB_COM_QueryBill:public IB_COM_BASE
{
public:
    IB_COM_QueryBill()
    {
        m_errPrefix += "IB_COM_QueryBill" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_QueryBill( const QueryBillInfo& data, QueryBillInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<Bill>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<FeeDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<SpDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<BalanceDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<IntDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<MonData>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<SixmonData>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<TipsData>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<Bill>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<FeeDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<SpDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<BalanceDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<IntDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<MonData>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<SixmonData>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<TipsData>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QueryBillInfo& data);
    bool getCallResult(HSendMsg *rcv, QueryBillInfoOut& res);
    


};
#endif /*__IB_COM_QUERYBILL_H__*/

