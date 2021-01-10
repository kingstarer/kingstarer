#ifndef __CC_COM_<COMMON>_H__
#define __CC_COM_<COMMON>_H__
#include "CC_COM_BASE.h"
__USING_WILLOW_NS

<InParamDefine>
<OutParamDefine>
	
/*<�������>*/
class CC_COM_<Common>:public CC_COM_BASE
{
public:
    CC_COM_<Common>()
    {
        m_errPrefix += "CC_COM_<Common>" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool CC_<Common>( const <inparam>& data, <outparam>& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
<getHDataNodeResultDefine>

<ParseArrayToNodeDefine>

private:
    bool setCallInfo(HRecvMsg *send, const <inparam>& data);
    bool getCallResult(HSendMsg *rcv, <outparam>& res);
    


};
#endif /*__CC_COM_<COMMON>_H__*/
