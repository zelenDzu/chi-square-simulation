
// CHI2_ProgramView.h : interface of the CCHI2ProgramView class
//

#pragma once


class CCHI2ProgramView : public CView
{
protected: // create from serialization only
	CCHI2ProgramView() noexcept;
	DECLARE_DYNCREATE(CCHI2ProgramView)

// Attributes
public:
	CCHI2ProgramDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Implementation
public:
	virtual ~CCHI2ProgramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Режимы программы
	void draw_histogram(CDC* pDC);
	void draw_pvalues(CDC* pDC);
	void draw_power_by_n(CDC* pDC);
	void draw_power_by_alpha(CDC* pDC);

	// вспомогательные функции для гистограммы
	void draw_histogram_axes(CDC* pDC, const CRect& plot_rect, int x_min, int x_max, double y_max);
	void draw_histogram_labels(CDC* pDC, const CRect& plot_rect, int x_min, int x_max, double y_max);
	void draw_x_ticks(CDC* pDC, const CRect& plot_rect, int states_num);
	void draw_y_ticks(CDC* pDC, const CRect& plot_rect, double y_max, const Array<int>& observed, const Array<double>& expected, int states_num);

	void draw_chi2_info(CDC* pDC, double chi2, int df, double p_value);
	void draw_modeling_info(CDC* pDC, const CRect& plot_rect);

	int x_value_to_left(const CRect& plot_rect, int x, int states_num) const;
	int x_value_to_right(const CRect& plot_rect, int x, int states_num) const;
	int freq_to_y(const CRect& plot_rect, double freq, double y_max) const;

	void draw_theoretical_bar(CDC* pDC, const CRect& plot_rect, int x, int states_num, double freq, double y_max);
	void draw_observed_bar(CDC* pDC, const CRect& plot_rect, int x, int states_num, int freq, double y_max);

	// вспомогательные функции для plevels
	void draw_pvalues_axes(CDC* pDC, const CRect& plot_rect, bool h0_equal_h1);
	void draw_pvalues_ticks(CDC* pDC, const CRect& plot_rect, const Array<double>& alphas, const Array<double>& observed_cdf);
	void draw_pvalues_theoretical_cdf(CDC* pDC, const CRect& plot_rect);
	void draw_pvalues_observed_cdf(CDC* pDC, const CRect& plot_rect, const Array<double>& alphas, const Array<double>& observed_cdf);
	void draw_pvalues_info(CDC* pDC, const CRect& plot_rect);

	int alpha_to_x(const CRect& plot_rect, double alpha) const;
	int cdf_value_to_y(const CRect& plot_rect, double value) const;


	// вспомогательные функции  для power
	void draw_power_by_n_axes(CDC* pDC, const CRect& plot_rect);
	void draw_power_by_n_ticks(CDC* pDC,const CRect& plot_rect, const Array<double>& power_values,int nmin,int nmax, int nstep);
	void draw_power_by_n_curve(CDC* pDC,const CRect& plot_rect,const Array<double>& power_values,int nmin,int nmax, int nstep);
	void draw_power_by_n_info(CDC* pDC, const CRect& plot_rect);
	int n_to_x(const CRect& plot_rect, int n,int nmin, int nmax) const;
	int power_to_y(const CRect& plot_rect, double power) const;



// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CHI2_ProgramView.cpp
inline CCHI2ProgramDoc* CCHI2ProgramView::GetDocument() const
   { return reinterpret_cast<CCHI2ProgramDoc*>(m_pDocument); }
#endif

