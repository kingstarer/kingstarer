#ifndef __IB_COM_MOVEINTEGRAL_H__
#define __IB_COM_MOVEINTEGRAL_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_MoveIntegralInfo
{
    char Businum[21]; //业务流水号
    char Outcustid[15]; //转出客户编码 [ 转出产品类型为全球通号码时使用 ]
    char OutServnumber[21]; //转出服务号码 [ 转出产品类型为动感地带号码,神州行号码，神州大众卡号码时使用 ]
    char OutProdtype[33]; //转出产品类型 [ 0:全球通号码 1:动感地带号码 2.神州行号码 3.神州大众卡号码 ]
    char Region[9]; //地区编码
    char Incustid[15]; //转入客户编码
    char InServnumber[21]; //转入服务号码 [ 转入产品类型为动感地带号码,神州行号码，神州大众卡号码时使用 ]
    char InProdtype[33]; //转入产品类型 [ 0:全球通号码 1:动感地带号码 2.神州行号码 3.神州大众卡号码 ]
    char Oprcode[21]; //操作员
    char Intchgrsn[9]; //积分变更原因
    char Moveintegral[15]; //转移积分
    char Movecycle[7]; //积分周期 [ YYYY ]
    char Unit[33]; //营业厅
} MoveIntegralInfo;


typedef struct tag_MoveIntegralInfoOut
{
    char ErrNo[9]; //返回码 [ 0――成功；其他――失败 ]
    char ErrInfo[257]; //返回错误信息
} MoveIntegralInfoOut;


    
/*积分转移*/
class IB_COM_MoveIntegral:public IB_COM_BASE
{
public:
    IB_COM_MoveIntegral()
    {
        m_errPrefix += "IB_COM_MoveIntegral" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IB_MoveIntegral( const MoveIntegralInfo& data, MoveIntegralInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用




private:
    bool setCallInfo(HRecvMsg *send, const MoveIntegralInfo& data);
    bool getCallResult(HSendMsg *rcv, MoveIntegralInfoOut& res);
    


};
#endif /*__IB_COM_MOVEINTEGRAL_H__*/

