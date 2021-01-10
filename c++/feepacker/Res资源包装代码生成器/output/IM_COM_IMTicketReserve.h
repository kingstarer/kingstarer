#ifndef __IM_COM_IMTICKETRESERVE_H__
#define __IM_COM_IMTICKETRESERVE_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMTicketReserveInfo
{
    char Region[33]; //��������
    char OrgID[33]; //��λ����
    char OperID[33]; //����Ա����
    char RecOID[33]; //ҵ����ˮ��
    char Quantity[33]; //��ֵȯ���� [ ���������ǰ��������� ]
    char Restype[33]; //��ֵȯ��Դ���� [ ��С�ࣩ ]
    char Agentid[33]; //�����̱���
    char PubKey[257]; //����Ա���ܹ�Կ
} IMTicketReserveInfo;


typedef struct tag_PacketInfo
{
    char PkgSeq[33]; //��ֵȯ�����
    char PkgKing[33]; //������ [ һ���Ű���һǧ�Ű���һ���Ű� ]
    char PkgSeqEnd[33]; //���κ�
    char ParValue[33]; //��ֵȯ���ĵ��� [ ʵ�ʼ۸� ]
} PacketInfo;

typedef struct tag_IMTicketReserveInfoOut
{
    char FileName[257]; //�ļ�����
    vector<PacketInfo> PacketInfoArray; //��ֵȯ����Ϣ�б�
    char RetCode[7]; //������� [ �ɹ�1 ʧ��-1 ]
    char ErrMsg[512]; //����ʧ��ԭ��
} IMTicketReserveInfoOut;


    
/*��ֵȯ����ԴԤ��*/
class IM_COM_IMTicketReserve:public IM_COM_BASE
{
public:
    IM_COM_IMTicketReserve()
    {
        m_errPrefix += "IM_COM_IMTicketReserve" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IM_IMTicketReserve( const IMTicketReserveInfo& data, IMTicketReserveInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<PacketInfo>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<PacketInfo>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMTicketReserveInfo& data);
    bool getCallResult(HSendMsg *rcv, IMTicketReserveInfoOut& res);
    


};
#endif /*__IM_COM_IMTICKETRESERVE_H__*/

