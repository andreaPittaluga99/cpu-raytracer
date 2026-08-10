#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <cmath>

namespace rt
{
    inline double random_double()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        //by default is already [0.0, 1.0)
        std::uniform_real_distribution<double> distrib(0.0, 1.0);
        return distrib(gen);
    }

    inline double random_double(double min, double max)
    {
        return min + (max - min) * random_double();
    }
}

#endif