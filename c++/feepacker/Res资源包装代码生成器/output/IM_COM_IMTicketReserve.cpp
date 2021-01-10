/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ��ֵȯ����ԴԤ��
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IM_COM_IMTicketReserve.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMTicketReserve::IM_IMTicketReserve( const IMTicketReserveInfo& data, IMTicketReserveInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMTicketReserve";

    //Msg Define
    auto_ptr<HRecvMsg> sendmsg ( new HRecvMsg() );              // ��������
    auto_ptr<HSendMsg> rcvmsg  ( new HSendMsg() );              // ��������


    BASE_LOG_INFO("���ṹ��ת����xml����");

    if ( setCallInfo(sendmsg.get(), data) )
    {
        BASE_LOG_INFO("ת���ɹ�");
    }
    else
    {
         BASE_LOG_INFO("ת��ʧ��");
         return false;
    }
    

    BASE_LOG_INFO("����Զ�̽ӿ�");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_INFO("����ʧ��");
        return false;
    }

    BASE_LOG_INFO("����cics���ر���");
    if (!getCallResult(rcvmsg.get(), res))
    {
        BASE_LOG_ERROR("����cics���ر���ʧ��");
        return false;
    }
    

    BASE_LOG_DEBUG("���������:%d,������Ϣ:%s", m_iErrorCode, m_sErrorStr.c_str());
    BASE_LOG_INFO( "-------------ִ�е���CICS�ӿ�END:%s-------------- ", m_rpcname );

    return true;
}

bool IM_COM_IMTicketReserve::setCallInfo(HRecvMsg *sendmsg, const IMTicketReserveInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��λ����");
    strValue = data.OrgID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����Ա����");
    strValue = data.OperID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ҵ����ˮ��");
    strValue = data.RecOID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��ֵȯ����");
    strValue = data.Quantity;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��ֵȯ��Դ����");
    strValue = data.Restype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�����̱���");
    strValue = data.Agentid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����Ա���ܹ�Կ");
    strValue = data.PubKey;
    node->AddSimpleData(strValue);


    return true;
}


bool IM_COM_IMTicketReserve::getCallResult(HSendMsg *rcvmsg, IMTicketReserveInfoOut& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = 4;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "�ļ�����");
    if( !StrToCstr(res.FileName, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[�ļ�����]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��ֵȯ����Ϣ�б�");
    if( !getHDataNodeResult(res.PacketInfoArray, node->DataList[pos++], 
            "����ṹ��[��ֵȯ����Ϣ�б�]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "�������");
    if( !StrToCstr(res.RetCode, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[�������]" ) )
    {
        return false;
    }

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrMsg, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}


bool IM_COM_IMTicketReserve::getHDataNodeResult(vector<PacketInfo>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 4;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        PacketInfo item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeq, *(node->DataList[pos++]->GetKey()), 
            memname + "[��ֵȯ�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgKing, *(node->DataList[pos++]->GetKey()), 
            memname + "[������]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgSeqEnd, *(node->DataList[pos++]->GetKey()), 
            memname + "[���κ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.ParValue, *(node->DataList[pos++]->GetKey()), 
            memname + "[��ֵȯ���ĵ���]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IM_COM_IMTicketReserve::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<PacketInfo>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //��ֵȯ�����
        strValue = nodeinfo[i].PkgSeq;
        node->AddSimpleData(strValue);

        //������
        strValue = nodeinfo[i].PkgKing;
        node->AddSimpleData(strValue);

        //���κ�
        strValue = nodeinfo[i].PkgSeqEnd;
        node->AddSimpleData(strValue);

        //��ֵȯ���ĵ���
        strValue = nodeinfo[i].ParValue;
        node->AddSimpleData(strValue);


    }

    return true;
}



