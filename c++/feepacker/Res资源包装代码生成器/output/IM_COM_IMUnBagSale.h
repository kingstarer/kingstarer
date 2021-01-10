#ifndef __IM_COM_IMUNBAGSALE_H__
#define __IM_COM_IMUNBAGSALE_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_BagSalePack
{
    char PkgSeqBegin[33]; //����ʼ���
    char PkgSeqEnd[33]; //���������
} BagSalePack;

typedef struct tag_IMUnBagSaleInfo
{
    char Region[7]; //��������
    char OrgID[33]; //��λ����
    char OperID[33]; //����Ա����
    char RecOID[33]; //ҵ����ˮ��
    char Agentid[33]; //�����̱���
    vector<BagSalePack> BagSalePackArray; //��ֵȯ������б�
} IMUnBagSaleInfo;


typedef struct tag_BagSaleErrInfo
{
    char PkgSeq[33]; //�����
    char ErrMsg[1025]; //������Ϣ
} BagSaleErrInfo;

typedef struct tag_IMUnBagSaleInfoOut
{
    vector<BagSaleErrInfo> BagSaleErrInfoArray; //������Ϣ�б�
    char RetCode[7]; //������� [ ���ֳɹ� 0 ȫ���ɹ� 1 ʧ�� -1 �����쳣 -2 ]
    char ErrMsg[513]; //����ʧ��ԭ��
} IMUnBagSaleInfoOut;


    
/*��ֵȯ�����ۻ���*/
class IM_COM_IMUnBagSale:public IM_COM_BASE
{
public:
    IM_COM_IMUnBagSale()
    {
        m_errPrefix += "IM_COM_IMUnBagSale" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IM_IMUnBagSale( const IMUnBagSaleInfo& data, IMUnBagSaleInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<BagSalePack>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<BagSaleErrInfo>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<BagSalePack>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<BagSaleErrInfo>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMUnBagSaleInfo& data);
    bool getCallResult(HSendMsg *rcv, IMUnBagSaleInfoOut& res);
    


};
#endif /*__IM_COM_IMUNBAGSALE_H__*/

