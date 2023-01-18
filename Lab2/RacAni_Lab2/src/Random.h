#pragma once
#include <random>
//#include <iostream>

class Random
{
public:
    static void Init()
    {
        rng.seed(std::random_device()());
    }
    static float Float(float lower, float upper)
    {
        float rand = (float)dist(rng) / ((float)std::numeric_limits<uint32_t>::max() / (upper - lower));
        //std::cout << lower + rand << std::endl;
        return lower + rand;
    }
private:
    //static std::random_device rd;
    static std::mt19937 rng;
    static std::uniform_int_distribution<std::mt19937::result_type> dist;
};