/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   <�������>
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_<Common>.h"

void CIB_SRV_<Common>::setBusinessName()
{
    m_rpcname = "IB_SRV_<Common>";
    m_businessname = "<�������>";
}

char * CIB_SRV_<Common>::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_<Common>";
}	

bool CIB_SRV_<Common>::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

	bool bRet = false;
    <inparam> info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        <outparam> outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_<Common>(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_<Common>�������ʧ��");
            m_iErrorCode = m_cicsOpr.m_iErrorCode;
            m_sErrorStr = m_cicsOpr.m_sErrorStr;
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
        
        if (!bRet || !strncmp(outinfo.ErrNo, "0", sizeof(outinfo.ErrNo)) )
        {
            BASE_LOG_WARN("cics���ò��ɹ�,ҵ��ع�");
            CicsRollBack();
            if (bRet)
            {
                outinfo.ErrNo[sizeof(outinfo.ErrNo) - 1] = '\0';
                BASE_LOG_ERROR("���񷵻���[%s]", outinfo.ErrNo);
            }
        }
    }

    return bRet;
}


bool CIB_SRV_<Common>::ParseSrvMsg(HSendMsg *sendmsg, <inparam>& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = <ParseSrvMsgCnt>;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
<ParseSrvMsg>
	return true;
}

bool CIB_SRV_<Common>::ParseComMsg(HRecvMsg *rcvmsg, <outparam>& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
<ParseComMsg>
    return true;
}

//Ĭ�ϵ�setErrorXml�����xml����Ϊ[(������), (seq�ṹ��), (������), (������Ϣ)]
//�����Ҫ������ʽ�Ĵ���������Ҫ���ش˺�����������Բ�����
void CIB_SRV_<Common>::setErrorXml(HRecvMsg * rcvmsg)
{
    //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //�������
    rootnode->FreeChildNode();

    string nullstr;
<setErrorXml>
}


