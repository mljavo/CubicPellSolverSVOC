#include "CubicSolverLLL.h"
#include <NTL/LLL.h>
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <iostream>

using namespace std;
using namespace NTL;

void CubicSolverLLL::solveCubicEquationLLL(const ZZ &d, ResultExporter &exporter)
{

    RR::SetPrecision(1024);
    RR theta = exp(log(conv<RR>(d)) / 3.0);
    RR theta2 = theta * theta;

    //  Postupne zvyšujeme K (citlivosť na lineárnu formu)
    for (long exp_val = 0; exp_val <= 150; exp_val += 2)
    {
        RR K = pow(conv<RR>(10), conv<RR>(exp_val));

        mat_ZZ B;
        B.SetDims(3, 3);

        // Riadok 0: reprezentuje x
        B[0][0] = RoundToZZ(K);
        B[0][1] = 0;
        B[0][2] = 0;

        // Riadok 1: reprezentuje y (násobíme K * theta)
        B[1][0] = RoundToZZ(K * theta);
        B[1][1] = 1;
        B[1][2] = 0;

        // Riadok 2: reprezentuje z (násobíme K * theta^2)
        B[2][0] = RoundToZZ(K * theta2);
        B[2][1] = 0;
        B[2][2] = 1;

        ZZ det;
        LLL(det, B);

        for (long i = 0; i < 3; i++)
        {

            ZZ y = B[i][1];
            ZZ z = B[i][2];

            RR real_x = -(conv<RR>(y) * theta + conv<RR>(z) * theta2);
            ZZ x = RoundToZZ(real_x);
            for (long dx = -1; dx <= 1; dx++)
            {
                ZZ curX = x + dx;
                if (IsZero(curX) && IsZero(y) && IsZero(z))
                    continue;
                if (abs(curX) == 1 && IsZero(y) && IsZero(z))
                    continue;

                ZZ n = calculateNorm(curX, y, z, d);

                if (n == 1 || n == -1)
                {
                    if (n == -1)
                    {
                        ZZVec squared = squareSolution(x, y, z, d);
                        x = squared[0];
                        y = squared[1];
                        z = squared[2];
                        n = calculateNorm(x, y, z, d);
                        if (n != 1)
                        {
                            cerr << "Chyba pri umocňovaní riešenia na druhú!" << endl;
                            return;
                        }
                        squared.kill();
                    }

                    std::stringstream sx, sy, sz, sn, sd;
                    sx << x;
                    sy << y;
                    sz << z;
                    sn << n;
                    sd << d;

                    std::vector<std::string> coeffs = {sx.str(), sy.str(), sz.str()};

                    exporter.addRawResult("LLL_Cubic", 3, sd.str(), coeffs, sn.str());
                    return;
                }
            }
        }
    }
    cout << "Riesenie nebolo najdene v stanovenom rozsahu." << endl;
}

ZZ CubicSolverLLL::calculateNorm(const ZZ &x, const ZZ &y, const ZZ &z, const ZZ &d)
{
    ZZ term1 = x * x * x;
    ZZ term2 = d * (y * y * y);
    ZZ term3 = (d * d) * (z * z * z);
    ZZ term4 = ZZ(3) * d * x * y * z;

    return term1 + term2 + term3 - term4;
}

ZZVec CubicSolverLLL::squareSolution(const ZZ &x, const ZZ &y, const ZZ &z, const ZZ &d)
{
    ZZVec result(3, 10);

    result[0] = x * x + 2 * d * y * z; // new_x
    result[1] = 2 * x * y + d * z * z; // new_y
    result[2] = y * y + 2 * x * z;     // new_z

    return result;
}