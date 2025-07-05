#ifndef PROBLEM3_HPP
#define PROBLEM3_HPP

#include <cmath>
#include <iostream>
#include <vector>

class Problem3 {
   public:
    Problem3() = default;

    // ⚙️ 1) Función objetivo
    double evaluateObjective(const std::vector<double>& x) const {
        return std::exp(x[0] * x[1] * x[2] * x[3] * x[4]);
    }

    // ⚙️ 2) Restricciones
    std::vector<double> constraints(const std::vector<double>& x) const {
        std::vector<double> violations;

        // h1(x): sum xi^2 - 10 = 0
        double sum_sq = 0.0;
        for (double xi : x)
            sum_sq += xi * xi;
        double h1 = sum_sq - 10.0;

        // h2(x): x2 * x3 - 5 * x4 * x5 = 0
        double h2 = x[1] * x[2] - 5.0 * x[3] * x[4];

        // h3(x): x1^3 + x2^3 + 1 = 0
        double h3 = std::pow(x[0], 3) + std::pow(x[1], 3) + 1.0;

        violations.push_back(std::abs(h1) > 1e-4 ? std::abs(h1) : 0.0);
        violations.push_back(std::abs(h2) > 1e-4 ? std::abs(h2) : 0.0);
        violations.push_back(std::abs(h3) > 1e-4 ? std::abs(h3) : 0.0);

        return violations;
    }

    // ⚙️ 3) Penalización
    double penalty(const std::vector<double>& x) const {
        double rho = 1000.0;  // Ajusta este factor
        auto v = constraints(x);

        double sum_penalty = 0.0;
        for (double viol : v)
            sum_penalty += rho * viol;

        return sum_penalty;
    }

    // ⚙️ 4) Evaluación final
    double evaluate(const std::vector<double>& x) const {
        return evaluateObjective(x) + penalty(x);  // Minimización
    }

    void printStatus(const std::vector<double>& x) const {
        std::cout << "Objetivo f(x): " << evaluateObjective(x) << "\n";
        auto vs = constraints(x);
        for (size_t i = 0; i < vs.size(); ++i)
            std::cout << "Restricción " << i + 1 << ": violación = " << vs[i] << "\n";
        std::cout << "Penalización total: " << penalty(x) << "\n";
        std::cout << "Evaluación final: " << evaluate(x) << "\n";
    }
};

#endif  // PROBLEM3_HPP
