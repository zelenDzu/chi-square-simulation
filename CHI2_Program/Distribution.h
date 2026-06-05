#pragma once

#include "array_template.h"
#include "string.h"

/**
 * @file Distribution.h
 * @brief Contains the Distribution class.
 */

 /**
  * @class Distribution
  * @brief Hypergeometric distribution with cached theoretical probabilities.
  *
  * The class stores parameters a, b and k of the hypergeometric distribution.
  * The random variable is the number of white balls among k drawn balls.
  */
class Distribution
{
private:
    int a{}; ///< Number of white balls.
    int b{}; ///< Number of black balls.
    int k{}; ///< Number of drawn balls (k<=a, k<=b).

    String name{ "Hypergeometric" }; ///< Distribution name.
    Array<double> ps{0};             ///< Cached probabilities P(X = x).
    int last_p{-1};                  ///< Index of the last calculated probability.
    int states_number{0};            ///< Number of possible states.

    /**
   * @brief Calculates the initial probability P(X = 0).
   * @return Probability P(X = 0).
   */
    double init_prob() const; 
    /**
    * @brief Calculates the next probability.
    * @param current_p Current probability P(X = i).
    * @param i Current state index.
    * @return Next probability P(X = i + 1).
    */
    double next_prob(double current_p, int i) const; 

public:
    /**
    * @brief Creates a hypergeometric distribution.
    * @param a Number of white balls.
    * @param b Number of black balls.
    * @param k Number of drawn balls, where k<=a, k<=b.
    */
    Distribution(int a = 1, int b = 1, int k = 1);
    /**
     * @brief Copy constructor.
     * @param dist Distribution to copy.
     */
    Distribution(const Distribution& dist);
    ~Distribution() {}

    String get_dist_name() const;
    int get_states_num() const;

    int get_a() const;
    int get_b() const;
    int get_k() const;
    /**
    * @brief Returns common amount of balls.
    * @return n = a + b.
    */
    int get_n() const;

    /**
   * @brief Returns probability P(X = x).
   * @param x State value.
   * @return Probability P(X = x), or 0 if x is outside the state space.
   */
    double calc_prob_x(int x);
    /**
    * @brief Returns all theoretical probabilities.
    * @return Constant reference to the probability array.
    */
    const Array<double>& get_probs();

    /**
       * @brief Compares two distributions.
       * @param other Distribution to compare with.
       * @return true if parameters a, b and k are equal.
       */
    bool operator==(const Distribution& other) const;
    /**
     * @brief Compares two distributions.
     * @param other Distribution to compare with.
     * @return true if at least one parameter is different.
     */
    bool operator!=(const Distribution& other) const;
};