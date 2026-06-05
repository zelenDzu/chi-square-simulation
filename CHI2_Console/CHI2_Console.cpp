#include <iostream>
#include "Interface.h"


int main()
{
    // How to set seed for sample simulations (if necessary)
    Sample::set_seed(1);

    // How to create distributions H0 and H1
    Distribution d0(10, 8, 4);
    Distribution d1(10, 8, 4);

    // How to get distribution parameters
    int a = d0.get_a();
    int b = d0.get_b();
    int k = d0.get_k();

    // How to calculate the probability of an outcome
    int x = 0;
    double prob = d0.calc_prob_x(x);
    
    // How to get all distribution probabilities
    Array<double> probs = d0.get_probs();

    // How to set modeling parameters
    int sample_size = 100;
    int pvalue_sample_size = 10000;
    Method method = Inverse;

    // How to create a sample object using enum and switch
    Sample* sample = nullptr;

    switch (method)
    {
    case Bernoulli:
        sample = new BernoulliSample(d1, sample_size);
        break;

    case Inverse:
        sample = new InverseSample(d1, sample_size);
        break;

    default:
        return 1;
    }

    // How to simulate one sample
    sample->simulate();

    //  How to get the simulated sample and its size
    Array<int> sapmle_data = sample->get_values();
    int size = sample->get_sample_size();

    // How to create chi-square histogram
    Chi2Histogram hist;
    hist.set_data(*sample, d0);

    // How to get frequencies used in the chi-square test
    Array<double> expected_freqs = hist.get_expected_freqs();
    Array<int> observed_freqs = hist.get_observed_freqs();

    // How to calculate chi-square test
    hist.calc_chi();

    // How to get chi-square test results
    double chi2 = hist.get_chi2();
    int df = hist.get_df();
    double pvalue = hist.get_pvalue();
    

    return 0;
}
