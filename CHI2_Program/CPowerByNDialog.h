#pragma once
#include "afxdialogex.h"


// Диалоговое окно CPowerByNDialog

class CPowerByNDialog : public CDialog
{
	DECLARE_DYNAMIC(CPowerByNDialog)

public:
	CPowerByNDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CPowerByNDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWER_BY_N_DIALOG };
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
	int m_b1{2};
	int m_k1{1};

	double m_alpha{0.05};
	int m_pvalue_sample_size{10000};
	int m_method{};

	int m_nmin{1};
	int m_nmax{20};
	int m_nstep{1};
};
