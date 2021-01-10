#ifndef __IM_COM_IMLOCKSIMCARD_H__
#define __IM_COM_IMLOCKSIMCARD_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMLockSimcardInfo
{
    char Region[7]; //地区编码
    char OrgID[33]; //单位编码
    char OperID[33]; //操作员工号
    char ICCID[33]; //SIM卡号
    char sRecOID[33]; //业务受理流水号
    char iPrice[17]; //实际销售价格
} IMLockSimcardInfo;


typedef struct tag_IMLockSimcardInfoOut
{
    char RetCode[7]; //错误代码 [ 成功1 失败-1 ]
    char ErrMsg[513]; //操作失败原因
} IMLockSimcardInfoOut;


    
/*SIM卡锁定*/
class IM_COM_IMLockSimcard:public IM_COM_BASE
{
public:
    IM_COM_IMLockSimcard()
    {
        m_errPrefix += "IM_COM_IMLockSimcard" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IM_IMLockSimcard( const IMLockSimcardInfo& data, IMLockSimcardInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用




private:
    bool setCallInfo(HRecvMsg *send, const IMLockSimcardInfo& data);
    bool getCallResult(HSendMsg *rcv, IMLockSimcardInfoOut& res);
    


};
#endif /*__IM_COM_IMLOCKSIMCARD_H__*/

