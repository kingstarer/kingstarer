#ifndef __IM_COM_IMQRYWAREDETAIL_H__
#define __IM_COM_IMQRYWAREDETAIL_H__
#include "IM_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_IMQryWareDetailInfo
{
    char Accoid[15]; //�ʺ�
    char StartTime[15]; //��ʼʱ�� [ Yyyymmddhh24miss ]
    char EndTime[15]; //����ʱ�� [ Yyyymmddhh24miss ]
    char qryType[2]; //��ѯ���� [ 0ȫ����1ֻҪ������� ]
} IMQryWareDetailInfo;


typedef struct tag_Ucid
{
    char ResSerial[1]; //��Ʒ���
    char ResType[1]; //��Ʒ��ʶ
    char Status[1]; //��Ʒ״̬
    char Price[1]; //��Ʒ�۸�
    char UCID[1]; //UCID
} Ucid;

typedef struct tag_IMQryWareDetailInfoOut
{
    vector<Ucid> UcidArray; //ucid����
    char RetCode[1]; //������� [ ʧ��-1 ]
    char ErrMsg[1]; //����ʧ��ԭ��
} IMQryWareDetailInfoOut;


    
/*���Ŵ����ʵ���ѯ*/
class IM_COM_IMQryWareDetail:public IM_COM_BASE
{
public:
    IM_COM_IMQryWareDetail()
    {
        m_errPrefix += "IM_COM_IMQryWareDetail" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ��
    //ErrNo 0-�ɹ� 1-û�з��������ļ�¼ ����-ʧ��
    bool IM_IMQryWareDetail( const IMQryWareDetailInfo& data, IMQryWareDetailInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<Ucid>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<Ucid>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMQryWareDetailInfo& data);
    bool getCallResult(HSendMsg *rcv, IMQryWareDetailInfoOut& res);
    


};
#endif /*__IM_COM_IMQRYWAREDETAIL_H__*/

