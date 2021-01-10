#ifndef __IM_COM_IMUNLOCKSIMCARD_H__
#define __IM_COM_IMUNLOCKSIMCARD_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMUnLockSimcardInfo
{
    char Region[7]; //地区编码
    char OrgID[33]; //单位编码
    char OperID[33]; //操作员工号
    char ICCID[33]; //SIM卡号
    char OprType[3]; //操作动作 [ (0 激活 1 取消) ]
    char RecOID[33]; //业务受理流水号
    char Price[17]; //实际销售价格
} IMUnLockSimcardInfo;


typedef struct tag_IMUnLockSimcardInfoOut
{
    char RetCode[7]; //错误代码 [ 成功1 失败-1 ]
    char ErrMsg[513]; //操作失败原因
} IMUnLockSimcardInfoOut;


    
/*SIM卡锁定释放*/
class IM_COM_IMUnLockSimcard:public IM_COM_BASE
{
public:
    IM_COM_IMUnLockSimcard()
    {
        m_errPrefix += "IM_COM_IMUnLockSimcard" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IM_IMUnLockSimcard( const IMUnLockSimcardInfo& data, IMUnLockSimcardInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用




private:
    bool setCallInfo(HRecvMsg *send, const IMUnLockSimcardInfo& data);
    bool getCallResult(HSendMsg *rcv, IMUnLockSimcardInfoOut& res);
    


};
#endif /*__IM_COM_IMUNLOCKSIMCARD_H__*/

