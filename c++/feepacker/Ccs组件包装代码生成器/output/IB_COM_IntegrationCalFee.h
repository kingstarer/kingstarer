#ifndef __IB_COM_INTEGRATIONCALFEE_H__
#define __IB_COM_INTEGRATIONCALFEE_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_YxPlanList
{
    char YxPlanId[15]; //Ӫ��������� [ ��Ӫ��������ϵΪ��Ӫ���������� �²�Ʒģ��Ϊ���Ʒѱ��� ]
    char StratTime[15]; //��Чʱ�� [ YYYYMMDDHHMMSS ]
    char StopTime[15]; //ʧЧʱ�� [ YYYYMMDDHHMMSS ]
} YxPlanList;

typedef struct tag_ProdList
{
    char ProdId[15]; //��Ʒ���
    char StratTime[15]; //��Чʱ�� [ YYYYMMDDHHMMSS ]
    char StopTime[15]; //ʧЧʱ�� [ YYYYMMDDHHMMSS ]
} ProdList;

typedef struct tag_IntegrationCalFeeInfo
{
    char subsid[15]; //�û����
    char recflag[5]; //ҵ���ʶ [ 0----����ҵ�� 1----����ҵ�� ]
    vector<ProdList> ProdListArray; //��Ʒ�����б�
    vector<YxPlanList> YxPlanListArray; //Ӫ�����������б�
    char TradeType[33]; //ҵ������
    char ActiveTime[15]; //����ʱ�� [ YYYYMMDDHHMMSS ]
} IntegrationCalFeeInfo;


typedef struct tag_CalFeeResultList
{
    char acctid[257]; //�ʵ���Ŀ����
    char recamt[17]; //ʵ�ս�� [ �Է�Ϊ��λ ]
    char subamt[17]; //������ [ �Է�Ϊ��λ ]
} CalFeeResultList;

typedef struct tag_IntegrationCalFeeInfoOut
{
    vector<CalFeeResultList> CalFeeResultListArray; //��ѽ���б�
    char ErrNo[9]; //������ [ 0-�ɹ�; 1-�û�������; -1��ʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} IntegrationCalFeeInfoOut;


    
/*��������ѽӿ�*/
class IB_COM_IntegrationCalFee:public IB_COM_BASE
{
public:
    IB_COM_IntegrationCalFee()
    {
        m_errPrefix += "IB_COM_IntegrationCalFee" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_IntegrationCalFee( const IntegrationCalFeeInfo& data, IntegrationCalFeeInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<ProdList>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<YxPlanList>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<CalFeeResultList>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<ProdList>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<YxPlanList>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<CalFeeResultList>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IntegrationCalFeeInfo& data);
    bool getCallResult(HSendMsg *rcv, IntegrationCalFeeInfoOut& res);
    


};
#endif /*__IB_COM_INTEGRATIONCALFEE_H__*/

