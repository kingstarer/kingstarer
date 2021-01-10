#ifndef __IB_COM_QUERYBILL_H__
#define __IB_COM_QUERYBILL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_QueryBillInfo
{
    char acctid[15]; //帐户ID [ 帐户ID可为空，但为空时subsid就不可以为空 ]
    char subsid[15]; //用户ID [ 用户ID可为空，但为空时acctid就不可以为空 ]
    char StartCycle[9]; //开始周期 [ 暂为YYYYMM00，有了日帐后两位就不是00了 ]
    char EndCycle[9]; //结束周期 [ 暂为YYYYMM00，有了日帐后两位就不是00了 ]
    char Status[2]; //帐单状态 [ 见下,此项和上两项是互斥的 ]
    char IsBadDebt[2]; //是否显示呆帐费用 [ “2”------是，但只列呆帐费用，用于单独的呆帐回收 ]
    char HowBadDebt[2]; //怎样对待呆帐费用 [ 不需要处理 ]
    char BillType[33]; //返回帐单的形式 [ 1――每帐号周期一条；（只有该项才返回余额明细） ]
    char BillFormatNum[33]; //费用明细代码的组合级别 [ 客户化帐单模板号（custbill_def中定义之template_id） ]
} QueryBillInfo;


typedef struct tag_TipsData
{
    char RealUse[15]; //实际使用 [ 当前真正使用的费用 ]
    char TipsUse[15]; //套餐月费 [ 使用省钱秘笈的费用 ]
} TipsData;

typedef struct tag_SixmonData
{
    char BillCyc[9]; //周期
    char Fee[15]; //费用
} SixmonData;

typedef struct tag_MonData
{
    char BaseFee[15]; //月基本费
    char IncreFee[15]; //增值业务费
    char ComFee[15]; //通信费
    char SpFee[15]; //代收费
    char PaybyFee[15]; //代他人付费 [ 代他人付费不包含在其他帐单项中，即不区分该项的费用组成 ]
    char ReceiveFee[15]; //补收费
    char ViolaFee[15]; //违约金
    char OtherFee[15]; //其他
} MonData;

typedef struct tag_IntDetail
{
    char MonInt[15]; //本月积分
    char ConsumInt[15]; //本月消费积分
    char AwardInt[15]; //本月奖励积分
    char BeforeAvlInt[15]; //上月末剩余可兑换积分
    char AwardedInt[15]; //本月已兑换积分
    char DonateInt[15]; //本月捐赠积分
    char LeftInt[15]; //本月末剩余积分
    char CleanInt[15]; //年底清零积分
} IntDetail;

typedef struct tag_BalanceDetail
{
    char BeforeAmt[15]; //本月初结余
    char ZSAmt[15]; //赠费
    char DiscAmt[15]; //优惠
    char BackFee[15]; //退费
    char PaybyAmt[15]; //他人代付
    char InvalidAmt[15]; //本月失效费用
    char DepositAmt[15]; //本月沉淀话费
    char ThisMonBack[15]; //分月返还
    char ThisMonPay[15]; //本月累计已交费
    char Totalfee[15]; //费用合计
    char AfterAmt[15]; //本月末结余
    char ShouldPay[15]; //本月末应缴
    char WillBackAmt[15]; //分月未返还金额
} BalanceDetail;

typedef struct tag_SpDetail
{
    char SpName[65]; //服务商名称
    char BusCode[33]; //服务代码
    char BusName[65]; //订购业务名称
    char Fee[15]; //费用金额
} SpDetail;

typedef struct tag_FeeDetail
{
    char BillItem[2]; //是否帐单项 [ 1为帐单项目、0为费项 ]
    char FeeTypeId[33]; //费用代码
    char Feename[65]; //费用名称
    char Fee[15]; //费用金额
} FeeDetail;

typedef struct tag_Bill
{
    char Accoid[15]; //帐号 [ 此参数与输入参数同值，如果该输入参数为空，且按用户查询时，并且billtype参数是1时要填具体帐号 ]
    char Subsoid[15]; //用户号 [ 如果是各用户帐单合并到帐号帐单，则为空串（与输入参数BillType相关） ]
    char Cycle[7]; //帐务周期
    char Totalfee[15]; //总费用（费用明细的合计）
    char ShouldPay[15]; //本月应缴 [ 增加 ]
    char LeftInt[15]; //本月末剩余积分 [ 增加 ]
    char isWrtOff[2]; //销帐标识 [ 1已经销帐，0没有销帐 ]
    char Paidamt[15]; //已销费用 [ 已销账金额 ]
    char TipsInfo[1001]; //推荐信息 [ 个性化的套餐资费方案推荐信息 ]
    vector<FeeDetail> FeeDetailArray; //费用明细
    vector<SpDetail> SpDetailArray; //代收信息费明细
    vector<BalanceDetail> BalanceDetailArray; //余额明细
    vector<IntDetail> IntDetailArray; //积分明细
    vector<MonData> MonDataArray; //当月费用结构数组
    vector<SixmonData> SixmonDataArray; //半年费用趋势数据数组
    vector<TipsData> TipsDataArray; //省钱秘笈数据数组
} Bill;

typedef struct tag_QueryBillInfoOut
{
    vector<Bill> BillArray; //帐单数组
    char ErrNo[9]; //返回码 [ 0――成功； 其他――失败 ]
    char ErrInfo[257]; //返回错误信息
} QueryBillInfoOut;


    
/*查帐单接口*/
class IB_COM_QueryBill:public IB_COM_BASE
{
public:
    IB_COM_QueryBill()
    {
        m_errPrefix += "IB_COM_QueryBill" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IB_QueryBill( const QueryBillInfo& data, QueryBillInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<Bill>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<FeeDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<SpDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<BalanceDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<IntDetail>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<MonData>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<SixmonData>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<TipsData>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<Bill>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<FeeDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<SpDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<BalanceDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<IntDetail>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<MonData>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<SixmonData>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<TipsData>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QueryBillInfo& data);
    bool getCallResult(HSendMsg *rcv, QueryBillInfoOut& res);
    


};
#endif /*__IB_COM_QUERYBILL_H__*/

