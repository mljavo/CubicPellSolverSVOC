#ifndef ALGEBRAICNUMBER_H
#define ALGEBRAICNUMBER_H
#include <gmp.h>
#include <string>
#include <iostream>
#include <vector>
#include "../BigInt/BigInt.h"

class AlgebraicNumber
{
protected:
    std::vector<BigInt> coefficients;
    BigInt d;
    int degree;
    std::string label;

public:
    AlgebraicNumber(int deg, BigInt d_val);
    virtual ~AlgebraicNumber() {}

    virtual BigInt norm() const = 0;

    virtual AlgebraicNumber *clone() const = 0;
    virtual std::string toString() const = 0;

    virtual void add(const AlgebraicNumber &other) = 0;
    virtual void subtract(const AlgebraicNumber &other) = 0;
    virtual void multiply(const AlgebraicNumber &other) = 0;
    virtual AlgebraicNumber &operator%(const BigInt &modulus) = 0;
    virtual void surdConjugate() = 0;

    int getDegree() const { return degree; }
    BigInt getD() const { return d; }
    BigInt getCoefficient(int index) const;
    void setCoefficient(int index, const BigInt &value);
    BigInt &getCoefficientRef(int index) { return coefficients[index]; }
    void setLabel(const std::string &lbl) { label = lbl; }
    std::string getLabel() const { return label; }
};

#endif