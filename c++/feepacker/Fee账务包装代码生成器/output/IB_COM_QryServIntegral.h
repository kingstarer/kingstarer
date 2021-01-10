#ifndef __IB_COM_QRYSERVINTEGRAL_H__
#define __IB_COM_QRYSERVINTEGRAL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_QryServIntegralInfo
{
    char Region[9]; //��������
    char Servnumber[21]; //������� [ ��Ʒ����Ϊ���еش�����, �����к��룬���ݴ��ڿ�����ʱʹ�� ]
    char CustID[15]; //�ͻ���� [ ��Ʒ����Ϊȫ��ͨ����ʱʹ�� ]
    char Prodtype[33]; //��Ʒ���� [ 0:ȫ��ͨ���� 1:���еش����� 2:�����к��� 3:���ݴ��ڿ����� 4:���ſͻ�������Ʒ����ָ��Ϊ���ſͻ�ʱ��ֻ����CustID��������ʱCustID�ֶβ���Ϊ�գ� ]
    char Integralcyc[7]; //�������� [ YYYY "-1"------��ѯ�ÿͻ����л������ڵĻ�����Ϣ ]
    char Brandscoretype[33]; //ȫƷ�ƻ������� [ 0;�������� 1:�Żݻ��� -1:������������ֺ��Żݻ��� ]
} QryServIntegralInfo;


typedef struct tag_ACustIntegral
{
    char ServNumber[21]; //������� [ �����������е�prodTypeΪ4������ֶ�ֵΪ���ſͻ����л��ֱ���ĸ���Ʒ�ķ�����룻���prodType��Ϊ4����Ϊ����ͻ��ķ������ ]
    char Totalintegral[15]; //�ܻ���
    char Integralcyc[7]; //�������� [ ����˵������������Ϊ0ʱ���û��ֱ�ʾΪ�Żݻ��֣�Ϊ������ʱ���û��ֻ��ǻ��������еĿ��û��� ]
    char Availintegral[15]; //���û���
    char Moveintegral[15]; //ת�ƻ���
    char Unavailintegral[15]; //ʧЧ����
    char Saleintegral[15]; //�������
    char Updatetime[15]; //���¸���ʱ��
    char Stoptime[15]; //ʧЧʱ�� [ ������Żݻ���ʱ������Żݻ�����Ч�� ]
    char Brandscoretype[33]; //ȫƷ�ƻ������� [ 0;�������� 1:�Żݻ��� -1:������������ֺ��Żݻ��� ]
} ACustIntegral;

typedef struct tag_QryServIntegralInfoOut
{
    vector<ACustIntegral> ACustIntegralArray; //������Ϣ����
    char ErrNo[9]; //������ [ 0�D�D�ɹ��� �����D�Dʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} QryServIntegralInfoOut;


    
/*��ͻ�������Ϣ*/
class IB_COM_QryServIntegral:public IB_COM_BASE
{
public:
    IB_COM_QryServIntegral()
    {
        m_errPrefix += "IB_COM_QryServIntegral" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_QryServIntegral( const QryServIntegralInfo& data, QryServIntegralInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<ACustIntegral>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<ACustIntegral>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QryServIntegralInfo& data);
    bool getCallResult(HSendMsg *rcv, QryServIntegralInfoOut& res);
    


};
#endif /*__IB_COM_QRYSERVINTEGRAL_H__*/

