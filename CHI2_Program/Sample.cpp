//#include "pch.h"
#include "Sample.h"
#include <cassert>

std::mt19937 Sample::generator{ std::random_device{}() };

Sample::Sample(const Distribution& dist, int sample_size)
    : dist(dist), sample_size(sample_size), values(sample_size)
{
    assert(sample_size > 0);
}

Sample::Sample(const Sample& other)
    : dist(other.dist), sample_size(other.sample_size), values(other.values) {}

int Sample::get_sample_size() const
{
    return sample_size;
}

const Array<int>& Sample::get_values() const
{
    return values;
}

void Sample::set_seed(unsigned int seed)
{
    generator.seed(seed);
}
