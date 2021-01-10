#ifndef __IB_COM_MOVEINTEGRAL_H__
#define __IB_COM_MOVEINTEGRAL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_MoveIntegralInfo
{
    char Businum[21]; //ҵ����ˮ��
    char Outcustid[15]; //ת���ͻ����� [ ת����Ʒ����Ϊȫ��ͨ����ʱʹ�� ]
    char OutServnumber[21]; //ת��������� [ ת����Ʒ����Ϊ���еش�����,�����к��룬���ݴ��ڿ�����ʱʹ�� ]
    char OutProdtype[33]; //ת����Ʒ���� [ 0:ȫ��ͨ���� 1:���еش����� 2.�����к��� 3.���ݴ��ڿ����� ]
    char Region[9]; //��������
    char Incustid[15]; //ת��ͻ�����
    char InServnumber[21]; //ת�������� [ ת���Ʒ����Ϊ���еش�����,�����к��룬���ݴ��ڿ�����ʱʹ�� ]
    char InProdtype[33]; //ת���Ʒ���� [ 0:ȫ��ͨ���� 1:���еش����� 2.�����к��� 3.���ݴ��ڿ����� ]
    char Oprcode[21]; //����Ա
    char Intchgrsn[9]; //���ֱ��ԭ��
    char Moveintegral[15]; //ת�ƻ���
    char Movecycle[7]; //�������� [ YYYY ]
    char Unit[33]; //Ӫҵ��
} MoveIntegralInfo;


typedef struct tag_MoveIntegralInfoOut
{
    char ErrNo[9]; //������ [ 0�D�D�ɹ��������D�Dʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} MoveIntegralInfoOut;


    
/*����ת��*/
class IB_COM_MoveIntegral:public IB_COM_BASE
{
public:
    IB_COM_MoveIntegral()
    {
        m_errPrefix += "IB_COM_MoveIntegral" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_MoveIntegral( const MoveIntegralInfo& data, MoveIntegralInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���




private:
    bool setCallInfo(HRecvMsg *send, const MoveIntegralInfo& data);
    bool getCallResult(HSendMsg *rcv, MoveIntegralInfoOut& res);
    


};
#endif /*__IB_COM_MOVEINTEGRAL_H__*/

