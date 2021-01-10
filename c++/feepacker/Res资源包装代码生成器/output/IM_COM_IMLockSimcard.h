#ifndef __IM_COM_IMLOCKSIMCARD_H__
#define __IM_COM_IMLOCKSIMCARD_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMLockSimcardInfo
{
    char Region[7]; //��������
    char OrgID[33]; //��λ����
    char OperID[33]; //����Ա����
    char ICCID[33]; //SIM����
    char sRecOID[33]; //ҵ��������ˮ��
    char iPrice[17]; //ʵ�����ۼ۸�
} IMLockSimcardInfo;


typedef struct tag_IMLockSimcardInfoOut
{
    char RetCode[7]; //������� [ �ɹ�1 ʧ��-1 ]
    char ErrMsg[513]; //����ʧ��ԭ��
} IMLockSimcardInfoOut;


    
/*SIM������*/
class IM_COM_IMLockSimcard:public IM_COM_BASE
{
public:
    IM_COM_IMLockSimcard()
    {
        m_errPrefix += "IM_COM_IMLockSimcard" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IM_IMLockSimcard( const IMLockSimcardInfo& data, IMLockSimcardInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���




private:
    bool setCallInfo(HRecvMsg *send, const IMLockSimcardInfo& data);
    bool getCallResult(HSendMsg *rcv, IMLockSimcardInfoOut& res);
    


};
#endif /*__IM_COM_IMLOCKSIMCARD_H__*/

