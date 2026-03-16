#include "Math_bp.h"

namespace MathBP
{
    BigInt nthRoot(const BigInt &D, int n)
    {
        if (D == BigInt(0))
            return BigInt(0);
        if (D < BigInt(0) && n % 2 == 0)
            throw std::invalid_argument("Párna odmocnina záporného čísla.");

        // 1. Počiatočný odhad (x0)
        // Pre jednoduchosť môžeme začať s D, ale lepšie je odhadnúť podľa počtu bitov
        BigInt x = D;
        BigInt prevX;

        // 2. Iteračný proces
        const BigInt bigN(n);
        const BigInt bigNMinus1(n - 1);

        while (true)
        {
            // Newtonov vzorec: x_next = ((n-1)*x + D / x^(n-1)) / n

            BigInt xPow = BigInt::power(x, n - 1);
            BigInt xNext = (bigNMinus1 * x + D / xPow) / bigN;

            // Kontrola konvergencie (ak sa hodnota prestane meniť alebo začne kmitať)
            if (xNext >= x || xNext == prevX)
            {
                break;
            }
            prevX = x;
            x = xNext;
        }
        return x;
    }

    BigInt gcd(const BigInt &a, const BigInt &b)
    {
        BigInt x = (a < BigInt(0)) ? a * BigInt(-1) : a;
        BigInt y = (b < BigInt(0)) ? b * BigInt(-1) : b;

        while (y != BigInt(0))
        {
            BigInt temp = y;
            y = x % y;
            x = temp;
        }

        return x;
    }

}
