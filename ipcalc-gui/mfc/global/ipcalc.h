#pragma once
#include"resource.h"
class IPCalc:public CWinApp{
	public:IPCalc();
	public:virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
extern IPCalc theApp;
