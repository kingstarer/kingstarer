#ifndef __IB_COM_CCCREATEACCTINFO_H__
#define __IB_COM_CCCREATEACCTINFO_H__
#include "IB_COM_BASE.h"
__USING_WILLOW_NS

typedef struct tag_bmaillist
{
    char oid[16]; //oid
    char region[7]; //region
    char billtype[18]; //billtype
    char langtype[18]; //langtype
    char mailtype[18]; //mailtype
    char mailaddr[130]; //mailaddr
    char postcode[8]; //postcode
    char linkman[66]; //linkman
    char linkphone[22]; //linkphone
    char status[3]; //status
    char ca_billmail_idu[3]; //ca_billmail_idu
    char statusdate[15]; //statusdate
} bmaillist;

typedef struct tag_stltypelist
{
    char oid[16]; //oid
    char paytype[18]; //paytype
    char bankid[18]; //bankid
    char bankacct[34]; //bankacct
    char accttype[18]; //accttype
    char acctname[130]; //acctname
    char payorder[5]; //payorder
    char certid[34]; //certid
    char contractid[72]; //contractid
    char drawtype[18]; //drawtype
    char drawamt[16]; //drawamt
    char trigamt[16]; //trigamt
    char status[3]; //status
    char startdate[15]; //startdate
    char settletype_idu[3]; //settletype_idu
} stltypelist;

typedef struct tag_acctlist
{
    char acctid[16]; //acctid
    char prepaytype[18]; //prepaytype
    char acctname[130]; //acctname
    char accttype[18]; //accttype
    char status[18]; //status
    char acct_idu[3]; //acct_idu
    char orgid[34]; //orgid
    char invoicetel[22]; //invoicetel
    char notifytype[3]; //notifytype
    char notifyvalue[12]; //notifyvalue
    char invprinttype[18]; //invprinttype
    char overdraft[18]; //overdraft
    char notes[258]; //notes
    char region[7]; //region
    char custid[16]; //custid
    char groupacctid[16]; //groupacctid
    char controlscheme[18]; //controlscheme
    char hbprintinv[3]; //hbprintinv
} acctlist;

typedef struct tag_CCCreateAcctInfoInfo
{
    char ordid[16]; //ordid
    char formnum[34]; //formnum
    char contacttype[18]; //contacttype
    char operid[18]; //operid
    char recdate[15]; //recdate
    char recdefid[34]; //recdefid
    char idu[3]; //idu
    char custid[16]; //custid
    char region[7]; //region
    vector<acctlist> acctlistArray; //acctlist
    vector<stltypelist> stltypelistArray; //stltypelist
    vector<bmaillist> bmaillistArray; //bmaillist
} CCCreateAcctInfoInfo;


typedef struct tag_CCCreateAcctInfoInfoOut
{
    char acctid[16]; //�˻�ID
    char ErrNo[9]; //������ [ 0�D�D�ɹ��������D�Dʧ�� ]
    char ErrInfo[257]; //���ش�����Ϣ
} CCCreateAcctInfoInfoOut;


    
/*������־��ѯ�ӿ�*/
class IB_COM_CCCreateAcctInfo:public IB_COM_BASE
{
public:
    IB_COM_CCCreateAcctInfo()
    {
        m_errPrefix += "IB_COM_CCCreateAcctInfo" + m_errSuffix;
    }

public:
    //true���óɹ�(��ʱ�������ݲ�һ����ȷ) false����ʧ�� 
    //�ɹ�ʱ�Ὣcics������Ϣ����ڳ����� ʧ���򲻻�
    bool IB_CCCreateAcctInfo( const CCCreateAcctInfoInfo& data, CCCreateAcctInfoInfoOut& res );
    
    //���Խṹ��������xml���Ļ�ת�ĺ����ŵ����Ϊ�˷����Ժ�����������Ե���
    bool getHDataNodeResult(vector<acctlist>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<stltypelist>& nodeinfo, IN HDataNode* node, const string& destname);
    bool getHDataNodeResult(vector<bmaillist>& nodeinfo, IN HDataNode* node, const string& destname);


    bool ParseArrayToNode(HDataNode *node, const vector<acctlist>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<stltypelist>& nodeinfo);
    bool ParseArrayToNode(HDataNode *node, const vector<bmaillist>& nodeinfo);


private:
    bool setCallInfo(HRecvMsg *send, const CCCreateAcctInfoInfo& data);
    bool getCallResult(HSendMsg *rcv, CCCreateAcctInfoInfoOut& res);
    


};
#endif /*__IB_COM_CCCREATEACCTINFO_H__*/

