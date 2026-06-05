// CHistogramDialog.cpp: файл реализации
//

#include "pch.h"
#include "CHI2_Program.h"
#include "afxdialogex.h"
#include "CHistogramDialog.h"


// Диалоговое окно CHistogramDialog

IMPLEMENT_DYNAMIC(CHistogramDialog, CDialog)

CHistogramDialog::CHistogramDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HISTOGRAM_DIALOG, pParent)
{

}

CHistogramDialog::~CHistogramDialog()
{
}

void CHistogramDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_A0, m_a0);
	DDX_Text(pDX, IDC_EDIT_B0, m_b0);
	DDX_Text(pDX, IDC_EDIT_K0, m_k0);
	DDX_Text(pDX, IDC_EDIT_A1, m_a1);
	DDX_Text(pDX, IDC_EDIT_B1, m_b1);
	DDX_Text(pDX, IDC_EDIT_K1, m_k1);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_SIZE, m_sample_size);
	DDX_Radio(pDX, IDC_RADIO_BERNOULLI, m_method);
}


void CHistogramDialog::OnOK()
{
    UpdateData(true);

    if (m_a0 <= 0 || m_b0 <= 0 || m_k0 <= 0)
    {
        AfxMessageBox(L"H0: a0, b0, k0 must be > 0.");
        return;
    }

    if (m_k0 > m_a0 || m_k0 > m_b0)
    {
        AfxMessageBox(L"H0: k0 must be <= a0 and <= b0.");
        return;
    }

    if (m_a1 <= 0 || m_b1 <= 0 || m_k1 <= 0)
    {
        AfxMessageBox(L"H1: a1, b1, k1 must be > 0.");
        return;
    }

    if (m_k1 > m_a1 || m_k1 > m_b1)
    {
        AfxMessageBox(L"H1: k1 must be <= a1 and <= b1.");
        return;
    }

    if (m_sample_size <= 0)
    {
        AfxMessageBox(L"Sample size must be > 0.");
        return;
    }

    CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CHistogramDialog, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CHistogramDialog

