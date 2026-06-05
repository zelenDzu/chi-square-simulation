//#include "pch.h"
#include "SampleBernoulli.h"
#include <random>


BernoulliSample::BernoulliSample(const Distribution& dist, int sample_size)
    : Sample(dist, sample_size) {}

void BernoulliSample::simulate()
{
    std::uniform_real_distribution<double> unif(0, 1);

    int a = dist.get_a();
    int b = dist.get_b();
    int k = dist.get_k();
    int n = a + b;

    for (int s = 0; s < sample_size; ++s)
    {
        int i = 0; // сколько уже вытащили
        int j = 0; // сколько среди них белых

        while (i < k)
        {
            double U = unif(generator);
            double p_white = (double)(a - j) / (double)(n - i);

            if (U < p_white)
                ++j;

            ++i;
        }

        values[s] = j;
    }
}