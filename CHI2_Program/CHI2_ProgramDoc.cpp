
// CHI2_ProgramDoc.cpp : implementation of the CCHI2ProgramDoc class
//

#pragma region Includes

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CHI2_Program.h"
#endif

#include "CHI2_ProgramDoc.h"
#include <propkey.h>

#include "CHistogramDialog.h"
#include "CPValuesDialog.h"
#include "CPowerByNDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma endregion


#pragma region Default(MAP,..)
// CCHI2ProgramDoc

IMPLEMENT_DYNCREATE(CCHI2ProgramDoc, CDocument)
BEGIN_MESSAGE_MAP(CCHI2ProgramDoc, CDocument)
	ON_COMMAND(ID_BUTTON_UPDATE, &CCHI2ProgramDoc::OnButtonUpdate)
	ON_COMMAND(ID_HISTOGRAM, &CCHI2ProgramDoc::OnHistogram)
	ON_COMMAND(ID_PLEVELS, &CCHI2ProgramDoc::OnPlevels)
	ON_COMMAND(ID_POWER_BYN, &CCHI2ProgramDoc::OnPowerByN)
	ON_COMMAND(ID_POWER_BYAPLHA, &CCHI2ProgramDoc::OnPowerByAplha)
END_MESSAGE_MAP()
#pragma endregion


#pragma region Constructor/Destructor

CCHI2ProgramDoc::CCHI2ProgramDoc() noexcept
{
	// TODO: add one-time construction code here
}

CCHI2ProgramDoc::~CCHI2ProgramDoc()
{
	delete s;
	s = nullptr;
}
#pragma endregion


#pragma region OnNewDoc

BOOL CCHI2ProgramDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//Sample::set_seed(42); // если нужно, можно задать сид

	delete s;
	s = nullptr;

	pvalues = Array<double>(0);
	pvalue_alphas = Array<double>(0);
	power_values_by_n = Array<double>(0);
	power_alpha = 0.05;
	power_nmin = 100;
	power_nmax = 150;
	power_nstep = 1;
	sample_size = 100;
	pvalue_sample_size = 10000;


	update_sample();
	update_hist();

	return true;
}

#pragma endregion


#pragma region Modeling Histogram

// Геттеры

const Distribution& CCHI2ProgramDoc::get_d0() const
{
	return d0;
}

const Distribution& CCHI2ProgramDoc::get_d1() const
{
	return d1;
}

const Sample* CCHI2ProgramDoc::get_sample() const
{
	return s;
}

const Chi2Histogram& CCHI2ProgramDoc::get_histogram() const
{
	return h;
}

Method CCHI2ProgramDoc::get_method() const
{
	return method;
}

ProgramMode CCHI2ProgramDoc::get_program_mode() const
{
	return program_mode;
}

int CCHI2ProgramDoc::get_sample_size() const
{
	return sample_size;
}

int CCHI2ProgramDoc::get_pvalue_sample_size() const
{
	return pvalue_sample_size;
}

// Сеттеры

//void CCHI2ProgramDoc::set_d0(const Distribution& new_d0)
//{
//	d0 = new_d0;
//}
//
//void CCHI2ProgramDoc::set_d1(const Distribution& new_d1)
//{
//	d1 = new_d1;
//}
//
//void CCHI2ProgramDoc::set_method(Method new_method)
//{
//	method = new_method;
//}
//
//void CCHI2ProgramDoc::set_program_mode(ProgramMode new_mode)
//{
//	program_mode = new_mode;
//}
//
//void CCHI2ProgramDoc::set_sample_size(int new_size)
//{
//	assert(new_size > 0);
//	sample_size = new_size;
//}
//
//void CCHI2ProgramDoc::set_pvalue_sample_size(int new_size)
//{
//	assert(new_size > 0);
//	pvalue_sample_size = new_size;
//}

// Апдейты

void CCHI2ProgramDoc::update_sample()
{
	delete s;
	s = nullptr;

	switch (method)
	{
	case Bernoulli:
		s = new BernoulliSample{ d1, sample_size };
		break;

	case Inverse:
		s = new InverseSample{ d1, sample_size };
		break;

	default:
		assert(false); //такого не должно быть
		return;
	}
}

void CCHI2ProgramDoc::update_hist()
{
	assert(s != nullptr);

	s->simulate();
	h.set_data(*s, d0);
	h.calc_chi();
}


#pragma endregion 


#pragma region Modeling Pvalues

const Array<double>& CCHI2ProgramDoc::get_pvalues() const
{
	return pvalues;
}

const Array<double>& CCHI2ProgramDoc::get_pvalue_alphas() const
{
	return pvalue_alphas;
}

bool CCHI2ProgramDoc::is_h0_equal_h1() const
{
	return d0 == d1;
}

double CCHI2ProgramDoc::pvalue_emp_dist(double a) const
{
	if (a <= 0)
		return 0;

	if (a >= 1)
		return 1;

	if (pvalues.get_size() == 0)
		return 0;

	int count = 0;

	for (int i = 0; i < pvalues.get_size(); ++i)
	{
		if (pvalues[i] < a)
			++count;
	}

	return (double)count / (double)pvalues.get_size();
}

void CCHI2ProgramDoc::build_alphas_grid(int alpha_count) // по умолчанию 21!
{
	pvalue_alphas = Array<double>(alpha_count);

	for (int i = 0; i < alpha_count; ++i)
	{
		pvalue_alphas[i] = (double)i / (double)(alpha_count - 1);
	}
}

void CCHI2ProgramDoc::generate_pvalues()
{
	assert(sample_size > 0);
	assert(pvalue_sample_size > 0);

	update_sample(); //обновляем метод генерации

	pvalues = Array<double>(pvalue_sample_size);

	for (int i = 0; i < pvalue_sample_size; ++i)
	{
		s->simulate();
		h.set_data(*s, d0);
		h.calc_chi();

		pvalues[i] = h.get_pvalue();
	}

	build_alphas_grid();
}

#pragma endregion


#pragma region Modeling Power By N

const Array<double>& CCHI2ProgramDoc::get_power_values_by_n() const
{
	return power_values_by_n;
}

double CCHI2ProgramDoc::get_power_alpha() const
{
	return power_alpha;
}

int CCHI2ProgramDoc::get_power_n_min() const
{
	return power_nmin;
}

int CCHI2ProgramDoc::get_power_n_max() const
{
	return power_nmax;
}

int CCHI2ProgramDoc::get_power_n_step() const
{
	return power_nstep;
}

void CCHI2ProgramDoc::generate_power_by_n()
{
	assert(power_nmin > 0 && power_nstep > 0);
	assert(power_nmax >= power_nmin);
	assert(power_alpha > 0 && power_alpha < 1);
	assert(pvalue_sample_size > 0);

	int n_count = (power_nmax - power_nmin) / power_nstep + 1; //CHANGED
	power_values_by_n = Array<double>(n_count);
	int old_sample_size = sample_size; //updatesample использует именно поле sample_size

	for (int i = 0; i < n_count; ++i)
	{
		int current_n = power_nmin + i * power_nstep; //CHANGED

		sample_size = current_n;
		update_sample();

		int rejected_count = 0;

		for (int j = 0; j < pvalue_sample_size; ++j)
		{
			s->simulate();
			h.set_data(*s, d0);
			h.calc_chi();

			if (h.get_pvalue() < power_alpha)
				++rejected_count;
		}

		power_values_by_n[i] = (double)rejected_count / (double)pvalue_sample_size;
	}

	sample_size = old_sample_size;
	update_sample();
}

#pragma endregion



#pragma region Serialize (files recording/reading)

// CCHI2ProgramDoc serialization
void CCHI2ProgramDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#pragma endregion


#pragma region Default(Handlers)

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCHI2ProgramDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCHI2ProgramDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCHI2ProgramDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS
#pragma endregion


#pragma region Debug
// CCHI2ProgramDoc diagnostics

#ifdef _DEBUG
// проверка корректности документа
void CCHI2ProgramDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// вывод в debug-log
void CCHI2ProgramDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
#pragma endregion


#pragma region Events
// CCHI2ProgramDoc commands

void CCHI2ProgramDoc::OnButtonUpdate()
{
	switch (program_mode)
	{
	case HistogramMode:
		if (s == nullptr)
			return;

		update_hist();
		break;

	case PValuesMode:
		generate_pvalues();
		break;

	case PowerByNMode:
		generate_power_by_n();
		break;

	case PowerByAlphaMode:
		generate_pvalues();
		break;

	default:
		return;
	}

	UpdateAllViews(nullptr);
}

void CCHI2ProgramDoc::OnHistogram()
{
	CHistogramDialog dlg;

	dlg.m_a0 = d0.get_a();
	dlg.m_b0 = d0.get_b();
	dlg.m_k0 = d0.get_k();

	dlg.m_a1 = d1.get_a();
	dlg.m_b1 = d1.get_b();
	dlg.m_k1 = d1.get_k();

	dlg.m_sample_size = sample_size;
	dlg.m_method = (method == Bernoulli ? 0 : 1);

	if (dlg.DoModal() == IDOK)
	{
		d0 = Distribution(dlg.m_a0, dlg.m_b0, dlg.m_k0);
		d1 = Distribution(dlg.m_a1, dlg.m_b1, dlg.m_k1);

		sample_size = dlg.m_sample_size;
		method = (dlg.m_method == 0 ? Bernoulli : Inverse);
		program_mode = HistogramMode;

		update_sample();
		update_hist();

		UpdateAllViews(nullptr);
	}
}

void CCHI2ProgramDoc::OnPlevels()
{
	CPValuesDialog dlg;

	dlg.m_a0 = d0.get_a();
	dlg.m_b0 = d0.get_b();
	dlg.m_k0 = d0.get_k();

	dlg.m_a1 = d1.get_a();
	dlg.m_b1 = d1.get_b();
	dlg.m_k1 = d1.get_k();

	dlg.m_sample_size = sample_size;
	dlg.m_pvalues_sample_size = pvalue_sample_size;
	dlg.m_method = (method == Bernoulli ? 0 : 1);

	if (dlg.DoModal() == IDOK)
	{
		d0 = Distribution(dlg.m_a0, dlg.m_b0, dlg.m_k0);
		d1 = Distribution(dlg.m_a1, dlg.m_b1, dlg.m_k1);

		sample_size = dlg.m_sample_size;
		pvalue_sample_size = dlg.m_pvalues_sample_size;
		method = (dlg.m_method == 0 ? Bernoulli : Inverse);
		program_mode = PValuesMode;

		generate_pvalues();

		UpdateAllViews(nullptr);
	}
}

void CCHI2ProgramDoc::OnPowerByN()
{
	CPowerByNDialog dlg;

	dlg.m_a0 = d0.get_a();
	dlg.m_b0 = d0.get_b();
	dlg.m_k0 = d0.get_k();

	dlg.m_a1 = d1.get_a();
	dlg.m_b1 = d1.get_b();
	dlg.m_k1 = d1.get_k();

	dlg.m_alpha = power_alpha;
	dlg.m_pvalue_sample_size = pvalue_sample_size;

	dlg.m_nmin = power_nmin;
	dlg.m_nmax = power_nmax;
	dlg.m_nstep = power_nstep;

	dlg.m_method = (method == Bernoulli ? 0 : 1);

	if (dlg.DoModal() == IDOK)
	{
		d0 = Distribution(dlg.m_a0, dlg.m_b0, dlg.m_k0);
		d1 = Distribution(dlg.m_a1, dlg.m_b1, dlg.m_k1);

		power_alpha = dlg.m_alpha;
		pvalue_sample_size = dlg.m_pvalue_sample_size;
		power_nmin = dlg.m_nmin;
		power_nmax = dlg.m_nmax;
		power_nstep = dlg.m_nstep;
		method = (dlg.m_method == 0 ? Bernoulli : Inverse);
		program_mode = PowerByNMode;

		generate_power_by_n();

		UpdateAllViews(nullptr);
	}
}

void CCHI2ProgramDoc::OnPowerByAplha()
{
	CPValuesDialog dlg;

	dlg.m_a0 = d0.get_a();
	dlg.m_b0 = d0.get_b();
	dlg.m_k0 = d0.get_k();

	dlg.m_a1 = d1.get_a();
	dlg.m_b1 = d1.get_b();
	dlg.m_k1 = d1.get_k();

	dlg.m_sample_size = sample_size;
	dlg.m_pvalues_sample_size = pvalue_sample_size;
	dlg.m_method = (method == Bernoulli ? 0 : 1);

	if (dlg.DoModal() == IDOK)
	{
		d0 = Distribution(dlg.m_a0, dlg.m_b0, dlg.m_k0);
		d1 = Distribution(dlg.m_a1, dlg.m_b1, dlg.m_k1);
		
		if (d0 == d1)
			AfxMessageBox(L"Warning! H0 = H1, Type I Error is displayed (not power).");
		
		sample_size = dlg.m_sample_size;
		pvalue_sample_size = dlg.m_pvalues_sample_size;
		method = (dlg.m_method == 0 ? Bernoulli : Inverse);
		program_mode = PowerByAlphaMode;

		generate_pvalues();

		UpdateAllViews(nullptr);
	}
}

#pragma endregion
