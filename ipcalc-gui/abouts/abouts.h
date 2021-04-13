#ifndef _ABOUTS_H
#define _ABOUTS_H
class Abouts:public CDialogEx{
	public:
		Abouts(CWnd*p=nullptr);
		#ifdef AFX_DESIGN_TIME
		enum{IDD=ABOUTS};
		#endif
	protected:
		HICON m_hIcon;
		virtual void DoDataExchange(CDataExchange*pDX);
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
};
#endif
