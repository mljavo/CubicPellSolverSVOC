#ifndef CUBICSOLVERLLL_H
#define CUBICSOLVERLLL_H

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <NTL/ZZvec.h>
#include "CubicNumber/CubicNumber.h"
#include "BigInt/BigInt.h"
#include "ResultExporter/ResultExporter.h"

class CubicSolverLLL
{
public:
    static void solveCubicEquationLLL(const NTL::ZZ &d, ResultExporter &exporter);

private:
    static NTL::ZZ calculateNorm(const NTL::ZZ &x, const NTL::ZZ &y, const NTL::ZZ &z, const NTL::ZZ &d);
    static NTL::ZZVec squareSolution(const NTL::ZZ &x, const NTL::ZZ &y, const NTL::ZZ &z, const NTL::ZZ &d);
};

#endif