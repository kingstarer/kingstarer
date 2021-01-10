#ifndef __IB_COM_QRYBALANCEFLOWLOG_H__
#define __IB_COM_QRYBALANCEFLOWLOG_H__
#include "IB_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_QryBalanceFlowLogInfo
{
	char Accoid[15]; //�ʺ�
	char StartTime[15]; //��ʼʱ�� [ Yyyymmddhh24miss ]
	char EndTime[15]; //����ʱ�� [ Yyyymmddhh24miss ]
	char qryType[2]; //��ѯ���� [ 0ȫ����1ֻҪ������� ]
} QryBalanceFlowLogInfo;


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

typedef struct tag_QryBalanceFlowLogInfoOut
{
	vector<ALog> ALogArray; //������Ϣ����
	char ErrNo[9]; //������ [ 0�D�D�ɹ��������D�Dʧ�� ]
	char ErrInfo[257]; //���ش�����Ϣ
} QryBalanceFlowLogInfoOut;


	
/*���������ʷ����Ŀ�����־����ѯ�ӿ�*/
class IB_COM_QryBalanceFlowLog:public IB_COM_BASE
{
public:
    IB_COM_QryBalanceFlowLog()
    {
        m_errPrefix += "IB_COM_QryBalanceFlowLog" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //ErrNo 0-�ɹ� 1-û�з��������ļ�¼ ����-ʧ��
    bool IB_QryBalanceFlowLog( const QryBalanceFlowLogInfo& data, QryBalanceFlowLogInfoOut& res );
private:
    bool setCallInfo(HRecvMsg *send, const QryBalanceFlowLogInfo& data);
    bool getCallResult(HSendMsg *rcv, QryBalanceFlowLogInfoOut& res);
    bool getHDataNodeResult(vector<ALog>& nodeinfo, IN HDataNode* node, const string& destname);
};
#endif /*__IB_COM_QRYBALANCEFLOWLOG_H__*/
