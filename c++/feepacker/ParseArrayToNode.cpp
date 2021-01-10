bool IB_COM_<Common>::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<<StuName>>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

<ParseArrayToNode>
    }

    return true;
}

