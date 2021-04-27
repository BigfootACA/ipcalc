#include"stdafx.h"
#include"ipcalc.h"
#include"../../ipv4calc/ipv4calc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BEGIN_MESSAGE_MAP(IPCalc,CWinApp)
END_MESSAGE_MAP()
IPCalc::IPCalc(){}
IPCalc theApp;
BOOL IPCalc::InitInstance(){
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize=sizeof(InitCtrls);
	InitCtrls.dwICC=ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	CShellManager*sm=new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	IPv4Calc d;
	m_pMainWnd=&d;
	if(d.DoModal()==-1)TRACE(traceAppMsg,0,"警告: 对话框创建失败。\n");
	if(sm!=nullptr)delete sm;
	#if !defined(_AFXDLL)&&!defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
	#endif
	return FALSE;
}
