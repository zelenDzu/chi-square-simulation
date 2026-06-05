#pragma once
#include "afxdialogex.h"


// Диалоговое окно CPValuesDialog

class CPValuesDialog : public CDialog
{
	DECLARE_DYNAMIC(CPValuesDialog)

public:
	CPValuesDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CPValuesDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVALUES_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	int m_a0{1};
	int m_b0{1};
	int m_k0{1};

	int m_a1{1};
	int m_b1{1};
	int m_k1{1};

	int m_sample_size{100};
	int m_pvalues_sample_size{1000};
	int m_method{};


};
