#ifndef MATH_BP_H
#define MATH_BP_H
#include <BigInt/BigInt.h>

namespace MathBP
{
    BigInt nthRoot(const BigInt &value, int n);
    BigInt gcd(const BigInt &a, const BigInt &b);
}

#endif