#ifndef __IB_COM_<COMMON>_H__
#define __IB_COM_<COMMON>_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

<InParamDefine>
<OutParamDefine>
	
/*<�������>*/
class IB_COM_<Common>:public IB_COM_BASE
{
public:
    IB_COM_<Common>()
    {
        m_errPrefix += "IB_COM_<Common>" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_<Common>( const <inparam>& data, <outparam>& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
<getHDataNodeResultDefine>

<ParseArrayToNodeDefine>

private:
    bool setCallInfo(HRecvMsg *send, const <inparam>& data);
    bool getCallResult(HSendMsg *rcv, <outparam>& res);
    


};
#endif /*__IB_COM_<COMMON>_H__*/
