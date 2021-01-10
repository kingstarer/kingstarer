#ifndef __IM_COM_IMQRYWAREDETAIL_H__
#define __IM_COM_IMQRYWAREDETAIL_H__
#include "IM_COM_BASE.h"
#include "view/SendMsg.h"
__USING_WILLOW_NS

typedef struct tag_IMQryWareDetailInfo
{
    char Accoid[15]; //帐号
    char StartTime[15]; //开始时间 [ Yyyymmddhh24miss ]
    char EndTime[15]; //结束时间 [ Yyyymmddhh24miss ]
    char qryType[2]; //查询类型 [ 0全部，1只要余额增加 ]
} IMQryWareDetailInfo;


typedef struct tag_Ucid
{
    char ResSerial[1]; //商品序号
    char ResType[1]; //商品标识
    char Status[1]; //商品状态
    char Price[1]; //商品价格
    char UCID[1]; //UCID
} Ucid;

typedef struct tag_IMQryWareDetailInfoOut
{
    vector<Ucid> UcidArray; //ucid数组
    char RetCode[1]; //错误代码 [ 失败-1 ]
    char ErrMsg[1]; //操作失败原因
} IMQryWareDetailInfoOut;


    
/*集团代付帐单查询*/
class IM_COM_IMQryWareDetail:public IM_COM_BASE
{
public:
    IM_COM_IMQryWareDetail()
    {
        m_errPrefix += "IM_COM_IMQryWareDetail" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败
    //ErrNo 0-成功 1-没有符合条件的记录 其他-失败
    bool IM_IMQryWareDetail( const IMQryWareDetailInfo& data, IMQryWareDetailInfoOut& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
    bool getHDataNodeResult(vector<Ucid>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<Ucid>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const IMQryWareDetailInfo& data);
    bool getCallResult(HSendMsg *rcv, IMQryWareDetailInfoOut& res);
    


};
#endif /*__IM_COM_IMQRYWAREDETAIL_H__*/

