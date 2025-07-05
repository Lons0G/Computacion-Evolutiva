#ifndef PROBLEM2_HPP
#define PROBLEM2_HPP

#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class Problem2 {
   public:
    Problem2() = default;

    // ⚙️ 1) Función objetivo
    double evaluateObjective(const std::vector<double>& x) const {
        double x1 = x[0];
        double x2 = x[1];

        double f =
            3.0 * x1 + 0.000001 * std::pow(x1, 3) + 2.0 * x2 + (0.000002 / 3.0) * std::pow(x2, 3);
        return f;
    }

    // ⚙️ 2) Restricciones
    std::vector<double> constraints(const std::vector<double>& x) const {
        std::vector<double> violations;

        double x1 = x[0], x2 = x[1], x3 = x[2], x4 = x[3];

        // g1, g2 (desigualdad)
        double g1 = -x4 + x3 - 0.55;
        double g2 = -x3 + x4 - 0.55;

        violations.push_back(std::max(0.0, g1));
        violations.push_back(std::max(0.0, g2));

        // h3, h4, h5 (igualdad)
        double h3 = 1000 * sin(-x3 - 0.25) + 1000 * sin(-x4 - 0.25) + 894.8 - x1;
        double h4 = 1000 * sin(x3 - 0.25) + 1000 * sin(x3 - x4 - 0.25) + 894.8 - x2;
        double h5 = 1000 * sin(x4 - 0.25) + 1000 * sin(x4 - x3 - 0.25) + 1294.8;

        if (std::abs(h3) > 1e-4)
            violations.push_back(std::abs(h3));
        else
            violations.push_back(0.0);

        if (std::abs(h4) > 1e-4)
            violations.push_back(std::abs(h4));
        else
            violations.push_back(0.0);

        if (std::abs(h5) > 1e-4)
            violations.push_back(std::abs(h5));
        else
            violations.push_back(0.0);

        return violations;
    }

    // ⚙️ 3) Penalización
    double penalty(const std::vector<double>& x) const {
        double rho = 1000.0;  // Factor de penalización ajustable
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

#endif  // PROBLEM2_HPP
