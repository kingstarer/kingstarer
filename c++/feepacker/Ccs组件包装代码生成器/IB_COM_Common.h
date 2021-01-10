#ifndef __CC_COM_<COMMON>_H__
#define __CC_COM_<COMMON>_H__
#include "CC_COM_BASE.h"
__USING_WILLOW_NS

<InParamDefine>
<OutParamDefine>
	
/*<组件名称>*/
class CC_COM_<Common>:public CC_COM_BASE
{
public:
    CC_COM_<Common>()
    {
        m_errPrefix += "CC_COM_<Common>" + m_errSuffix;
    }

public:
    //true调用成功(此时出参内容不一定正确) false调用失败 
    //成功时会将cics返回信息存放在出参中 失败则不会
    bool CC_<Common>( const <inparam>& data, <outparam>& res );
    
    //将对结构体数组与xml报文互转的函数放到这里，为了方便以后其它服务可以调用
<getHDataNodeResultDefine>

<ParseArrayToNodeDefine>

private:
    bool setCallInfo(HRecvMsg *send, const <inparam>& data);
    bool getCallResult(HSendMsg *rcv, <outparam>& res);
    


};
#endif /*__CC_COM_<COMMON>_H__*/
