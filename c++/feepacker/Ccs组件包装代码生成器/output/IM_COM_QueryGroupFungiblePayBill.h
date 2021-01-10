#ifndef __IM_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#define __IM_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#include "IM_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_InParam
{
    char Accoid[15]; //�ʺ�
    char StartTime[15]; //��ʼʱ�� [ Yyyymmddhh24miss ]
    char EndTime[15]; //����ʱ�� [ Yyyymmddhh24miss ]
    char qryType[2]; //��ѯ���� [ 0ȫ����1ֻҪ������� ]
} InParam;


typedef struct tag_ALog
{
    char SubjectId[33]; //��Ŀ����Ŀ������
    char StartCycle[7]; //��Ŀ��ʼ����
    char EndCycle[7]; //��Ŀ��������
    char FlowTime[15]; //����ʱ�� [ Yyyymmddhh24miss ]
    char FlowDirection[2]; //�������� [ 0���룬1���� ]
    char FlowType[33]; //�������� [ ���մ� ]
    char FlowTypeName[65]; //���ԭ��
    char FlowAmt[15]; //�������
    char FlowNoBalance[15]; //������ʣ��
    char OperID[11]; //�����˹���
    char Formnum[15]; //��ˮ�� [ ��Ӫҵ����Ľɷ���ˮ�ţ���������������绮�ۣ����գ�Ϊ�մ� ]
    char Source[9]; //��Դ [ ���г�ֵ��д�����̣��������Ƽ�дLL���������ԭ���ÿ� ]
} ALog;

typedef struct tag_OutParam
{
    vector<ALog> ALogArray; //������Ϣ����
    char ErrNo[9]; //������ [ 0�D�D�ɹ��������D�Dʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} OutParam;


    
/*���Ŵ����ʵ���ѯ*/
class IM_COM_QueryGroupFungiblePayBill:public IM_COM_BASE
{
public:
    IM_COM_QueryGroupFungiblePayBill()
    {
        m_errPrefix += "IM_COM_QueryGroupFungiblePayBill" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //ErrNo 0-�ɹ� 1-û�з��������ļ�¼ ����-ʧ��
    bool IM_QueryGroupFungiblePayBill( const InParam& data, OutParam& res );
private:
    bool setCallInfo(HRecvMsg *send, const InParam& data);
    bool getCallResult(HSendMsg *rcv, OutParam& res);
    bool getHDataNodeResult(vector<ALog>& nodeinfo, IN HDataNode* node, const string& destname);

};
#endif /*__IM_COM_QUERYGROUPFUNGIBLEPAYBILL_H__*/

