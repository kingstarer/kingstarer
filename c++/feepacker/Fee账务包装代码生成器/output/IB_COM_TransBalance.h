#ifndef __IB_COM_TRANSBALANCE_H__
#define __IB_COM_TRANSBALANCE_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_TransBalanceInfo
{
    char Formnum[21]; //缴费流水号
    char FromAccoid[15]; //源帐号
    char ToAccoid[15]; //目的帐号
    char Unit[33]; //营业厅
    char Op[33]; //操作员
    char busicode[33]; //业务类型
    char payReason[21]; //缴费变更类型
} TransBalanceInfo;


typedef struct tag_TransBalanceInfoOut
{
    char ErrNo[9]; //返回码 [ 0――成功； 其他――失败 ]
    char TransFee[15]; //转移金额
    char ErrInfo[257]; //返回错误信息
} TransBalanceInfoOut;


    
/*余额转移*/
class IB_COM_TransBalance:public IB_COM_BASE
{
public:
    IB_COM_TransBalance()
    {
        m_errPrefix += "IB_COM_TransBalance" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IB_TransBalance( const TransBalanceInfo& data, TransBalanceInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用




private:
    bool setCallInfo(HRecvMsg *send, const TransBalanceInfo& data);
    bool getCallResult(HSendMsg *rcv, TransBalanceInfoOut& res);
    


};
#endif /*__IB_COM_TRANSBALANCE_H__*/

