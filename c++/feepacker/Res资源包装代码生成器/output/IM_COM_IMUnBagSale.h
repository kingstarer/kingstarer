#ifndef __IM_COM_IMUNBAGSALE_H__
#define __IM_COM_IMUNBAGSALE_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_BagSalePack
{
    char PkgSeqBegin[33]; //包起始编号
    char PkgSeqEnd[33]; //包结束编号
} BagSalePack;

typedef struct tag_IMUnBagSaleInfo
{
    char Region[7]; //地区编码
    char OrgID[33]; //单位编码
    char OperID[33]; //操作员工号
    char RecOID[33]; //业务流水号
    char Agentid[33]; //代理商编码
    vector<BagSalePack> BagSalePackArray; //冲值券包编号列表
} IMUnBagSaleInfo;


typedef struct tag_BagSaleErrInfo
{
    char PkgSeq[33]; //包编号
    char ErrMsg[1025]; //错误信息
} BagSaleErrInfo;

typedef struct tag_IMUnBagSaleInfoOut
{
    vector<BagSaleErrInfo> BagSaleErrInfoArray; //错误信息列表
    char RetCode[7]; //错误代码 [ 部分成功 0 全部成功 1 失败 -1 程序异常 -2 ]
    char ErrMsg[513]; //操作失败原因
} IMUnBagSaleInfoOut;


    
/*充值券包销售回退*/
class IM_COM_IMUnBagSale:public IM_COM_BASE
{
public:
    IM_COM_IMUnBagSale()
    {
        m_errPrefix += "IM_COM_IMUnBagSale" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IM_IMUnBagSale( const IMUnBagSaleInfo& data, IMUnBagSaleInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<BagSalePack>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<BagSaleErrInfo>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<BagSalePack>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<BagSaleErrInfo>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMUnBagSaleInfo& data);
    bool getCallResult(HSendMsg *rcv, IMUnBagSaleInfoOut& res);
    


};
#endif /*__IM_COM_IMUNBAGSALE_H__*/

