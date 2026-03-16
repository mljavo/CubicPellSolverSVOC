#ifndef CUBICNUMBER_H
#define CUBICNUMBER_H
#include "../AlgebraicNumber/AlgebraicNumber.h"
#include "../BigInt/BigInt.h"
#include <stdexcept>

class CubicNumber : public AlgebraicNumber
{
public:
    CubicNumber(BigInt d_val) : AlgebraicNumber(3, d_val) {}

    CubicNumber(BigInt d_val, BigInt x_val, BigInt y_val, BigInt z_val);

    virtual ~CubicNumber() {}

    virtual BigInt norm() const override;

    virtual AlgebraicNumber *clone() const override
    {
        return new CubicNumber(*this);
    };
    virtual std::string toString() const override;

    virtual void add(const AlgebraicNumber &other) override;
    virtual void subtract(const AlgebraicNumber &other) override;
    virtual void multiply(const AlgebraicNumber &other) override;
    virtual CubicNumber &operator%(const BigInt &modulus) override;
    virtual void surdConjugate() override;
};

#endif // CUBICNUMBER_H