//#include "pch.h"
#include "Chi2Histogram.h"
#include "PROBDIST.H"
#include <cassert>

// возвращает pvalue(t) для chi2 при заданном df
static double chi_square_p_value(double t, int df)
{
    if (df <= 0) return 1.0;
    if (t <= 0.0) return 1.0;

    return 1.0 - pChi(t, df);
}

Chi2Histogram::Chi2Histogram() {} //-1 для еще не посчитанных значений

Chi2Histogram::Chi2Histogram(const Sample& sample, Distribution& d0)
{
    set_data(sample, d0);
}

// возвращает список с частотами значений от 0 до states_num-1
Array<int> Chi2Histogram::count_observed(const Array<int>& sample_values, int states_number) const
{
    Array<int> freqs(states_number);

    for (int i = 0; i < sample_values.get_size(); ++i)
    {
        int x = sample_values[i];
        if (x >= 0 && x < states_number)
            ++freqs[x];
    }

    return freqs;
}

void Chi2Histogram::set_data(const Sample& sample, Distribution& d0)
{
    const Array<int>& values = sample.get_values();

    sample_size = sample.get_sample_size();
    states_number = d0.get_states_num();

    assert(sample_size > 0);
    assert(states_number > 0);

    observed_freqs = count_observed(values, states_number);
    expected_freqs = Array<double>(states_number);

    for (int x = 0; x < states_number; ++x)
    {
        expected_freqs[x] = sample_size * d0.calc_prob_x(x);
    }

    chi2 = -1;
    df = -1;
    p_value = -1;
}

// обновляет значения chi2, df, pvalue
void Chi2Histogram::calc_chi()
{
    Array<int> grouped_obs(states_number);
    Array<double> grouped_exp(states_number);

    int groups = 0;
    int obs_sum = 0;
    double exp_sum = 0;

    for (int i = 0; i < states_number; ++i)
    {
        obs_sum += observed_freqs[i];
        exp_sum += expected_freqs[i];

        if (exp_sum >= use_rule)
        {
            grouped_obs[groups] = obs_sum;
            grouped_exp[groups] = exp_sum;

            ++groups;
            obs_sum = 0;
            exp_sum = 0;
        }
    }

    // хвост к последней группе
    if (exp_sum > 0)
    {
        if (groups == 0)
        {
            grouped_obs[0] = obs_sum;
            grouped_exp[0] = exp_sum;
            groups = 1;
        }
        else
        {
            grouped_obs[groups - 1] += obs_sum;
            grouped_exp[groups - 1] += exp_sum;
        }
    }

    chi2 = 0;

    for (int i = 0; i < groups; ++i)
    {
        double O = grouped_obs[i];
        double E = grouped_exp[i];

        if (E > 0)
        {
            double diff = O - E;
            chi2 += diff * diff / E;
        }
    }

    df = groups - 1;
    p_value = chi_square_p_value(chi2, df);
}

const Array<int>& Chi2Histogram::get_observed_freqs() const
{
    return observed_freqs;
}

const Array<double>& Chi2Histogram::get_expected_freqs() const
{
    return expected_freqs;
}

int Chi2Histogram::get_states_num() const
{
    return states_number;
}

int Chi2Histogram::get_sample_size() const
{
    return sample_size;
}

double Chi2Histogram::get_chi2() const
{
    return chi2;
}

int Chi2Histogram::get_df() const
{
    return df;
}

double Chi2Histogram::get_pvalue() const
{
    return p_value;
}