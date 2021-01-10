#ifndef __IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#define __IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__
#include "IB_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_QueryGroupFungiblePayBillInfo
{
    char Region[7]; //地区编码
    char OrgID[33]; //单位编码
    char OperID[33]; //操作员工号
    char ResType[33]; //商品类别
    char ResSerial[33]; //商品序号
} QueryGroupFungiblePayBillInfo;


typedef struct tag_CustMemberBill
{
    char ServNum[33]; //代付的产品号码
    char Name[65]; //代付的产品名称
    char FungibleRate[65]; //代付额度
    char RealPayFee[15]; //实际代付金额
} CustMemberBill;

typedef struct tag_BillAccount
{
    char GroupID[15]; //集团编码 [ 此参数与输入参数同值 ]
    char Cycle[7]; //帐务周期
    char Totalfee[15]; //总费用（所有费用项的合计） [ CustMemberBill 中各项费用的合计 ]
    vector<CustMemberBill> CustMemberBillArray; //费用明细
} BillAccount;

typedef struct tag_QueryGroupFungiblePayBillInfoOut
{
    vector<BillAccount> BillAccountArray; //代付帐单数组
    char SourceNo[15]; //查询流水号 [ 该字段只有在输入字段Flag不为空时才增加 ]
    char ErrNo[9]; //返回码 [ 0――成功； -1――失败 1000—开始异步查询 1001—异步等待 ]
    char ErrInfo[257]; //返回错误信息
} QueryGroupFungiblePayBillInfoOut;


    
/*集团代付帐单查询*/
class IB_COM_QueryGroupFungiblePayBill:public IB_COM_BASE
{
public:
    IB_COM_QueryGroupFungiblePayBill()
    {
        m_errPrefix += "IB_COM_QueryGroupFungiblePayBill" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //ErrNo 0-成功 1-没有符合条件的记录 其他-失败
    bool IB_QueryGroupFungiblePayBill( const QueryGroupFungiblePayBillInfo& data, QueryGroupFungiblePayBillInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<BillAccount>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<CustMemberBill>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<BillAccount>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<CustMemberBill>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QueryGroupFungiblePayBillInfo& data);
    bool getCallResult(HSendMsg *rcv, QueryGroupFungiblePayBillInfoOut& res);
    


};
#endif /*__IB_COM_QUERYGROUPFUNGIBLEPAYBILL_H__*/

