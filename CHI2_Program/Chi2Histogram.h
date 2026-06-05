#pragma once

#include "array_template.h"
#include "Sample.h"
#include "Distribution.h"

/**
 * @file Chi2Histogram.h
 * @brief Contains the Chi2Histogram class.
 */

/**
 * @class Chi2Histogram
 * @brief Stores observed and expected frequencies and calculates chi-square test.
 *
 * The class stores observed frequencies from the given sample and expected frequencies
 * calculated from H0. 
 * It calculates chi-square statistic, degrees of freedom and p-value.
 */
class Chi2Histogram
{
private:
    Array<int> observed_freqs{0};       ///< Observed frequencies from the sample.
    Array<double> expected_freqs{0};    ///< Expected frequencies under H0.
    int states_number{};                ///< Number of possible states.
    int sample_size{};                  ///< Sample size.

    // -1 если еще не считали
    double chi2{-1};                    ///< Chi-square statistic.
    int df{-1};                         ///< Degrees of freedom.
    double p_value{-1};                 ///< P-value of the chi-square test.

    const double use_rule{5};           ///< Value used for frequencies grouping

    /**
    * @brief Counts observed frequencies.
    * @param sample_values Array of sample values.
    * @param states_number Number of possible states.
    * @return Array of observed frequencies.
    */
    Array<int> count_observed(const Array<int>& sample_values, int states_num) const;

public:
    /**
    * @brief Creates an empty chi-square histogram.
    */
    Chi2Histogram();
    /**
     * @brief Creates a chi-square histogram from sample and H0.
     * @param sample Simulated sample.
     * @param d0 Null distribution H0.
     */
    Chi2Histogram(const Sample& sample, Distribution& d0);
    ~Chi2Histogram() {}

    /**
   * @brief Sets data for the chi-square test.
   * @param sample Simulated sample.
   * @param d0 Null distribution H0.
   */
    void set_data(const Sample& sample, Distribution& d0);
    /**
    * @brief Calculates chi-square statistic, degrees of freedom and p-value.
    * 
    * Calculated values are stored in the class fields.
    */
    void calc_chi();

    const Array<int>& get_observed_freqs() const;
    const Array<double>& get_expected_freqs() const;
    int get_states_num() const;
    int get_sample_size() const;
    double get_chi2() const;
    int get_df() const;
    double get_pvalue() const;
};