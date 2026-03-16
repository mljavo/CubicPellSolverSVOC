#include "CubicNumber.h"

CubicNumber::CubicNumber(BigInt d_val, BigInt x_val, BigInt y_val, BigInt z_val)
    : AlgebraicNumber(3, d_val) // Zavolá konštruktor predka (degree=3, d=d_val)
{
    this->coefficients.clear();
    this->coefficients.push_back(x_val);
    this->coefficients.push_back(y_val);
    this->coefficients.push_back(z_val);
}

BigInt CubicNumber::norm() const
{
    // kubicke cislo tvaru a + b*cbrt(d) + c*cbrt(d)^2 ma normu tvaru
    // a^3 + b^3*d + c^3*d^2 - 3*a*b*c*d
    BigInt x = getCoefficient(0);
    BigInt y = getCoefficient(1);
    BigInt z = getCoefficient(2);
    return (x * x * x) + (d * y * y * y) + (d * d * z * z * z) - (BigInt(3) * d * x * y * z);
}

std::string CubicNumber::toString() const
{
    return "(" + getCoefficient(0).toString() + " + " +
           getCoefficient(1).toString() + "*cbrt(" + d.toString() + ") + " +
           getCoefficient(2).toString() + "*cbrt(" + d.toString() + ")^2)";
}

void CubicNumber::add(const AlgebraicNumber &other)
{
    if (this->getDegree() != other.getDegree())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre scitanie (rozdielny stupen).");
    }
    if (this->getD() != other.getD())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre scitanie (rozdielne d).");
    }

    const CubicNumber *pOther = dynamic_cast<const CubicNumber *>(&other);

    if (pOther)
    {
        this->coefficients[0] += pOther->coefficients[0];
        this->coefficients[1] += pOther->coefficients[1];
        this->coefficients[2] += pOther->coefficients[2];
        return;
    }

    throw std::invalid_argument("Nekompatibilne algebraicke cislo pre scitanie (nespravny typ).");
}

void CubicNumber::subtract(const AlgebraicNumber &other)
{
    if (this->getDegree() != other.getDegree())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre odcitanie (rozdielny stupen).");
    }
    if (this->getD() != other.getD())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre odcitanie (rozdielne d).");
    }

    const CubicNumber *pOther = dynamic_cast<const CubicNumber *>(&other);

    if (pOther)
    {
        this->coefficients[0] -= pOther->coefficients[0];
        this->coefficients[1] -= pOther->coefficients[1];
        this->coefficients[2] -= pOther->coefficients[2];
        return;
    }

    throw std::invalid_argument("Nekompatibilne algebraicke cislo pre odcitanie (nespravny typ).");
}

void CubicNumber::multiply(const AlgebraicNumber &other)
{
    if (this->getDegree() != other.getDegree())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre nasobenie (rozdielny stupen).");
    }
    if (this->getD() != other.getD())
    {
        throw std::invalid_argument("Nekompatibilne algebraicke cislo pre nasobenie (rozdielne d).");
    }

    const CubicNumber *pOther = dynamic_cast<const CubicNumber *>(&other);

    if (pOther)
    {
        BigInt a1 = this->coefficients[0];
        BigInt b1 = this->coefficients[1];
        BigInt c1 = this->coefficients[2];

        BigInt a2 = pOther->coefficients[0];
        BigInt b2 = pOther->coefficients[1];
        BigInt c2 = pOther->coefficients[2];

        BigInt newA = a1 * a2 + d * (b1 * c2 + c1 * b2);
        BigInt newB = a1 * b2 + b1 * a2 + d * c1 * c2;
        BigInt newC = a1 * c2 + b1 * b2 + c1 * a2;

        this->coefficients[0] = newA;
        this->coefficients[1] = newB;
        this->coefficients[2] = newC;
        return;
    }

    throw std::invalid_argument("Nekompatibilne algebraicke cislo pre nasobenie (nespravny typ).");
}

CubicNumber &CubicNumber::operator%(const BigInt &modulus)
{
    for (int i = 0; i < 3; i++)
    {
        this->coefficients[i] %= modulus;
    }
    return *this;
}

void CubicNumber::surdConjugate()
{
    throw std::runtime_error("surdConjugate zatial nie je implementovane pre CubicNumber.");
}
