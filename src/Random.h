#if !defined(__INCLUDE_RANDOM__H)
#define __INCLUDE_RANDOM__H

#include <list>

using std::list;


class Random
{
public:
    Random();
    virtual ~Random();

    void setSeedIndex(int index=-1);
    void setSeed(unsigned long seed);
    int iuniform(int min, int max);
    float uniform();
    float uniform(float min, float max);
    float normal(float mean, float stddev);
};

static Random gRandom;

#endif