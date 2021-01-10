/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ��ֵȯ�����ۻ���
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IM_COM_IMUnBagSale.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMUnBagSale::IM_IMUnBagSale( const IMUnBagSaleInfo& data, IMUnBagSaleInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMUnBagSale";

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

bool IM_COM_IMUnBagSale::setCallInfo(HRecvMsg *sendmsg, const IMUnBagSaleInfo& data)
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

    BASE_LOG_DEBUG("%s", "�����̱���");
    strValue = data.Agentid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��ֵȯ������б�");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.BagSalePackArray) )
    {
        return false;
    }


    return true;
}


bool IM_COM_IMUnBagSale::getCallResult(HSendMsg *rcvmsg, IMUnBagSaleInfoOut& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = 3;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "������Ϣ�б�");
    if( !getHDataNodeResult(res.BagSaleErrInfoArray, node->DataList[pos++], 
            "����ṹ��[������Ϣ�б�]" ) )
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


bool IM_COM_IMUnBagSale::getHDataNodeResult(vector<BagSalePack>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 2;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BagSalePack item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeqBegin, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ʼ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgSeqEnd, *(node->DataList[pos++]->GetKey()), 
            memname + "[���������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IM_COM_IMUnBagSale::getHDataNodeResult(vector<BagSaleErrInfo>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 2;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BagSaleErrInfo item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeq, *(node->DataList[pos++]->GetKey()), 
            memname + "[�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.ErrMsg, *(node->DataList[pos++]->GetKey()), 
            memname + "[������Ϣ]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IM_COM_IMUnBagSale::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BagSalePack>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //����ʼ���
        strValue = nodeinfo[i].PkgSeqBegin;
        node->AddSimpleData(strValue);

        //���������
        strValue = nodeinfo[i].PkgSeqEnd;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IM_COM_IMUnBagSale::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BagSaleErrInfo>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�����
        strValue = nodeinfo[i].PkgSeq;
        node->AddSimpleData(strValue);

        //������Ϣ
        strValue = nodeinfo[i].ErrMsg;
        node->AddSimpleData(strValue);


    }

    return true;
}



