//#include "pch.h"
#include "Distribution.h"
#include <cassert>

Distribution::Distribution(int a, int b, int k)
    : a(a), b(b), k(k),
    states_number(k + 1),
    ps(k + 1)
{
    assert(a > 0 && b > 0 && k > 0);
    assert(k <= a && k <= b);

    ps[0] = init_prob();
    last_p = 0;
}

Distribution::Distribution(const Distribution& dist)
    : a(dist.a), b(dist.b), k(dist.k),
    name(dist.name),
    ps(dist.ps),
    last_p(dist.last_p),
    states_number(dist.states_number) {
}

int Distribution::get_a() const
{
    return a;
}

int Distribution::get_b() const
{
    return b;
}

int Distribution::get_k() const
{
    return k;
}

int Distribution::get_n() const
{
    return a + b;
}

String Distribution::get_dist_name() const
{
    return name;
}

int Distribution::get_states_num() const
{
    return states_number;
}

double Distribution::init_prob() const
{
    double p0 = 1;
    int n = a + b;

    for (int t = 0; t < k; ++t)
    {
        p0 *= (double)(b - t) / (double)(n - t);
    }

    return p0;
}

double Distribution::next_prob(double current_p, int i) const
{
    return current_p * (double)((a - i) * (k - i)) / (double)((i + 1) * (b - k + i + 1));
}

double Distribution::calc_prob_x(int x)
{
    if (x < 0 || x >= states_number)
        return 0;

    if (last_p < x)
    {
        double current_p = ps[last_p];

        for (int i = last_p; i < x; ++i)
        {
            current_p = next_prob(current_p, i);
            ps[i + 1] = current_p;
        }

        last_p = x;
    }

    return ps[x];
}

const Array<double>& Distribution::get_probs()
{
    calc_prob_x(states_number - 1);
    return ps;
}


bool Distribution::operator==(const Distribution& other) const
{
    return a == other.a && b == other.b && k == other.k;
}

bool Distribution::operator!=(const Distribution& other) const
{
    return !(*this == other);
}
