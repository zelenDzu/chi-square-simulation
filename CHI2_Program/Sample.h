#pragma once

#include "array_template.h"
#include "Distribution.h"
#include <random>

/**
 * @file Sample.h
 * @brief Contains the abstract Sample base class.
 */

/**
 * @class Sample
 * @brief Abstract base class for simulated samples.
 *
 * The class stores a distribution, sample size and generated values.
 * Derived classes implement different simulation methods.
 * 
 * The Mersenne Twister generator is a static field of Sample. Therefore, all
 * Sample objects and all derived objects use the same random number generator.
 */
class Sample
{
protected:
    Distribution dist;              ///< Distribution used for simulation.
    int sample_size{};              ///< Number of observations in the sample.
    Array<int> values{0};           ///< Generated sample values.
    static std::mt19937 generator;  ///< Common Mersenne Twister generator.

public:
    /**
     * @brief Creates a sample object.
     * @param dist Distribution used for simulation.
     * @param sample_size Number of observations in the sample.
     */
    Sample(const Distribution& dist, int sample_size);
    /**
     * @brief Copy constructor.
     * @param other Sample to copy.
     */
    Sample(const Sample& other);
    virtual ~Sample() {}

    int get_sample_size() const;
    const Array<int>& get_values() const;

    /**
    * @brief Simulates sample values.
    *
    * This function is implemented in derived classes.
    */
    virtual void simulate() = 0;
    /**
     * @brief Sets seed for the common random number generator.
     * @param seed New seed value.
     */
    static void set_seed(unsigned int seed);
};