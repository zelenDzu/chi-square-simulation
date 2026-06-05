
// CHI2_ProgramView.cpp : implementation of the CCHI2ProgramView class
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
#include "CHI2_ProgramView.h"
#include <set>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma endregion


#pragma region Default(MAP,..)
// CCHI2ProgramView
IMPLEMENT_DYNCREATE(CCHI2ProgramView, CView)
BEGIN_MESSAGE_MAP(CCHI2ProgramView, CView)
END_MESSAGE_MAP()
#pragma endregion


#pragma region Cinstructor/Destructor
// CCHI2ProgramView construction/destruction

CCHI2ProgramView::CCHI2ProgramView() noexcept
{
	// TODO: add construction code here

}

CCHI2ProgramView::~CCHI2ProgramView()
{
}
#pragma endregion


#pragma region Precreate, OnDraw
BOOL CCHI2ProgramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCHI2ProgramView drawing

void CCHI2ProgramView::OnDraw(CDC* pDC)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    // NEW CODE
    switch (pDoc->get_program_mode())
    {
    case HistogramMode:
        draw_histogram(pDC);
        break;

    case PValuesMode:
        draw_pvalues(pDC);
        break;

    case PowerByNMode:
        draw_power_by_n(pDC);
        break;

    case PowerByAlphaMode:
        draw_power_by_alpha(pDC);
        break;

    default:
        pDC->TextOutW(20, 20, L"Unknown mode");
        break;
    }
	
}
#pragma endregion


#pragma region ProgramModes

void CCHI2ProgramView::draw_histogram(CDC* pDC)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Chi2Histogram& h = pDoc->get_histogram();
	const Array<int>& observed = h.get_observed_freqs();
	const Array<double>& expected = h.get_expected_freqs();

	const int states_num = h.get_states_num();
	if (states_num <= 0)
	{
		pDC->TextOutW(20, 20, L"States number = 0, nothing to draw :(");
		return;
	}

	CRect rc;
	GetClientRect(&rc);

	// отступы
	const int left_margin = 80;
	const int right_margin = 40;
	const int top_margin = 70;
	const int bottom_margin = 80;

	// область графика
	CRect plot_rect(
		left_margin,
		top_margin,
		rc.right - right_margin,
		rc.bottom - bottom_margin
	);

	// деления осей
	const int x_min = 0;
	const int x_max = states_num - 1;

	double y_max = 0; 
	for (int i = 0; i < states_num; ++i)
	{
		if (observed[i] > y_max)
			y_max = (double)observed[i];
		if (expected[i] > y_max)
			y_max = expected[i];
	}
	if (y_max <= 0)
		y_max = 1;

	// chi2, df, pvalue и параметры моделирования
	draw_chi2_info(pDC, h.get_chi2(), h.get_df(), h.get_pvalue());
	draw_modeling_info(pDC, plot_rect);

	// оси и подписи
	draw_histogram_axes(pDC, plot_rect, x_min, x_max, y_max);
	draw_x_ticks(pDC, plot_rect, states_num);
	draw_y_ticks(pDC, plot_rect, y_max, observed, expected, states_num);
	draw_histogram_labels(pDC, plot_rect, x_min, x_max, y_max);

	// теор. и эмп. столбцы
	for (int x = 0; x < states_num; ++x)
	{
		draw_theoretical_bar(pDC, plot_rect, x, states_num, expected[x], y_max);
	}

	for (int x = 0; x < states_num; ++x)
	{
		draw_observed_bar(pDC, plot_rect, x, states_num, observed[x], y_max);
	}
}

void CCHI2ProgramView::draw_pvalues(CDC* pDC)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Array<double>& pvalues = pDoc->get_pvalues();
	const Array<double>& alphas = pDoc->get_pvalue_alphas();

	if (pvalues.get_size() == 0 || alphas.get_size() == 0)
	{
		pDC->TextOutW(20, 20, L"Pvalues Size or Aplhas Number = 0 (for a reason). There is nothing to draw :(");
		return;
	}

	const bool h0_equal_h1 = pDoc->is_h0_equal_h1();
	Array<double> observed_cdf(alphas.get_size());
	for (int i = 0; i < alphas.get_size(); ++i)
	{
		double alpha = alphas[i];
		observed_cdf[i] = pDoc->pvalue_emp_dist(alpha);
	}

	CRect rc;
	GetClientRect(&rc);

	const int left_margin = 80;
	const int right_margin = 60;
	const int top_margin = 90;
	const int bottom_margin = 80;

	CRect plot_rect(
		left_margin,
		top_margin,
		rc.right - right_margin,
		rc.bottom - bottom_margin
	);

	draw_pvalues_info(pDC, plot_rect);

	draw_pvalues_axes(pDC, plot_rect, h0_equal_h1);
	draw_pvalues_ticks(pDC, plot_rect, alphas, observed_cdf);

	draw_pvalues_theoretical_cdf(pDC, plot_rect);
	draw_pvalues_observed_cdf(pDC, plot_rect, alphas, observed_cdf);

}

void CCHI2ProgramView::draw_power_by_n(CDC* pDC)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Array<double>& power_values = pDoc->get_power_values_by_n();

	if (power_values.get_size() == 0)
	{
		pDC->TextOutW(20, 20, L"Power Values Size = 0. There is nothing to draw :(");
		return;
	}

	int nmin = pDoc->get_power_n_min();
	int nmax = pDoc->get_power_n_max();
	int nstep = pDoc->get_power_n_step();

	CRect rc;
	GetClientRect(&rc);

	const int left_margin = 80;
	const int right_margin = 60;
	const int top_margin = 90;
	const int bottom_margin = 80;

	CRect plot_rect(
		left_margin,
		top_margin,
		rc.right - right_margin,
		rc.bottom - bottom_margin
	);

	draw_power_by_n_info(pDC, plot_rect);
	draw_power_by_n_axes(pDC, plot_rect);
	draw_power_by_n_ticks(pDC, plot_rect, power_values, nmin, nmax, nstep);
	draw_power_by_n_curve(pDC, plot_rect, power_values, nmin, nmax, nstep);

}

void CCHI2ProgramView::draw_power_by_alpha(CDC* pDC)
{
    //pDC->TextOutW(20, 20, L"Use P-levels mode with H1!=H0, where n = sample_size, H1 = H1");
	draw_pvalues(pDC);
}

#pragma endregion


#pragma region HistogramDrawing

void CCHI2ProgramView::draw_histogram_axes(CDC* pDC, const CRect& plot_rect, int x_min, int x_max, double y_max)
{
	CPen axis_pen;
	axis_pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* old_pen = pDC->SelectObject(&axis_pen);

	// Ox
	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.right, plot_rect.bottom);

	// Oy
	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.left, plot_rect.top);

	pDC->SelectObject(old_pen);
}

void CCHI2ProgramView::draw_x_ticks(CDC* pDC, const CRect& plot_rect, int states_num)
{
	if (states_num <= 0)
		return;

	const int tick_half = 4; //половина высоты

	for (int x = 0; x < states_num; ++x)
	{
		int x_left = x_value_to_left(plot_rect, x, states_num);
		int x_right = x_value_to_right(plot_rect, x, states_num);
		int x_center = (x_left + x_right) / 2;

		pDC->MoveTo(x_center, plot_rect.bottom - tick_half);
		pDC->LineTo(x_center, plot_rect.bottom + tick_half);

		CString str;
		str.Format(L"%d", x);
		pDC->TextOutW(x_center - 6, plot_rect.bottom + 8, str);
	}
}


void CCHI2ProgramView::draw_y_ticks(CDC* pDC, const CRect& plot_rect, double y_max, const Array<int>& observed, const Array<double>& expected, int states_num)
{
	if (y_max <= 0)
		return;

	std::set<double> levels;
	levels.insert(0);

	for (int i = 0; i < states_num; ++i)
	{
		levels.insert((double)observed[i]);
		levels.insert(expected[i]);
	}

	const int tick_half = 4;
	const int min_dist = 18;   // минимальное расстояние между тиками
	int last_y = -1;

	for (double value : levels)
	{
		int y = freq_to_y(plot_rect, value, y_max);

		if (last_y != -1 && abs(y - last_y) < min_dist)
			continue;

		pDC->MoveTo(plot_rect.left - tick_half, y);
		pDC->LineTo(plot_rect.left + tick_half, y);

		CString str;
		str.Format(L"%.1f", value);
		pDC->TextOutW(plot_rect.left - 45, y - 8, str);

		last_y = y;
	}
}


void CCHI2ProgramView::draw_histogram_labels(CDC* pDC, const CRect& plot_rect, int x_min, int x_max, double y_max)
{
	pDC->TextOutW(plot_rect.right + 10, plot_rect.bottom - 10, L"X");
	pDC->TextOutW(plot_rect.left - 15, plot_rect.top - 25, L"Freq");
}


void CCHI2ProgramView::draw_chi2_info(CDC* pDC, double chi2, int df, double p_value)
{
	CString str;

	str.Format(L"Chi-square = %.4f", chi2);
	pDC->TextOutW(20, 10, str);

	str.Format(L"d.f. = %d", df);
	pDC->TextOutW(220, 10, str);

	str.Format(L"p-value = %.8f", p_value);
	pDC->TextOutW(320, 10, str);
}

void CCHI2ProgramView::draw_modeling_info(CDC* pDC, const CRect& plot_rect)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Distribution& d0 = pDoc->get_d0();
	const Distribution& d1 = pDoc->get_d1();
	const Method method = pDoc->get_method();
	const int sample_size = pDoc->get_sample_size();

	int info_x = plot_rect.right - 190;
	int info_y = plot_rect.top - 65;

	CString str;

	str.Format(L"H0 = (%d, %d, %d)", d0.get_a(), d0.get_b(), d0.get_k());
	pDC->TextOutW(info_x, info_y, str);

	str.Format(L"H1 = (%d, %d, %d)", d1.get_a(), d1.get_b(), d1.get_k());
	pDC->TextOutW(info_x, info_y + 18, str);

	str.Format(L"Method = %s", method == Bernoulli ? L"Bernoulli" : L"Inverse");
	pDC->TextOutW(info_x, info_y + 36, str);

	str.Format(L"N = %d", sample_size);
	pDC->TextOutW(info_x, info_y + 54, str);
}

// левая Ox координата столбца значения x
int CCHI2ProgramView::x_value_to_left(const CRect& plot_rect, int x, int states_num) const
{
	double width = (double)(plot_rect.right - plot_rect.left);
	double step = width / states_num;
	return (int)(plot_rect.left + x * step);
}

// правая Ox координата столбца значения x
int CCHI2ProgramView::x_value_to_right(const CRect& plot_rect, int x, int states_num) const
{
	double width = (double)(plot_rect.right - plot_rect.left);
	double step = width / states_num;
	return (int)(plot_rect.left + (x + 1) * step);
}

int CCHI2ProgramView::freq_to_y(const CRect& plot_rect, double freq, double y_max) const
{
	if (y_max <= 0)
		return plot_rect.bottom;

	double height = (double)(plot_rect.bottom - plot_rect.top);
	double bar_height = freq / y_max * height;

	return (int)(plot_rect.bottom - bar_height);
}


void CCHI2ProgramView::draw_theoretical_bar(CDC* pDC, const CRect& plot_rect, int x, int states_num, double freq, double y_max)
{
	int x_left = x_value_to_left(plot_rect, x, states_num);
	int x_right = x_value_to_right(plot_rect, x, states_num);
	int y_top = freq_to_y(plot_rect, freq, y_max);

	int margin = 2;

	CBrush bar_brush;
	bar_brush.CreateSolidBrush(RGB(135, 206, 250));

	CBrush* old_brush = pDC->SelectObject(&bar_brush);

	pDC->Rectangle(
		x_left + margin,
		y_top,
		x_right - margin,
		plot_rect.bottom
	);

	pDC->SelectObject(old_brush);
}


void CCHI2ProgramView::draw_observed_bar(CDC* pDC, const CRect& plot_rect, int x, int states_num, int freq, double y_max)
{
	int x_left = x_value_to_left(plot_rect, x, states_num);
	int x_right = x_value_to_right(plot_rect, x, states_num);
	int y_top = freq_to_y(plot_rect, (double)freq, y_max);

	int width = x_right - x_left;
	int inner_margin = width / 5;

	CBrush bar_brush;
	bar_brush.CreateSolidBrush(RGB(30, 144, 255));

	CBrush* old_brush = pDC->SelectObject(&bar_brush);

	pDC->Rectangle(
		x_left + inner_margin,
		y_top,
		x_right - inner_margin,
		plot_rect.bottom
	);

	pDC->SelectObject(old_brush);
}

#pragma endregion


#pragma region PLevelsDrawing

int CCHI2ProgramView::alpha_to_x(const CRect& plot_rect, double alpha) const
{
	double width = (double)(plot_rect.right - plot_rect.left);
	return (int)(plot_rect.left + alpha * width);
}

int CCHI2ProgramView::cdf_value_to_y(const CRect& plot_rect, double value) const
{
	double height = (double)(plot_rect.bottom - plot_rect.top);
	return (int)(plot_rect.bottom - value * height);
}

void CCHI2ProgramView::draw_pvalues_axes(CDC* pDC, const CRect& plot_rect, bool h0_equal_h1)
{
	CPen axis_pen;
	axis_pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	CPen* old_pen = pDC->SelectObject(&axis_pen);

	// Ox
	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.right, plot_rect.bottom);

	// Oy
	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.left, plot_rect.top);

	pDC->SelectObject(old_pen);

	pDC->TextOutW(plot_rect.right + 10, plot_rect.bottom - 10, L"alpha");
	if (h0_equal_h1)
		pDC->TextOutW(plot_rect.left - 65, plot_rect.top - 30, L"a1 (type I error)");
	else
		pDC->TextOutW(plot_rect.left - 65, plot_rect.top - 25, L"1 - a2 (power)");

}

void CCHI2ProgramView::draw_pvalues_ticks(CDC* pDC, const CRect& plot_rect, const Array<double>& alphas, const Array<double>& observed_cdf)
{
	const int tick_half = 4;

	// Ox, тики по alpha
	for (int i = 0; i < alphas.get_size(); ++i)
	{
		double alpha = alphas[i];
		int x = alpha_to_x(plot_rect, alpha);

		pDC->MoveTo(x, plot_rect.bottom - tick_half);
		pDC->LineTo(x, plot_rect.bottom + tick_half);

		CString str;
		str.Format(L"%.2f", alpha);

		pDC->TextOutW(x - 12, plot_rect.bottom + 8, str);
	}

	// Oy, тики по значениям P(p<a)
	std::set<double> levels;
	levels.insert(0);
	levels.insert(1);

	for (int i = 0; i < observed_cdf.get_size(); ++i)
	{
		levels.insert(observed_cdf[i]);
	}

	const int min_dist = 18;
	int last_y = -1;

	for (double value : levels)
	{
		int y = cdf_value_to_y(plot_rect, value);

		if (last_y != -1 && abs(y - last_y) < min_dist)
			continue;

		pDC->MoveTo(plot_rect.left - tick_half, y);
		pDC->LineTo(plot_rect.left + tick_half, y);

		CString str;
		str.Format(L"%.2f", value);
		pDC->TextOutW(plot_rect.left - 48, y - 8, str);

		last_y = y;
	}
}

void CCHI2ProgramView::draw_pvalues_theoretical_cdf(CDC* pDC, const CRect& plot_rect)
{
	CPen theory_pen;
	theory_pen.CreatePen(PS_DASH, 1, RGB(120, 120, 120));

	CPen* old_pen = pDC->SelectObject(&theory_pen);

	pDC->MoveTo(
		alpha_to_x(plot_rect, 0),
		cdf_value_to_y(plot_rect, 0)
	);

	pDC->LineTo(
		alpha_to_x(plot_rect, 1),
		cdf_value_to_y(plot_rect, 1)
	);

	pDC->SelectObject(old_pen);

	pDC->TextOutW(plot_rect.left + 20, plot_rect.top + 20, L"F(alpha) = alpha");
}

void CCHI2ProgramView::draw_pvalues_observed_cdf(CDC* pDC,const CRect& plot_rect,const Array<double>& alphas, const Array<double>& observed_cdf)
{
	CPen observed_pen;
	observed_pen.CreatePen(PS_SOLID, 2, RGB(30, 144, 255));

	CPen* old_pen = pDC->SelectObject(&observed_pen);

	pDC->MoveTo(
		alpha_to_x(plot_rect, alphas[0]),
		cdf_value_to_y(plot_rect, observed_cdf[0]));

	for (int i = 1; i < observed_cdf.get_size(); ++i)
	{
		pDC->LineTo(
			alpha_to_x(plot_rect, alphas[i]),
			cdf_value_to_y(plot_rect, observed_cdf[i]));
	}

	pDC->SelectObject(old_pen);

	pDC->TextOutW(
		plot_rect.left + 20,
		plot_rect.top + 40,
		L"P(p < alpha)");
}

void CCHI2ProgramView::draw_pvalues_info(CDC* pDC, const CRect& plot_rect)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Distribution& d0 = pDoc->get_d0();
	const Distribution& d1 = pDoc->get_d1();
	const Method method = pDoc->get_method();

	int info_x = plot_rect.right - 230;
	int info_y = plot_rect.top - 80;

	CString str;

	pDC->TextOutW(20, 10, L"P-values CDF");

	str.Format(L"H0 = (%d, %d, %d)", d0.get_a(), d0.get_b(), d0.get_k());
	pDC->TextOutW(info_x, info_y, str);

	str.Format(L"H1 = (%d, %d, %d)", d1.get_a(), d1.get_b(), d1.get_k());
	pDC->TextOutW(info_x, info_y + 18, str);

	str.Format(L"Method = %s", method == Bernoulli ? L"Bernoulli" : L"Inverse");
	pDC->TextOutW(info_x, info_y + 36, str);

	str.Format(L"N = %d", pDoc->get_sample_size());
	pDC->TextOutW(info_x, info_y + 54, str);

	str.Format(L"p-values count = %d", pDoc->get_pvalue_sample_size());
	pDC->TextOutW(info_x, info_y + 72, str);

	if (pDoc->is_h0_equal_h1())
		pDC->TextOutW(20, 32, L"H0 = H1: alpha_I(alpha)");
	else
		pDC->TextOutW(20, 32, L"H0 != H1: 1 - alpha_II(alpha)");
}

#pragma endregion


#pragma region PowerByNDrawing

void CCHI2ProgramView::draw_power_by_n_axes(CDC* pDC, const CRect& plot_rect)
{
	CPen axis_pen;
	axis_pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	CPen* old_pen = pDC->SelectObject(&axis_pen);

	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.right, plot_rect.bottom);

	pDC->MoveTo(plot_rect.left, plot_rect.bottom);
	pDC->LineTo(plot_rect.left, plot_rect.top);

	pDC->SelectObject(old_pen);

	pDC->TextOutW(plot_rect.right + 10, plot_rect.bottom - 10, L"n");
	pDC->TextOutW(plot_rect.left - 75, plot_rect.top - 25, L"1 - alpha_II");
}

void CCHI2ProgramView::draw_power_by_n_ticks(CDC* pDC, const CRect& plot_rect, 
	const Array<double>& power_values, int nmin, int nmax, int nstep)
{
	if (power_values.get_size() == 0)
		return;

	const int tick_half = 4;

	// Oх, тики для всех n от nmin до nmax
	const int min_x_label_dist = 35;

	int n_count = (nmax - nmin) / nstep + 1;
	int label_step =  min_x_label_dist * (n_count - 1) / (plot_rect.right - plot_rect.left) + 1;

	for (int i = 0; i < n_count; i += label_step)
	{
		int n = nmin + i * nstep;
		int x = n_to_x(plot_rect, n, nmin, nmax);

		pDC->MoveTo(x, plot_rect.bottom - tick_half);
		pDC->LineTo(x, plot_rect.bottom + tick_half);

		CString str;
		str.Format(L"%d", n);
		pDC->TextOutW(x - 10, plot_rect.bottom + 8, str);
	}

	// Oy, тики по мощности
	std::set<double> levels;
	levels.insert(0);
	levels.insert(1);

	for (int i = 0; i < power_values.get_size(); ++i)
	{
		levels.insert(power_values[i]);
	}

	const int min_y_label_dist = 18;
	int last_y = -1;

	for (double value : levels)
	{
		int y = power_to_y(plot_rect, value);

		if (last_y != -1 && abs(y - last_y) < min_y_label_dist)
			continue;

		pDC->MoveTo(plot_rect.left - tick_half, y);
		pDC->LineTo(plot_rect.left + tick_half, y);

		CString str;
		str.Format(L"%.2f", value);
		pDC->TextOutW(plot_rect.left - 48, y - 8, str);

		last_y = y;
	}
}

void CCHI2ProgramView::draw_power_by_n_curve(CDC* pDC, const CRect& plot_rect, 
	const Array<double>& power_values, int nmin, int nmax, int nstep)
{
	if (power_values.get_size() == 0)
		return;

	CPen curve_pen;
	curve_pen.CreatePen(PS_SOLID, 2, RGB(30, 144, 255));

	CPen* old_pen = pDC->SelectObject(&curve_pen);

	int n = nmin;
	int x = n_to_x(plot_rect, n, nmin, nmax);
	int y = power_to_y(plot_rect, power_values[0]);
	pDC->MoveTo(x, y);

	for (int i = 1; i < power_values.get_size(); ++i)
	{
		n = nmin + i * nstep;

		x = n_to_x(plot_rect, n, nmin, nmax);
		y = power_to_y(plot_rect, power_values[i]);

		pDC->LineTo(x, y);
	}

	pDC->SelectObject(old_pen);
}

void CCHI2ProgramView::draw_power_by_n_info(CDC* pDC, const CRect& plot_rect)
{
	CCHI2ProgramDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;

	const Distribution& d0 = pDoc->get_d0();
	const Distribution& d1 = pDoc->get_d1();
	const Method method = pDoc->get_method();

	int info_x = plot_rect.right - 230;
	int info_y = plot_rect.top - 80;

	CString str;

	pDC->TextOutW(20, 10, L"Power By N");

	str.Format(L"H0 = (%d, %d, %d)", d0.get_a(), d0.get_b(), d0.get_k());
	pDC->TextOutW(info_x, info_y, str);

	str.Format(L"H1 = (%d, %d, %d)", d1.get_a(), d1.get_b(), d1.get_k());
	pDC->TextOutW(info_x, info_y + 18, str);

	str.Format(L"Method = %s", method == Bernoulli ? L"Bernoulli" : L"Inverse");
	pDC->TextOutW(info_x, info_y + 36, str);

	str.Format(L"p-values count = %d", pDoc->get_pvalue_sample_size());
	pDC->TextOutW(info_x, info_y + 54, str);

	str.Format(
		L"n = %d...%d, step = %d",
		pDoc->get_power_n_min(),
		pDoc->get_power_n_max(),
		pDoc->get_power_n_step()
	);
	pDC->TextOutW(info_x, info_y + 72, str);

	str.Format(L"alpha = %.2f", pDoc->get_power_alpha());
	pDC->TextOutW(info_x, info_y + 90, str);
}

int CCHI2ProgramView::n_to_x(const CRect& plot_rect, int n, int nmin, int nmax) const
{
	if (nmax == nmin)
		return (plot_rect.left + plot_rect.right) / 2;

	double width = (double)(plot_rect.right - plot_rect.left);
	double part = (double)(n - nmin) / (double)(nmax - nmin);

	return (int)(plot_rect.left + part * width);
}

int CCHI2ProgramView::power_to_y(const CRect& plot_rect, double power) const
{
	if (power < 0)
		power = 0;

	if (power > 1)
		power = 1;

	double height = (double)(plot_rect.bottom - plot_rect.top);

	return (int)(plot_rect.bottom - power * height);
}

#pragma endregion



#pragma region Debug
// CCHI2ProgramView diagnostics

#ifdef _DEBUG
void CCHI2ProgramView::AssertValid() const
{
	CView::AssertValid();
}

void CCHI2ProgramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCHI2ProgramDoc* CCHI2ProgramView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCHI2ProgramDoc)));
	return (CCHI2ProgramDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion



// CCHI2ProgramView message handlers
