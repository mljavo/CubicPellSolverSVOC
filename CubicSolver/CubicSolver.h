#ifndef CUBICSOLVER_H
#define CUBICSOLVER_H
#include <vector>
#include "../CubicNumber/CubicNumber.h"
#include "../BigInt/BigInt.h"

class CubicSolver
{
public:
    static CubicNumber findCubicSolution(const BigInt &d, long limit);
};

#endif