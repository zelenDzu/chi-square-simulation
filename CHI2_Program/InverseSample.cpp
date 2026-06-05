//#include "pch.h"
#include "SampleInverse.h"
#include <random>

InverseSample::InverseSample(const Distribution& dist, int sample_size)
    : Sample(dist, sample_size){}

void InverseSample::simulate()
{
    std::uniform_real_distribution<double> unif(0,1);

    int a = dist.get_a();
    int b = dist.get_b();
    int k = dist.get_k();

    for (int s = 0; s < sample_size; ++s)
    {
        double U = unif(generator);

        int i = 0; // текущее число белых
        double p = dist.calc_prob_x(0);
        double sum = p;

        while (U >= sum && i < k)
        {
            p *= (double)(a - i) * (double)(k - i);
            p /= (double)(i + 1) * (double)(b - k + i + 1);

            sum += p;
            ++i;
        }

        values[s] = i;
    }
}