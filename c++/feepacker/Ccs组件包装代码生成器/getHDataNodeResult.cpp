bool IB_COM_<Common>::getHDataNodeResult(vector<<StuName>>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = <getHDataNodeResultCnt>;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        <StuName> item;
        string memname(destname);

<getHDataNodeResult>
        nodeinfo.push_back(item);
    }

    return true;
}

