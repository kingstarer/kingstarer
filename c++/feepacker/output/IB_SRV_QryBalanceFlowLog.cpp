/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ���������ʷ����Ŀ�����־����ѯ�ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_QryBalanceFlowLog.h"

void CIB_SRV_QryBalanceFlowLog::setBusinessName()
{
    m_rpcname = "IB_SRV_QryBalanceFlowLog";
    m_businessname = "���������ʷ����Ŀ�����־����ѯ�ӿ�";
}

char * CIB_SRV_QryBalanceFlowLog::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_QryBalanceFlowLog";
}	

bool CIB_SRV_QryBalanceFlowLog::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

	bool bRet = false;
    QryBalanceFlowLogInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        QryBalanceFlowLogInfoOut outinfo;
        IB_COM_QryBalanceFlowLog cicsOpr;
        //��������߼�
        bRet = cicsOpr.IB_QryBalanceFlowLog(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QryBalanceFlowLog�������ʧ��");
            m_iErrorCode = cicsOpr.m_iErrorCode;
            m_sErrorStr = cicsOpr.m_sErrorStr;
        }
        else
        {
            //���������ת����xml����
            bRet = ParseComMsg(rcvmsg, outinfo);
            if (!bRet)
            {
                BASE_LOG_ERROR("���������ת����xml����ʧ��");
            }
        }
    }

    return bRet;
}


bool CIB_SRV_QryBalanceFlowLog::ParseSrvMsg(HSendMsg *sendmsg, QryBalanceFlowLogInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 4;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.Accoid, pStr, "�ʺ�"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.StartTime, pStr, "��ʼʱ��"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.EndTime, pStr, "����ʱ��"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.qryType, pStr, "��ѯ����"))
	{
		return false;
	}


	return true;
}

bool CIB_SRV_QryBalanceFlowLog::ParseComMsg(HRecvMsg *rcvmsg, QryBalanceFlowLogInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
	//������Ϣ����
	if ( !ParseArrayToNode(node->AddArrayChild(), data.ALogArray) )
	{
		return false;
	}

	//��װ����seq�ڵ�
	node->AddUseqChild();

	//������
	strValue = data.ErrNo;
	node->AddSimpleData(strValue);

	//���ش�����Ϣ
	strValue = data.ErrInfo;
	node->AddSimpleData(strValue);


    return true;
}

//Ĭ�ϵ�setErrorXml�����xml����Ϊ[(������), (seq�ṹ��), (������), (������Ϣ)]
//�����Ҫ������ʽ�Ĵ���������Ҫ���ش˺�����������Բ�����
 void CIB_SRV_QryBalanceFlowLog::setErrorXml(HRecvMsg * rcvmsg)
 {
     //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
     HDataNode *rootnode = rcvmsg->GetRootNode();

     //�������
     rootnode->FreeChildNode();

     string nullstr;
	//��������Ҫ�ڴ����һ���յ�����
	rootnode->AddArrayChild();

	rootnode->AddUseqChild(); //��װ�ķ�����Ҫ����seq�ڵ�

	string tmpstr = IntToStr(m_iErrorCode); //xlc��֧�ֽ�����ֵ��������
	rootnode->AddSimpleData( tmpstr );

	rootnode->AddSimpleData( m_sErrorStr );


 }

bool CIB_SRV_QryBalanceFlowLog::ParseArrayToNode(HDataNode *arraynode, const vector<ALog>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

		//��Ŀ����Ŀ������
		strValue = nodeinfo[i].SubjectId;
		node->AddSimpleData(strValue);

		//��Ŀ��ʼ����
		strValue = nodeinfo[i].StartCycle;
		node->AddSimpleData(strValue);

		//��Ŀ��������
		strValue = nodeinfo[i].EndCycle;
		node->AddSimpleData(strValue);

		//����ʱ��
		strValue = nodeinfo[i].FlowTime;
		node->AddSimpleData(strValue);

		//��������
		strValue = nodeinfo[i].FlowDirection;
		node->AddSimpleData(strValue);

		//��������
		strValue = nodeinfo[i].FlowType;
		node->AddSimpleData(strValue);

		//���ԭ��
		strValue = nodeinfo[i].FlowTypeName;
		node->AddSimpleData(strValue);

		//�������
		strValue = nodeinfo[i].FlowAmt;
		node->AddSimpleData(strValue);

		//������ʣ��
		strValue = nodeinfo[i].FlowNoBalance;
		node->AddSimpleData(strValue);

		//�����˹���
		strValue = nodeinfo[i].OperID;
		node->AddSimpleData(strValue);

		//��ˮ��
		strValue = nodeinfo[i].Formnum;
		node->AddSimpleData(strValue);

		//��Դ
		strValue = nodeinfo[i].Source;
		node->AddSimpleData(strValue);


    }

    return true;
}

