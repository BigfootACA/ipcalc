#ifndef _IPCALC_H
#define _IPCALC_H
class IPv4Calc:public CDialogEx{
	public:
		IPv4Calc(CWnd*pParent=nullptr);
		afx_msg void OnChangeNetmask(NMHDR*pn,LRESULT*pr);
		afx_msg void OnDeltaposPrefix(NMHDR*pn,LRESULT*pr);
		afx_msg void OnChangePrefix();
		afx_msg void OnClickCommit();
		afx_msg void OnClickAbout();
		void Clear();
		#ifdef AFX_DESIGN_TIME
		enum{IDD=IPV4CALC};
		#endif
	protected:
		HICON m_hIcon;
		void set_class(CString c);
		void set_type(CString c);
		void set_group(CString c);
		void set_string_free(int id,char*c);
		void set_string(int id,char*c);
		void set_ulong(int id,unsigned long c);
		void set_ip(int id_dec,int id_hex,int id_bin,struct ipv4_addr*ip);
		virtual BOOL PreTranslateMessage(MSG*p);
		virtual void DoDataExchange(CDataExchange*p);
		virtual BOOL OnInitDialog();
		virtual void OnClose();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
};
#endif
