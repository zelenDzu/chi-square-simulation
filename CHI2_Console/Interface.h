#pragma once

#include <iostream>

#include "../CHI2_Program/Distribution.h"
#include "../CHI2_Program/Sample.h"
#include "../CHI2_Program/SampleBernoulli.h"
#include "../CHI2_Program/SampleInverse.h"
#include "../CHI2_Program/Chi2Histogram.h"
#include "../CHI2_Program/array_template.h"
#include "../CHI2_Program/Method.h"
#include "../CHI2_Program/string.h"


//void print_sample(const Sample& sample);
//void print_hist(const Chi2Histogram& hist);
//double emp_dist(const Array<double>& pvalues, double alpha);
//Array<double> generate_pvalues(Distribution& d0, Distribution& d1, 
//	int sample_size, int pvalue_sample_size, Method method);
//Array<double> generate_power_by_n(Distribution& d0, Distribution& d1,
//	int nmin, int nmax, double alpha, int pvalue_sample_size, Method method);