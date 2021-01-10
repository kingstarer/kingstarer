/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ���ʵ��ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_QueryBill.h"
#include "view/Server.h"

bool IB_COM_QueryBill::IB_QueryBill( const QueryBillInfo& data, QueryBillInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QueryBill";

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

bool IB_COM_QueryBill::setCallInfo(HRecvMsg *sendmsg, const QueryBillInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "�ʻ�ID");
    strValue = data.acctid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�û�ID");
    strValue = data.subsid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��ʼ����");
    strValue = data.StartCycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.EndCycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�ʵ�״̬");
    strValue = data.Status;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�Ƿ���ʾ���ʷ���");
    strValue = data.IsBadDebt;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�����Դ����ʷ���");
    strValue = data.HowBadDebt;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�����ʵ�����ʽ");
    strValue = data.BillType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "������ϸ�������ϼ���");
    strValue = data.BillFormatNum;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_QueryBill::getCallResult(HSendMsg *rcvmsg, QueryBillInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "�ʵ�����");
    if( !getHDataNodeResult(res.BillArray, node->DataList[pos++], 
            "����ṹ��[�ʵ�����]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "������");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[������]" ) )
    {
        return false;
    }

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrInfo, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}


bool IB_COM_QueryBill::getHDataNodeResult(vector<Bill>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 16;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        Bill item;
        string memname(destname);

        if( !StrToCstr(item.Accoid, *(node->DataList[pos++]->GetKey()), 
            memname + "[�ʺ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.Subsoid, *(node->DataList[pos++]->GetKey()), 
            memname + "[�û���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Cycle, *(node->DataList[pos++]->GetKey()), 
            memname + "[��������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalfee, *(node->DataList[pos++]->GetKey()), 
            memname + "[�ܷ��ã�������ϸ�ĺϼƣ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.ShouldPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[����Ӧ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.LeftInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ĩʣ�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.isWrtOff, *(node->DataList[pos++]->GetKey()), 
            memname + "[���ʱ�ʶ]") )
        {
            return false;
        }

        if( !StrToCstr(item.Paidamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[��������]") )
        {
            return false;
        }

        if( !StrToCstr(item.TipsInfo, *(node->DataList[pos++]->GetKey()), 
            memname + "[�Ƽ���Ϣ]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.FeeDetailArray, node->DataList[pos++], 
            memname + "[������ϸ]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.SpDetailArray, node->DataList[pos++], 
            memname + "[������Ϣ����ϸ]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.BalanceDetailArray, node->DataList[pos++], 
            memname + "[�����ϸ]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.IntDetailArray, node->DataList[pos++], 
            memname + "[������ϸ]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.MonDataArray, node->DataList[pos++], 
            memname + "[���·��ýṹ����]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.SixmonDataArray, node->DataList[pos++], 
            memname + "[�������������������]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.TipsDataArray, node->DataList[pos++], 
            memname + "[ʡǮ������������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<FeeDetail>& nodeinfo, 
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
        FeeDetail item;
        string memname(destname);

        if( !StrToCstr(item.BillItem, *(node->DataList[pos++]->GetKey()), 
            memname + "[�Ƿ��ʵ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.FeeTypeId, *(node->DataList[pos++]->GetKey()), 
            memname + "[���ô���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Feename, *(node->DataList[pos++]->GetKey()), 
            memname + "[��������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[���ý��]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<SpDetail>& nodeinfo, 
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
        SpDetail item;
        string memname(destname);

        if( !StrToCstr(item.SpName, *(node->DataList[pos++]->GetKey()), 
            memname + "[����������]") )
        {
            return false;
        }

        if( !StrToCstr(item.BusCode, *(node->DataList[pos++]->GetKey()), 
            memname + "[�������]") )
        {
            return false;
        }

        if( !StrToCstr(item.BusName, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ҵ������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[���ý��]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<BalanceDetail>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 13;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BalanceDetail item;
        string memname(destname);

        if( !StrToCstr(item.BeforeAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���³�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.ZSAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����]") )
        {
            return false;
        }

        if( !StrToCstr(item.DiscAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[�Ż�]") )
        {
            return false;
        }

        if( !StrToCstr(item.BackFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[�˷�]") )
        {
            return false;
        }

        if( !StrToCstr(item.PaybyAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���˴���]") )
        {
            return false;
        }

        if( !StrToCstr(item.InvalidAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ʧЧ����]") )
        {
            return false;
        }

        if( !StrToCstr(item.DepositAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���³�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.ThisMonBack, *(node->DataList[pos++]->GetKey()), 
            memname + "[���·���]") )
        {
            return false;
        }

        if( !StrToCstr(item.ThisMonPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[�����ۼ��ѽ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalfee, *(node->DataList[pos++]->GetKey()), 
            memname + "[���úϼ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.AfterAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ĩ����]") )
        {
            return false;
        }

        if( !StrToCstr(item.ShouldPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ĩӦ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.WillBackAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����δ�������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<IntDetail>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 8;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        IntDetail item;
        string memname(destname);

        if( !StrToCstr(item.MonInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���»���]") )
        {
            return false;
        }

        if( !StrToCstr(item.ConsumInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[�������ѻ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.AwardInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���½�������]") )
        {
            return false;
        }

        if( !StrToCstr(item.BeforeAvlInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ĩʣ��ɶһ�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.AwardedInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[�����Ѷһ�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.DonateInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[���¾�������]") )
        {
            return false;
        }

        if( !StrToCstr(item.LeftInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����ĩʣ�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.CleanInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[����������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<MonData>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 8;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        MonData item;
        string memname(destname);

        if( !StrToCstr(item.BaseFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[�»�����]") )
        {
            return false;
        }

        if( !StrToCstr(item.IncreFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[��ֵҵ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.ComFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[ͨ�ŷ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.SpFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[���շ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.PaybyFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[�����˸���]") )
        {
            return false;
        }

        if( !StrToCstr(item.ReceiveFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[���շ�]") )
        {
            return false;
        }

        if( !StrToCstr(item.ViolaFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[ΥԼ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.OtherFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[����]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<SixmonData>& nodeinfo, 
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
        SixmonData item;
        string memname(destname);

        if( !StrToCstr(item.BillCyc, *(node->DataList[pos++]->GetKey()), 
            memname + "[����]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[����]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<TipsData>& nodeinfo, 
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
        TipsData item;
        string memname(destname);

        if( !StrToCstr(item.RealUse, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʵ��ʹ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.TipsUse, *(node->DataList[pos++]->GetKey()), 
            memname + "[�ײ��·�]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<Bill>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�ʺ�
        strValue = nodeinfo[i].Accoid;
        node->AddSimpleData(strValue);

        //�û���
        strValue = nodeinfo[i].Subsoid;
        node->AddSimpleData(strValue);

        //��������
        strValue = nodeinfo[i].Cycle;
        node->AddSimpleData(strValue);

        //�ܷ��ã�������ϸ�ĺϼƣ�
        strValue = nodeinfo[i].Totalfee;
        node->AddSimpleData(strValue);

        //����Ӧ��
        strValue = nodeinfo[i].ShouldPay;
        node->AddSimpleData(strValue);

        //����ĩʣ�����
        strValue = nodeinfo[i].LeftInt;
        node->AddSimpleData(strValue);

        //���ʱ�ʶ
        strValue = nodeinfo[i].isWrtOff;
        node->AddSimpleData(strValue);

        //��������
        strValue = nodeinfo[i].Paidamt;
        node->AddSimpleData(strValue);

        //�Ƽ���Ϣ
        strValue = nodeinfo[i].TipsInfo;
        node->AddSimpleData(strValue);

        //������ϸ
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].FeeDetailArray) )
        {
            return false;
        }
        //������Ϣ����ϸ
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].SpDetailArray) )
        {
            return false;
        }
        //�����ϸ
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].BalanceDetailArray) )
        {
            return false;
        }
        //������ϸ
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].IntDetailArray) )
        {
            return false;
        }
        //���·��ýṹ����
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].MonDataArray) )
        {
            return false;
        }
        //�������������������
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].SixmonDataArray) )
        {
            return false;
        }
        //ʡǮ������������
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].TipsDataArray) )
        {
            return false;
        }

    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<FeeDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�Ƿ��ʵ���
        strValue = nodeinfo[i].BillItem;
        node->AddSimpleData(strValue);

        //���ô���
        strValue = nodeinfo[i].FeeTypeId;
        node->AddSimpleData(strValue);

        //��������
        strValue = nodeinfo[i].Feename;
        node->AddSimpleData(strValue);

        //���ý��
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<SpDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //����������
        strValue = nodeinfo[i].SpName;
        node->AddSimpleData(strValue);

        //�������
        strValue = nodeinfo[i].BusCode;
        node->AddSimpleData(strValue);

        //����ҵ������
        strValue = nodeinfo[i].BusName;
        node->AddSimpleData(strValue);

        //���ý��
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BalanceDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //���³�����
        strValue = nodeinfo[i].BeforeAmt;
        node->AddSimpleData(strValue);

        //����
        strValue = nodeinfo[i].ZSAmt;
        node->AddSimpleData(strValue);

        //�Ż�
        strValue = nodeinfo[i].DiscAmt;
        node->AddSimpleData(strValue);

        //�˷�
        strValue = nodeinfo[i].BackFee;
        node->AddSimpleData(strValue);

        //���˴���
        strValue = nodeinfo[i].PaybyAmt;
        node->AddSimpleData(strValue);

        //����ʧЧ����
        strValue = nodeinfo[i].InvalidAmt;
        node->AddSimpleData(strValue);

        //���³�����
        strValue = nodeinfo[i].DepositAmt;
        node->AddSimpleData(strValue);

        //���·���
        strValue = nodeinfo[i].ThisMonBack;
        node->AddSimpleData(strValue);

        //�����ۼ��ѽ���
        strValue = nodeinfo[i].ThisMonPay;
        node->AddSimpleData(strValue);

        //���úϼ�
        strValue = nodeinfo[i].Totalfee;
        node->AddSimpleData(strValue);

        //����ĩ����
        strValue = nodeinfo[i].AfterAmt;
        node->AddSimpleData(strValue);

        //����ĩӦ��
        strValue = nodeinfo[i].ShouldPay;
        node->AddSimpleData(strValue);

        //����δ�������
        strValue = nodeinfo[i].WillBackAmt;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<IntDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //���»���
        strValue = nodeinfo[i].MonInt;
        node->AddSimpleData(strValue);

        //�������ѻ���
        strValue = nodeinfo[i].ConsumInt;
        node->AddSimpleData(strValue);

        //���½�������
        strValue = nodeinfo[i].AwardInt;
        node->AddSimpleData(strValue);

        //����ĩʣ��ɶһ�����
        strValue = nodeinfo[i].BeforeAvlInt;
        node->AddSimpleData(strValue);

        //�����Ѷһ�����
        strValue = nodeinfo[i].AwardedInt;
        node->AddSimpleData(strValue);

        //���¾�������
        strValue = nodeinfo[i].DonateInt;
        node->AddSimpleData(strValue);

        //����ĩʣ�����
        strValue = nodeinfo[i].LeftInt;
        node->AddSimpleData(strValue);

        //����������
        strValue = nodeinfo[i].CleanInt;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<MonData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�»�����
        strValue = nodeinfo[i].BaseFee;
        node->AddSimpleData(strValue);

        //��ֵҵ���
        strValue = nodeinfo[i].IncreFee;
        node->AddSimpleData(strValue);

        //ͨ�ŷ�
        strValue = nodeinfo[i].ComFee;
        node->AddSimpleData(strValue);

        //���շ�
        strValue = nodeinfo[i].SpFee;
        node->AddSimpleData(strValue);

        //�����˸���
        strValue = nodeinfo[i].PaybyFee;
        node->AddSimpleData(strValue);

        //���շ�
        strValue = nodeinfo[i].ReceiveFee;
        node->AddSimpleData(strValue);

        //ΥԼ��
        strValue = nodeinfo[i].ViolaFee;
        node->AddSimpleData(strValue);

        //����
        strValue = nodeinfo[i].OtherFee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<SixmonData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //����
        strValue = nodeinfo[i].BillCyc;
        node->AddSimpleData(strValue);

        //����
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<TipsData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //ʵ��ʹ��
        strValue = nodeinfo[i].RealUse;
        node->AddSimpleData(strValue);

        //�ײ��·�
        strValue = nodeinfo[i].TipsUse;
        node->AddSimpleData(strValue);


    }

    return true;
}



