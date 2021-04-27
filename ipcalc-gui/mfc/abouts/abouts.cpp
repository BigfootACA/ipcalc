#include"../../global/stdafx.h"
#include"../../global/resource.h"
#include"abouts.h"
#include"afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BEGIN_MESSAGE_MAP(Abouts,CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()
Abouts::Abouts(CWnd*p):CDialogEx(ABOUTS,p){m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);}
void Abouts::DoDataExchange(CDataExchange*p){CDialogEx::DoDataExchange(p);}
void Abouts::OnPaint(){
	if(!IsIconic()){CDialogEx::OnPaint();return;}
	CPaintDC dc(this);
	SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);
	CRect r;
	GetClientRect(&r);
	dc.DrawIcon(
		r.Width()-GetSystemMetrics(SM_CXICON)+1,
		r.Height()-GetSystemMetrics(SM_CYICON)+1,
		m_hIcon
	);
}
BOOL Abouts::OnInitDialog(){
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);
	return TRUE;
}
HCURSOR Abouts::OnQueryDragIcon(){return static_cast<HCURSOR>(m_hIcon);}
