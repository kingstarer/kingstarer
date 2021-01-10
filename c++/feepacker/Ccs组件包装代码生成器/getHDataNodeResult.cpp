bool IB_COM_<Common>::getHDataNodeResult(vector<<StuName>>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
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

