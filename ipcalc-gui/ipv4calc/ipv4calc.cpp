#include"../global/stdafx.h"
#include"../global/resource.h"
#include"../global/ipcalc.h"
#include"../abouts/abouts.h"
#include"../../libipcalculator/ipv4.h"
#include"ipv4calc.h"
#include"afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BEGIN_MESSAGE_MAP(IPv4Calc,CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(PREFIX,      &IPv4Calc::OnChangePrefix)
	ON_BN_CLICKED(IPV4_COMMIT,&IPv4Calc::OnClickCommit)
	ON_BN_CLICKED(IPV4_CLEAR, &IPv4Calc::Clear)
	ON_BN_CLICKED(BTN_ABOUT,  &IPv4Calc::OnClickAbout)
	ON_BN_CLICKED(BTN_EXIT,   &IPv4Calc::OnClose)
	ON_NOTIFY(UDN_DELTAPOS,SPIN_PREFIX,&IPv4Calc::OnDeltaposPrefix)
	ON_NOTIFY(IPN_FIELDCHANGED,NETMASK,&IPv4Calc::OnChangeNetmask)
END_MESSAGE_MAP()
static CString unknown=L"未知";
static CString AnsiToUnicode(char*a,int len){
	CString str;
	int l=(len>0)?len:(MultiByteToWideChar(CP_ACP,NULL,a,(int)strlen(a),NULL,0));
	wchar_t*s=new wchar_t[l+1];
	MultiByteToWideChar(CP_ACP,NULL,a,(int)strlen(a),s,l);
	s[l]='\0';
	str=s;
	delete s;
	return str;
}
static char*UnicodeToAnsi(const wchar_t*wchar,char*chr,int length){WideCharToMultiByte(CP_ACP,0,wchar,-1,chr,length,NULL,NULL);return chr;}
IPv4Calc::IPv4Calc(CWnd*p):CDialogEx(IPV4CALC,p){m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);}
void IPv4Calc::DoDataExchange(CDataExchange*p){CDialogEx::DoDataExchange(p);}
void IPv4Calc::set_ulong(int id,unsigned long c){CString ss;ss.Format(L"%lu",c);SetDlgItemText(id,ss);}
void IPv4Calc::set_string(int id,char*c){CString ss=AnsiToUnicode(c,0);SetDlgItemText(id,ss);}
void IPv4Calc::set_string_free(int id,char*c){set_string(id,c);free_string(c);}
#define set_ip_dec(id,ip)set_string_free(id,ipv4_addr_to_string(ip));
#define set_ip_bin(id,ip)set_string_free(id,ipv4_addr_to_bin_string(ip));
#define set_ip_hex(id,ip)set_string_free(id,ipv4_addr_to_hex_string(ip));
void IPv4Calc::set_ip(int id_dec,int id_hex,int id_bin,struct ipv4_addr*ip){set_ip_dec(id_dec,ip);set_ip_hex(id_hex,ip);set_ip_bin(id_bin,ip);}
void IPv4Calc::set_type(CString c){ CString s;s.Format(L"类型: %s",c);SetDlgItemText(LBL_IP_TYPE,s);}
void IPv4Calc::set_class(CString c){CString s;s.Format(L"类别: %s",c);SetDlgItemText(LBL_IP_CLASS,s);}
void IPv4Calc::set_group(CString c){CString s;s.Format(L"分配: %s",c);SetDlgItemText(LBL_IP_GROUP,s);}
void IPv4Calc::Clear(){
	CString
		dec=L"0.0.0.0",
		hex=L"00.00.00.00",
		bin=L"00000000.00000000.00000000.00000000";
	SetDlgItemText(EDIT_DEC_NETWORK,dec);   SetDlgItemText(EDIT_HEX_NETWORK,hex);   SetDlgItemText(EDIT_BIN_NETWORK,bin);
	SetDlgItemText(EDIT_DEC_NETMASK,dec);   SetDlgItemText(EDIT_HEX_NETMASK,hex);   SetDlgItemText(EDIT_BIN_NETMASK,bin);
	SetDlgItemText(EDIT_DEC_WILDCARD,dec);  SetDlgItemText(EDIT_HEX_WILDCARD,hex);  SetDlgItemText(EDIT_BIN_WILDCARD,bin);
	SetDlgItemText(EDIT_DEC_HOST_MIN,dec);  SetDlgItemText(EDIT_HEX_HOST_MIN,hex);  SetDlgItemText(EDIT_BIN_HOST_MIN,bin);
	SetDlgItemText(EDIT_DEC_HOST_MAX,dec);  SetDlgItemText(EDIT_HEX_HOST_MAX,hex);  SetDlgItemText(EDIT_BIN_HOST_MAX,bin);
	SetDlgItemText(EDIT_DEC_BROADCAST,dec); SetDlgItemText(EDIT_HEX_BROADCAST,hex); SetDlgItemText(EDIT_BIN_BROADCAST,bin);
	SetDlgItemText(EDIT_DEC_IP_ADDRESS,dec);SetDlgItemText(EDIT_HEX_IP_ADDRESS,hex);SetDlgItemText(EDIT_BIN_IP_ADDRESS,bin);
	SetDlgItemText(EDIT_CIDR,L"0.0.0.0/0"); SetDlgItemText(EDIT_HOST_COUNT,L"0");
	SetDlgItemText(IP_ADDRESS,L"");SetDlgItemText(NETMASK,L"");SetDlgItemText(PREFIX,L"0");
	set_class(unknown);set_group(unknown);set_type(unknown);
}
BOOL IPv4Calc::OnInitDialog(){
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);
	Clear();
	return TRUE;
}
void IPv4Calc::OnPaint(){
	if(!IsIconic()){CDialogEx::OnPaint();return;}
	CPaintDC dc(this);
	SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);
	CRect r;
	GetClientRect(&r);
	dc.DrawIcon(
		(r.Width()-GetSystemMetrics(SM_CXICON)+1)/2,
		(r.Height()-GetSystemMetrics(SM_CYICON)+1)/2,
		m_hIcon
	);
}
HCURSOR IPv4Calc::OnQueryDragIcon(){return static_cast<HCURSOR>(m_hIcon);}
void IPv4Calc::OnClickCommit(){
	CString pr,nm,ip,oo,_class,_type,_group;
	GetDlgItemText(PREFIX,pr);GetDlgItemText(NETMASK,nm);GetDlgItemText(IP_ADDRESS,ip);
	int prefix=StrToInt(pr);
	char rr[32];
	struct ipv4_net*net=ipv4_new_net_obj();
	memset(&rr,0,32);UnicodeToAnsi(nm,rr,31);if(!ipv4_string_to_addr_x(rr,net->mask)){ipv4_free_net(net);goto errmask;}
	memset(&rr,0,32);UnicodeToAnsi(ip,rr,31);if(!ipv4_string_to_addr_x(rr,net->net)){ipv4_free_net(net);goto errip;}
	if(prefix<1||prefix>32||ipv4_mask_to_prefix(net->mask)!=prefix||!ipv4_addr_is_mask(net->mask))goto errmask;
	set_ip(EDIT_DEC_IP_ADDRESS,EDIT_HEX_IP_ADDRESS,EDIT_BIN_IP_ADDRESS,net->net);
	set_ip(EDIT_DEC_NETWORK,   EDIT_HEX_NETWORK,   EDIT_BIN_NETWORK,   ipv4_get_net_addr_net(net));
	set_ip(EDIT_DEC_NETMASK,   EDIT_HEX_NETMASK,   EDIT_BIN_NETMASK,   net->mask);
	set_ip(EDIT_DEC_WILDCARD,  EDIT_HEX_WILDCARD,  EDIT_BIN_WILDCARD,  ipv4_mask_to_wildcard(net->mask));
	set_ip(EDIT_DEC_BROADCAST, EDIT_HEX_BROADCAST, EDIT_BIN_BROADCAST, ipv4_get_brd_addr_net(net));
	set_ip(EDIT_DEC_HOST_MIN,  EDIT_HEX_HOST_MIN,  EDIT_BIN_HOST_MIN,  ipv4_get_min_addr_net(net));
	set_ip(EDIT_DEC_HOST_MAX,  EDIT_HEX_HOST_MAX,  EDIT_BIN_HOST_MAX,  ipv4_get_max_addr_net(net));
	set_string_free(EDIT_CIDR,ipv4_net_to_string(net));
	set_ulong(EDIT_HOST_COUNT,ipv4_get_count_addr_net(net));
	switch(ipv4_get_addr_class(net->net)){
		case IPV4_CLASS_A:set_class(L"A类地址");break;
		case IPV4_CLASS_B:set_class(L"B类地址");break;
		case IPV4_CLASS_C:set_class(L"C类地址");break;
		case IPV4_CLASS_D:set_class(L"D类地址");break;
		case IPV4_CLASS_E:set_class(L"E类地址");break;
		case IPV4_CLASS_M:set_class(L"广播地址");break;
		default:          set_class(unknown);break;
	}
	switch(ipv4_get_addr_group(net->net)){
		case IPV4_GRP_ANY:      set_group(L"RFC1122 此主机");break;
		case IPV4_GRP_LOOP:     set_group(L"RFC1122 环回");break;
		case IPV4_GRP_CGN:      set_group(L"RFC6598 共享地址空间");break;
		case IPV4_GRP_CHAIN:    set_group(L"RFC3927 本地链路");break;
		case IPV4_GRP_BENCHMARK:set_group(L"RFC2544 性能测试");break;
		case IPV4_GRP_IP6TO4:   set_group(L"RFC3068 6to4中继选任播");break;
		case IPV4_GRP_TEST_1:   set_group(L"RFC5737 文档(TEST-NET-1)");break;
		case IPV4_GRP_TEST_2:   set_group(L"RFC5737 文档(TEST-NET-2)");break;
		case IPV4_GRP_TEST_3:   set_group(L"RFC5737 文档(TEST-NET-3)");break;
		case IPV4_GRP_IETF:     set_group(L"RFC6890 IETF协议分配");break;
		case IPV4_GRP_LOCAL:    set_group(L"RFC1918 私网地址");break;
		case IPV4_GRP_EXPER:    set_group(L"RFC1112 保留");break;
		case IPV4_GRP_PUBLIC:   set_group(L"公网地址");break;
		case IPV4_GRP_BROADCAST:set_group(L"广播地址");break;
		case IPV4_GRP_MULTICAST:set_group(L"多播地址");break;
		default:                set_group(unknown);break;
	}
	switch(ipv4_get_addr_type(net,net->net)){
		case IPV4_TYPE_NET:      set_type(L"网络地址");break;
		case IPV4_TYPE_HOST:     set_type(L"主机地址");break;
		case IPV4_TYPE_BROADCAST:set_type(L"广播地址");break;
		default:                 set_type(unknown);break;

	}
	return;
	errmask:MessageBox(L"请输入正确的子网掩码或者掩码前缀!",L"输入错误",MB_ICONWARNING);return;
	errip:MessageBox(L"请输入正确的IP地址!",L"输入错误",MB_ICONWARNING);return;
}
void IPv4Calc::OnChangeNetmask(NMHDR*pn,LRESULT*pr){
	CString ss,oo;
	GetDlgItemText(NETMASK,ss);
	char rr[32];
	memset(&rr,0,32);
	UnicodeToAnsi(ss,rr,31);
	struct ipv4_addr*mask=ipv4_string_to_addr(rr);
	if(!mask)return;
	oo.Format(L"%d",ipv4_mask_to_prefix(mask));
	ipv4_free_addr(mask);
	SetDlgItemText(PREFIX,oo);
	*pr=0;
}
void IPv4Calc::OnChangePrefix(){
	CString str;
	GetDlgItemText(PREFIX,str);
	int s=StrToInt(str);
	if(s<0){s=0;SetDlgItemText(PREFIX,L"0");}
	if(s>32){s=32;SetDlgItemText(PREFIX,L"32");}
	CString ss;
	if(s==0)ss=L"";
	else{
		char*c=ipv4_addr_to_string(ipv4_prefix_to_mask(s));
		ss=AnsiToUnicode(c,0);
		free_string(c);
	}
	SetDlgItemText(NETMASK,ss);
}
void IPv4Calc::OnDeltaposPrefix(NMHDR*pn,LRESULT*pr){
	LPNMUPDOWN pu=reinterpret_cast<LPNMUPDOWN>(pn);
	CString str,oo;
	GetDlgItemText(PREFIX,str);
	int s=StrToInt(str)-(pu->iDelta);
	if(s<1||s>32)return;
	oo.Format(L"%d",s);
	SetDlgItemText(PREFIX,oo);
	*pr=0;
}
void IPv4Calc::OnClickAbout(){Abouts ab;ab.DoModal();}
BOOL IPv4Calc::PreTranslateMessage(MSG*p){return(p->message==WM_KEYDOWN&&p->wParam==VK_ESCAPE)?TRUE:CDialog::PreTranslateMessage(p);}
void IPv4Calc::OnClose(){EndDialog(0);}
