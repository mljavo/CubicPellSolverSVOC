#include "AlgebraicNumber.h"

AlgebraicNumber::AlgebraicNumber(int deg, BigInt d_val) : degree(deg), d(d_val)
{
    coefficients.resize(degree);
}

BigInt AlgebraicNumber::getCoefficient(int index) const
{
    if (index < 0 || index >= degree)
    {
        return BigInt(0);
    }
    return coefficients[index];
}

void AlgebraicNumber::setCoefficient(int index, const BigInt &value)
{
    if (index < 0 || index >= degree)
    {
        return;
    }
    coefficients[index] = value;
}
