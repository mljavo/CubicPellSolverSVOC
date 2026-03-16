#include "CubicSolver.h"
#include <cmath>
#include <algorithm>

CubicNumber CubicSolver::findCubicSolution(const BigInt &d, long limit)
{
    double d_double = std::stod(d.toString());
    double theta = std::pow(d_double, 1.0 / 3.0);
    double theta2 = theta * theta;

    CubicNumber candidate(d, BigInt(0), BigInt(0), BigInt(0));

    // Iterujeme cez "polomer" r, čím zabezpečíme, že prvé nájdené riešenie
    // bude mať najmenšie možné koeficienty (fundamentálne riešenie).
    for (long r = 1; r <= limit; ++r)
    {
        // Prechádzame všetky kombinácie y a z také, že |y| + |z| = r
        for (long z = -r; z <= r; ++z)
        {
            long y_abs = r - std::abs(z);
            // Skúsime kladné aj záporné y pre dané z
            long vals[2] = {-y_abs, y_abs};
            int count = (y_abs == 0) ? 1 : 2;
            for (int i = 0; i < count; ++i)
            {
                long y = vals[i];

                // Odhadneme x pomocou lineárnej formy
                double x_approx = -(static_cast<double>(y) * theta + static_cast<double>(z) * theta2);
                long x_base = std::round(x_approx);

                // Kontrolujeme okolie x_base
                for (long x : {x_base, x_base - 1, x_base + 1})
                {
                    // Rýchly filter (double)
                    double xd = (double)x;
                    double yd = (double)y;
                    double zd = (double)z;
                    double approxNorm = (xd * xd * xd) + (d_double * yd * yd * yd) +
                                        (d_double * d_double * zd * zd * zd) - (3.0 * d_double * xd * yd * zd);

                    if (std::abs(approxNorm - 1.0) > 0.1)
                        continue;

                    // Presné overenie cez BigInt normu
                    candidate.setCoefficient(0, BigInt(std::to_string(x)));
                    candidate.setCoefficient(1, BigInt(std::to_string(y)));
                    candidate.setCoefficient(2, BigInt(std::to_string(z)));

                    if (candidate.norm() == BigInt(1))
                    {
                        return candidate;
                    }
                }
            }
        }
    }
    throw std::runtime_error("Fundamentalne riesenie sa v danom limite nenaslo.");
}