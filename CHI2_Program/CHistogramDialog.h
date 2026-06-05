#pragma once
#include "afxdialogex.h"


// Диалоговое окно CHistogramDialog

class CHistogramDialog : public CDialog
{
	DECLARE_DYNAMIC(CHistogramDialog)

public:
	CHistogramDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CHistogramDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM_DIALOG };
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
	int m_method{};
};
