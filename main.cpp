#include <iostream>
#include <chrono>
#include "BigInt.h"
#include "ResultExporter/ResultExporter.h"
#include "Utilities/Math_bp.h"
#include "CubicSolver/CubicSolver.h"
#include "CubicNumber/CubicNumber.h"
#include "CubicSolver/CubicSolverLLL.h"
#include <NTL/ZZ.h>
#include <NTL/Vec_ZZ.h>
#include <fstream>
#include <string>
#include <algorithm>

void runBenchmark()
{
    std::vector<PerfMetric> metrics;
    ResultExporter exporter("results.json");

    std::vector<int> presentation_D = {2, 3, 5, 7, 10, 15, 20, 26, 28, 40, 63, 65, 100, 150, 215, 217, 300, 342, 344,
                                       400,
                                       493, 511, 513, 600, 728, 730, 850, 999, 1001, 1150, 1330, 1332, 1450, 1599, 1727, 1729, 1850, 2000,
                                       2150, 2196, 2198, 2250, 2300, 2350, 2400, 2420, 2440, 2460, 2480, 2500};

    for (int d_val : presentation_D)
    {
        BigInt D(d_val);
        std::cout << "D: " << D.toString() << std::endl;

        // 0. Brute-force Cubic Solver
        if (D < 41) // Vynecháme vyssie D
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto solCubic = CubicSolver::findCubicSolution(D, 100000);
            auto end = std::chrono::high_resolution_clock::now();
            auto durationCubic = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            metrics.push_back({"CubicSolver_BruteForce", durationCubic, D.toString()});
            exporter.addResult(solCubic);
        }

        // // 1. LLL Solver
        auto start = std::chrono::high_resolution_clock::now();
        CubicSolverLLL::solveCubicEquationLLL(D.toZZ(), exporter);
        auto end = std::chrono::high_resolution_clock::now();
        auto durationLLL = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        metrics.push_back({"CubicSolver_LLL", durationLLL, D.toString()});
    }

    exporter.save();
    ResultExporter::saveToFile(ResultExporter::toPerfJSON(metrics), "perf_data.json");
}

int main(int argc, char **argv)
{
    runBenchmark();
    return 0;
}
