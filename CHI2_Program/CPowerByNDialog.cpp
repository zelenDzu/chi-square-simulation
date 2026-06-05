// CPowerByNDialog.cpp: файл реализации
//

#include "pch.h"
#include "CHI2_Program.h"
#include "afxdialogex.h"
#include "CPowerByNDialog.h"


// Диалоговое окно CPowerByNDialog

IMPLEMENT_DYNAMIC(CPowerByNDialog, CDialog)

CPowerByNDialog::CPowerByNDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_POWER_BY_N_DIALOG, pParent)
    , m_nstep(0)
{

}

CPowerByNDialog::~CPowerByNDialog()
{
}

void CPowerByNDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_A0, m_a0);
    DDX_Text(pDX, IDC_EDIT_B0, m_b0);
    DDX_Text(pDX, IDC_EDIT_K0, m_k0);
    DDX_Text(pDX, IDC_EDIT_A1, m_a1);
    DDX_Text(pDX, IDC_EDIT_B1, m_b1);
    DDX_Text(pDX, IDC_EDIT_K1, m_k1);
    DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);
    DDX_Text(pDX, IDC_EDIT_PVALUES_SAMPLE_SIZE, m_pvalue_sample_size);
    DDX_Text(pDX, IDC_EDIT_NMIN, m_nmin);
    DDX_Text(pDX, IDC_EDIT_NMAX, m_nmax);
    DDX_Radio(pDX, IDC_RADIO_BERNOULLI, m_method);
    DDX_Text(pDX, IDC_EDIT_NSTEP, m_nstep);
}

void CPowerByNDialog::OnOK()
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

    if (m_a0 == m_a1 && m_b0 == m_b1 && m_k0 == m_k1)
    {
        AfxMessageBox(L"H0 must be != H1");
        return;
    }

    if (m_alpha <= 0 || m_alpha >= 1)
    {
        AfxMessageBox(L"Alpha must be in (0,1).");
        return;
    }

    if (m_pvalue_sample_size <= 0)
    {
        AfxMessageBox(L"Number of p-values must be > 0.");
        return;
    }

    if (m_nmin <= 0 || m_nmax <= 0 || m_nstep <= 0)
    {
        AfxMessageBox(L"n_min, n_max, n_step must be > 0.");
        return;
    }

    if (m_nmin > m_nmax)
    {
        AfxMessageBox(L"n_min must be <= n_max.");
        return;
    }

    CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CPowerByNDialog, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CPowerByNDialog

