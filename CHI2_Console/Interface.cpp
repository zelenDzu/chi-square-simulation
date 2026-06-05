//
//#include "Interface.h"
//
// // Если вдруг понадобятся доп функции, используемые в Doc
// 
//void print_sample(const Sample& sample)
//{
//    const Array<int>& values = sample.get_values();
//
//    std::cout << "Sample:\n";
//
//    for (int i = 0; i < values.get_size(); ++i)
//    {
//        std::cout << values[i] << " ";
//    }
//
//    std::cout << "\n";
//}
//
//void print_hist(const Chi2Histogram& hist)
//{
//    const Array<int>& observed = hist.get_observed_freqs();
//    const Array<double>& expected = hist.get_expected_freqs();
//
//    std::cout << "\nFrequency table\n";
//    std::cout << "x\tObserved\tExpected\n";
//
//    for (int x = 0; x < hist.get_states_num(); ++x)
//    {
//        std::cout << x << "\t" << observed[x] << "\t\t" << expected[x] << "\n";
//    }
//
//    std::cout << "\nChi-square = " << hist.get_chi2();
//    std::cout << "\nd.f. = " << hist.get_df();
//    std::cout << "\np-value = " << hist.get_pvalue() << "\n";
//}
//
//double emp_dist(const Array<double>& pvalues, double alpha)
//{
//    if (alpha <= 0)
//        return 0;
//
//    if (alpha >= 1)
//        return 1;
//
//    int count = 0;
//
//    for (int i = 0; i < pvalues.get_size(); ++i)
//    {
//        if (pvalues[i] < alpha)
//            ++count;
//    }
//
//    return (double)count / (double)pvalues.get_size();
//}
//
//Array<double> generate_pvalues(Distribution& d0, Distribution& d1,
//    int sample_size,int pvalue_sample_size, Method method)
//{
//    Array<double> pvalues(pvalue_sample_size);
//
//    Sample* sample = nullptr;
//
//    switch (method)
//    {
//    case Bernoulli:
//        sample = new BernoulliSample(d1, sample_size);
//        break;
//
//    case Inverse:
//        sample = new InverseSample(d1, sample_size);
//        break;
//
//    default:
//        return pvalues;
//    }
//
//    Chi2Histogram hist;
//
//    for (int i = 0; i < pvalue_sample_size; ++i)
//    {
//        sample->simulate();
//        hist.set_data(*sample, d0);
//        hist.calc_chi();
//
//        pvalues[i] = hist.get_pvalue();
//    }
//
//    delete sample;
//    sample = nullptr;
//
//    return pvalues;
//}
//
//Array<double> generate_power_by_n(Distribution& d0, Distribution& d1,
//    int nmin, int nmax, double alpha, int pvalue_sample_size, Method method)
//{
//    int n_count = nmax - nmin + 1;
//    Array<double> power_values(n_count);
//
//    for (int i = 0; i < n_count; ++i)
//    {
//        int current_n = nmin + i;
//
//        Array<double> pvalues = generate_pvalues(d0,d1,current_n,pvalue_sample_size,method);
//
//        power_values[i] = emp_dist(pvalues, alpha);
//    }
//
//    return power_values;
//}