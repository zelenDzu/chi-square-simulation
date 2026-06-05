#pragma once

/**
 * @enum Method
 * @brief Sample simulation method.
 *
 * The value of this enum is used with switch to create a concrete
 * derived object of the abstract Sample class.
 */
enum Method
{
    Bernoulli, ///< Bernoulli simulation method.
    Inverse ///< Inverse simulation method.
};