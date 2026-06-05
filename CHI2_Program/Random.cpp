//#include "pch.h"
#include "Random.h"

std::mt19937& global_rng()
{
    static std::mt19937 gen(std::random_device{}());
    return gen;
}