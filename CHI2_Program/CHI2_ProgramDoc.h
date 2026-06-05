#pragma once

#include "Distribution.h"
#include "Sample.h"
#include "SampleBernoulli.h"
#include "SampleInverse.h"
#include "Chi2Histogram.h"
#include "Method.h"
#include "ProgramMode.h"


class CCHI2ProgramDoc : public CDocument
{
protected: 
	// CONSTRUCTOR
	CCHI2ProgramDoc() noexcept;
	DECLARE_DYNCREATE(CCHI2ProgramDoc)

private:
	ProgramMode program_mode{ HistogramMode };

	Distribution d0{10, 8, 4};
	Distribution d1{10, 8, 4};
	Sample* s{ nullptr };
	Chi2Histogram h{};

	Method method{ Inverse };
	int sample_size{ 100 };
	int pvalue_sample_size{ 10000 };

	Array<double> pvalues{ 0 };
	Array<double> pvalue_alphas{ 0 }; // будут 0, 0.05, 0.1, 0.15,.... 

	Array<double> power_values_by_n{ 0 };
	double power_alpha{0.05};
	int power_nmin{ 100 };
	int power_nmax{ 150 };
	int power_nstep{ 1 };


private:
	// обновить метод моделирования
	void update_sample();
	// сгенерировать новую гистаграмму
	void update_hist();

	// обновляет список pvalues 
	void generate_pvalues();
	// обновляет pvalue_alphas - сетку альфа (0, 0.05, 0.1, ...)
	void build_alphas_grid(int alpha_count = 21);

	// создает массив [nmin, nmax], где уже посчитанные доли pvalue < alpha
	void generate_power_by_n(); 

public:
	// DESTRUCTOR
	virtual ~CCHI2ProgramDoc();

	// Default
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// Modeling Histogram (getters)
	const Distribution& get_d0() const;
	const Distribution& get_d1() const;
	const Sample* get_sample() const;
	const Chi2Histogram& get_histogram() const;
	Method get_method() const;
	ProgramMode get_program_mode() const;
	int get_sample_size() const;
	int get_pvalue_sample_size() const;

	// Сеттеры
	//void set_d0(const Distribution& new_d0);
	//void set_d1(const Distribution& new_d1);
	//void set_method(Method new_method);
	//void set_program_mode(ProgramMode new_mode);
	//void set_sample_size(int new_size);
	//void set_pvalue_sample_size(int new_size);


	// PValues
	const Array<double>& get_pvalues() const;
	const Array<double>& get_pvalue_alphas() const;
	bool is_h0_equal_h1() const;

	//эмпирическая функция распределения F(a)=P(p<a) = #{p_i<a} / n
	double pvalue_emp_dist(double a) const; 

	
	// Power
	// возвращает массив [P(p<a) при n=nmin, P(p<a) при n=nmin+1, ...]
	const Array<double>& get_power_values_by_n() const;
	double get_power_alpha() const;
	int get_power_n_min() const;
	int get_power_n_max() const;
	int get_power_n_step() const;



// default
public:
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif 

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif 
public:
	afx_msg void OnButtonUpdate();
	afx_msg void OnHistogram();
	afx_msg void OnPlevels();
	afx_msg void OnPowerByN();
	afx_msg void OnPowerByAplha();
};
