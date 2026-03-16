#ifndef BIGINT_H
#define BIGINT_H

#include <gmp.h>
#include <string>
#include <iostream>
#include <cstring>
#include <NTL/ZZ.h>

class BigInt
{
private:
    mpz_t value;

public:
    // Constructors
    BigInt();
    BigInt(long i);
    BigInt(const std::string &str);
    BigInt(const BigInt &other);
    ~BigInt();

    // Assignment operator
    BigInt &operator=(const BigInt &other);

    // Arithmetic operators
    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &other) const;
    BigInt sqrt() const;

    BigInt &operator+=(const BigInt &other);
    BigInt &operator-=(const BigInt &other);
    BigInt &operator*=(const BigInt &other);
    BigInt &operator/=(const BigInt &other);
    BigInt &operator%=(const BigInt &other);

    // Comparison operators
    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    bool operator<(const BigInt &other) const;
    bool operator<=(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator>=(const BigInt &other) const;

    // Conversion to string
    std::string toString() const;
    // Conversion to NTL::ZZ
    NTL::ZZ toZZ() const;

    // Friend function for output
    friend std::ostream &operator<<(std::ostream &os, const BigInt &bigInt);

    static BigInt power(const BigInt &base, int exp);
};

#endif