#include "BigInt.h"

BigInt::BigInt()
{
    mpz_init(value);
}

BigInt::BigInt(long i)
{
    mpz_init_set_si(value, i);
}

BigInt::BigInt(const std::string &str)
{
    mpz_init_set_str(value, str.c_str(), 10);
}

BigInt::BigInt(const BigInt &other)
{
    mpz_init_set(value, other.value);
}

BigInt::~BigInt()
{
    mpz_clear(value);
}

BigInt &BigInt::operator=(const BigInt &other)
{
    if (this != &other)
    {
        mpz_set(value, other.value);
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt &other) const
{
    BigInt result(*this);
    result += other;
    return result;
}

BigInt BigInt::operator-(const BigInt &other) const
{
    BigInt result(*this);
    result -= other;
    return result;
}

BigInt BigInt::operator*(const BigInt &other) const
{
    BigInt result(*this);
    result *= other;
    return result;
}

BigInt BigInt::operator/(const BigInt &other) const
{
    BigInt result(*this);
    result /= other;
    return result;
}

BigInt BigInt::sqrt() const
{
    BigInt result;
    mpz_sqrt(result.value, this->value);
    return result;
}

BigInt &BigInt::operator+=(const BigInt &other)
{
    mpz_add(this->value, this->value, other.value);
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &other)
{
    mpz_sub(this->value, this->value, other.value);
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &other)
{
    mpz_mul(this->value, this->value, other.value);
    return *this;
}

BigInt &BigInt::operator/=(const BigInt &other)
{
    mpz_tdiv_q(this->value, this->value, other.value);
    return *this;
}

bool BigInt::operator==(const BigInt &other) const
{
    return mpz_cmp(value, other.value) == 0;
}

bool BigInt::operator!=(const BigInt &other) const
{
    return mpz_cmp(value, other.value) != 0;
}

bool BigInt::operator<(const BigInt &other) const
{
    return mpz_cmp(value, other.value) < 0;
}

bool BigInt::operator<=(const BigInt &other) const
{
    return mpz_cmp(value, other.value) <= 0;
}

bool BigInt::operator>(const BigInt &other) const
{
    return mpz_cmp(value, other.value) > 0;
}

bool BigInt::operator>=(const BigInt &other) const
{
    return mpz_cmp(value, other.value) >= 0;
}

std::string BigInt::toString() const
{
    char *str = mpz_get_str(nullptr, 10, value);
    std::string result(str);
    free(str);
    return result;
}

std::ostream &operator<<(std::ostream &os, const BigInt &bigInt)
{
    os << bigInt.toString();
    return os;
}

BigInt &BigInt::operator%=(const BigInt &other)
{
    mpz_tdiv_r(this->value, this->value, other.value);
    return *this;
}

BigInt BigInt::operator%(const BigInt &other) const
{
    BigInt result(*this);
    result %= other;
    return result;
}

BigInt BigInt::power(const BigInt &base, int exp)
{
    if (exp < 0)
    {
        throw std::invalid_argument("Exponent musi byt nezáporný.");
    }

    BigInt result(1);
    BigInt b = base;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result *= b;
        }
        b *= b;
        exp /= 2;
    }
    return result;
}

NTL::ZZ BigInt::toZZ() const
{
    std::string s = this->toString();
    return NTL::conv<NTL::ZZ>(s.c_str());
}