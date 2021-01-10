#ifndef __IM_COM_IMTICKETRESERVE_H__
#define __IM_COM_IMTICKETRESERVE_H__
#include "IM_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_IMTicketReserveInfo
{
    char Region[33]; //地区编码
    char OrgID[33]; //单位编码
    char OperID[33]; //操作员工号
    char RecOID[33]; //业务流水号
    char Quantity[33]; //冲值券数量 [ （张数不是包的数量） ]
    char Restype[33]; //冲值券资源类型 [ （小类） ]
    char Agentid[33]; //代理商编码
    char PubKey[257]; //销售员解密公钥
} IMTicketReserveInfo;


typedef struct tag_PacketInfo
{
    char PkgSeq[33]; //充值券包编号
    char PkgKing[33]; //包种类 [ 一万张包、一千张包、一百张包 ]
    char PkgSeqEnd[33]; //批次号
    char ParValue[33]; //充值券包的单价 [ 实际价格 ]
} PacketInfo;

typedef struct tag_IMTicketReserveInfoOut
{
    char FileName[257]; //文件名称
    vector<PacketInfo> PacketInfoArray; //充值券包信息列表
    char RetCode[7]; //错误代码 [ 成功1 失败-1 ]
    char ErrMsg[512]; //操作失败原因
} IMTicketReserveInfoOut;


    
/*充值券包资源预定*/
class IM_COM_IMTicketReserve:public IM_COM_BASE
{
public:
    IM_COM_IMTicketReserve()
    {
        m_errPrefix += "IM_COM_IMTicketReserve" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool IM_IMTicketReserve( const IMTicketReserveInfo& data, IMTicketReserveInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<PacketInfo>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<PacketInfo>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMTicketReserveInfo& data);
    bool getCallResult(HSendMsg *rcv, IMTicketReserveInfoOut& res);
    


};
#endif /*__IM_COM_IMTICKETRESERVE_H__*/

