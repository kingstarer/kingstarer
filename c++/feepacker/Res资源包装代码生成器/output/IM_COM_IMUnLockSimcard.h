#ifndef __IM_COM_IMUNLOCKSIMCARD_H__
#define __IM_COM_IMUNLOCKSIMCARD_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMUnLockSimcardInfo
{
    char Region[7]; //��������
    char OrgID[33]; //��λ����
    char OperID[33]; //����Ա����
    char ICCID[33]; //SIM����
    char OprType[3]; //�������� [ (0 ���� 1 ȡ��) ]
    char RecOID[33]; //ҵ��������ˮ��
    char Price[17]; //ʵ�����ۼ۸�
} IMUnLockSimcardInfo;


typedef struct tag_IMUnLockSimcardInfoOut
{
    char RetCode[7]; //������� [ �ɹ�1 ʧ��-1 ]
    char ErrMsg[513]; //����ʧ��ԭ��
} IMUnLockSimcardInfoOut;


    
/*SIM�������ͷ�*/
class IM_COM_IMUnLockSimcard:public IM_COM_BASE
{
public:
    IM_COM_IMUnLockSimcard()
    {
        m_errPrefix += "IM_COM_IMUnLockSimcard" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IM_IMUnLockSimcard( const IMUnLockSimcardInfo& data, IMUnLockSimcardInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���




private:
    bool setCallInfo(HRecvMsg *send, const IMUnLockSimcardInfo& data);
    bool getCallResult(HSendMsg *rcv, IMUnLockSimcardInfoOut& res);
    


};
#endif /*__IM_COM_IMUNLOCKSIMCARD_H__*/

