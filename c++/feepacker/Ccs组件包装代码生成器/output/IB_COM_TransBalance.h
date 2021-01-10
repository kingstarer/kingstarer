#ifndef __IB_COM_TRANSBALANCE_H__
#define __IB_COM_TRANSBALANCE_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_TransBalanceInfo
{
    char Formnum[21]; //�ɷ���ˮ��
    char FromAccoid[15]; //Դ�ʺ�
    char ToAccoid[15]; //Ŀ���ʺ�
    char Unit[33]; //Ӫҵ��
    char Op[33]; //����Ա
    char busicode[33]; //ҵ������
    char payReason[21]; //�ɷѱ������
} TransBalanceInfo;


typedef struct tag_TransBalanceInfoOut
{
    char ErrNo[9]; //������ [ 0�D�D�ɹ��� �����D�Dʧ�� ]
    char TransFee[15]; //ת�ƽ��
    char ErrInfo[257]; //���ش�����Ϣ
} TransBalanceInfoOut;


    
/*���ת��*/
class IB_COM_TransBalance:public IB_COM_BASE
{
public:
    IB_COM_TransBalance()
    {
        m_errPrefix += "IB_COM_TransBalance" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_TransBalance( const TransBalanceInfo& data, TransBalanceInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���




private:
    bool setCallInfo(HRecvMsg *send, const TransBalanceInfo& data);
    bool getCallResult(HSendMsg *rcv, TransBalanceInfoOut& res);
    


};
#endif /*__IB_COM_TRANSBALANCE_H__*/

