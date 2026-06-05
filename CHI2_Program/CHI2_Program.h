
// CHI2_Program.h : main header file for the CHI2_Program application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCHI2ProgramApp:
// See CHI2_Program.cpp for the implementation of this class
//

class CCHI2ProgramApp : public CWinApp
{
public:
	CCHI2ProgramApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCHI2ProgramApp theApp;
