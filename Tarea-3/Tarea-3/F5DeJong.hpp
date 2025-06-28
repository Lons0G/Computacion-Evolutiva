#ifndef F5DEJONG_HPP
#define F5DEJONG_HPP

#include <cmath>
#include <iostream>
#include <vector>

namespace DeJong {

class F5 {
   public:
    static double evaluate(const std::vector<double>& x) {
        // MATRIZ Aij
        static const int A[25][2] = {{-32, -32}, {-16, -32}, {0, -32}, {16, -32}, {32, -32},
                                     {-32, -16}, {-16, -16}, {0, -16}, {16, -16}, {32, -16},
                                     {-32, 0},   {-16, 0},   {0, 0},   {16, 0},   {32, 0},
                                     {-32, 16},  {-16, 16},  {0, 16},  {16, 16},  {32, 16},
                                     {-32, 32},  {-16, 32},  {0, 32},  {16, 32},  {32, 32}};

        // OBTENER LA SUMATORIA
        double sum = 0.0;
        for (int j = 0; j < 25; ++j) {
            double denom = j + 1;
            // (x1 - Aij) ^ 6 + (x2 - Aij) ^ 6
            denom += std::pow(x[0] - A[j][0], 6);
            denom += std::pow(x[1] - A[j][1], 6);
            // f^-1
            sum += 1.0 / denom;
        }
        // 1 / (1/5 + SUMA)
        return 1.0 / (0.002 + sum);
    }

    // IMPRESION DE LA FUNCION CON SUS VARIABLES
    static void printFxWithVariables(const std::vector<double>& x) {
        double fx = evaluate(x);
        std::cout << "f(x) = " << fx << "\tVariables: ";
        for (double v : x) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
};

}  // namespace DeJong

#endif
