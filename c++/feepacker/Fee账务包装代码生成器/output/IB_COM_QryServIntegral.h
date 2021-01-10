#ifndef __IB_COM_QRYSERVINTEGRAL_H__
#define __IB_COM_QRYSERVINTEGRAL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_QryServIntegralInfo
{
    char Region[9]; //地区编码
    char Servnumber[21]; //服务号码 [ 产品类型为动感地带号码, 神州行号码，神州大众卡号码时使用 ]
    char CustID[15]; //客户编号 [ 产品类型为全球通号码时使用 ]
    char Prodtype[33]; //产品类型 [ 0:全球通号码 1:动感地带号码 2:神州行号码 3:神州大众卡号码 4:集团客户（当产品类型指定为集团客户时，只根据CustID来处理，这时CustID字段不能为空） ]
    char Integralcyc[7]; //积分周期 [ YYYY "-1"------查询该客户所有积分周期的积分信息 ]
    char Brandscoretype[33]; //全品牌积分类型 [ 0;基本积分 1:优惠积分 -1:不区别基本积分和优惠积分 ]
} QryServIntegralInfo;


typedef struct tag_ACustIntegral
{
    char ServNumber[21]; //服务号码 [ 如果输入参数中的prodType为4，则该字段值为集团客户下有积分变更的各产品的服务号码；如果prodType不为4，则为变更客户的服务号码 ]
    char Totalintegral[15]; //总积分
    char Integralcyc[7]; //积分周期 [ 特殊说明：积分周期为0时可用积分表示为优惠积分，为正常年时可用积分还是基本积分中的可用积分 ]
    char Availintegral[15]; //可用积分
    char Moveintegral[15]; //转移积分
    char Unavailintegral[15]; //失效积分
    char Saleintegral[15]; //购买积分
    char Updatetime[15]; //最新更新时间
    char Stoptime[15]; //失效时间 [ 如果是优惠积分时，存放优惠积分有效期 ]
    char Brandscoretype[33]; //全品牌积分类型 [ 0;基本积分 1:优惠积分 -1:不区别基本积分和优惠积分 ]
} ACustIntegral;

typedef struct tag_QryServIntegralInfoOut
{
    vector<ACustIntegral> ACustIntegralArray; //积分信息数组
    char ErrNo[9]; //返回码 [ 0――成功； 其他――失败 ]
    char ErrInfo[257]; //返回错误信息
} QryServIntegralInfoOut;


    
/*查客户积分信息*/
class IB_COM_QryServIntegral:public IB_COM_BASE
{
public:
    IB_COM_QryServIntegral()
    {
        m_errPrefix += "IB_COM_QryServIntegral" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IB_QryServIntegral( const QryServIntegralInfo& data, QryServIntegralInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<ACustIntegral>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<ACustIntegral>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const QryServIntegralInfo& data);
    bool getCallResult(HSendMsg *rcv, QryServIntegralInfoOut& res);
    


};
#endif /*__IB_COM_QRYSERVINTEGRAL_H__*/

