#ifndef __IB_COM_INTEGRATIONCALFEE_H__
#define __IB_COM_INTEGRATIONCALFEE_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_YxPlanList
{
    char YxPlanId[15]; //营销方案编号 [ 旧营销方案体系为：营销方案编码 新产品模型为：计费编码 ]
    char StratTime[15]; //生效时间 [ YYYYMMDDHHMMSS ]
    char StopTime[15]; //失效时间 [ YYYYMMDDHHMMSS ]
} YxPlanList;

typedef struct tag_ProdList
{
    char ProdId[15]; //产品编号
    char StratTime[15]; //生效时间 [ YYYYMMDDHHMMSS ]
    char StopTime[15]; //失效时间 [ YYYYMMDDHHMMSS ]
} ProdList;

typedef struct tag_IntegrationCalFeeInfo
{
    char subsid[15]; //用户编号
    char recflag[5]; //业务标识 [ 0----正常业务 1----回退业务 ]
    vector<ProdList> ProdListArray; //产品申请列表
    vector<YxPlanList> YxPlanListArray; //营销方案申请列表
    char TradeType[33]; //业务类型
    char ActiveTime[15]; //激活时间 [ YYYYMMDDHHMMSS ]
} IntegrationCalFeeInfo;


typedef struct tag_CalFeeResultList
{
    char acctid[257]; //帐单科目名称
    char recamt[17]; //实收金额 [ 以分为单位 ]
    char subamt[17]; //减免金额 [ 以分为单位 ]
} CalFeeResultList;

typedef struct tag_IntegrationCalFeeInfoOut
{
    vector<CalFeeResultList> CalFeeResultListArray; //算费结果列表
    char ErrNo[9]; //返回码 [ 0-成功; 1-用户不存在; -1—失败 ]
    char ErrInfo[257]; //返回错误信息
} IntegrationCalFeeInfoOut;


    
/*智能网算费接口*/
class IB_COM_IntegrationCalFee:public IB_COM_BASE
{
public:
    IB_COM_IntegrationCalFee()
    {
        m_errPrefix += "IB_COM_IntegrationCalFee" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IB_IntegrationCalFee( const IntegrationCalFeeInfo& data, IntegrationCalFeeInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<ProdList>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<YxPlanList>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<CalFeeResultList>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<ProdList>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<YxPlanList>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<CalFeeResultList>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IntegrationCalFeeInfo& data);
    bool getCallResult(HSendMsg *rcv, IntegrationCalFeeInfoOut& res);
    


};
#endif /*__IB_COM_INTEGRATIONCALFEE_H__*/

