
#ifndef PROBLEM1_HPP
#define PROBLEM1_HPP

#include <cmath>
#include <iostream>
#include <numeric>  // std::accumulate
#include <vector>

class Problema_1 {
   public:
    Problema_1(int n) : n(n) {}

    // ⚙️ 1) Función objetivo
    double evaluateObjective(const std::vector<double>& vars) const {
        double sum_cos4 = 0.0;
        double prod_cos2 = 1.0;
        double sum_ix2 = 0.0;

        for (int i = 0; i < n; ++i) {
            double xi = vars[i];
            sum_cos4 += std::pow(std::cos(xi), 4);
            prod_cos2 *= std::pow(std::cos(xi), 2);
            sum_ix2 += (i + 1) * xi * xi;
        }

        double numerator = sum_cos4 - 2.0 * prod_cos2;
        double denominator = std::sqrt(sum_ix2);

        double result = std::abs(numerator / denominator);
        return result;  // Maximizar
    }

    // ⚙️ 2) Restricciones
    std::vector<double> constraints(const std::vector<double>& vars) const {
        std::vector<double> violations;

        // g1(x): 0.75 - PROD xi <= 0
        double prod_xi = 1.0;
        for (double xi : vars)
            prod_xi *= xi;
        double g1 = 0.75 - prod_xi;
        violations.push_back(std::max(0.0, g1));

        // g2(x): SUM xi - 7.5 * n <= 0
        double sum_xi = std::accumulate(vars.begin(), vars.end(), 0.0);
        double g2 = sum_xi - 7.5 * n;
        violations.push_back(std::max(0.0, g2));

        return violations;
    }

    // ⚙️ 3) Penalización
    double penalty(const std::vector<double>& vars) const {
        double rho = 1000.0;  // Ajusta este factor
        auto violations = constraints(vars);

        double p = 0.0;
        for (double v : violations)
            p += rho * v;

        return p;
    }

    // ⚙️ 4) Evaluación final
    double evaluate(const std::vector<double>& vars) const {
        double f = evaluateObjective(vars);
        double pen = penalty(vars);
        return f - pen;  // MAXIMIZACIÓN: penalización reduce la aptitud
    }

    void printStatus(const std::vector<double>& vars) const {
        std::cout << "Objetivo f(x): " << evaluateObjective(vars) << "\n";
        auto vs = constraints(vars);
        for (size_t i = 0; i < vs.size(); ++i)
            std::cout << "Restricción " << i + 1 << ": violación = " << vs[i] << "\n";
        std::cout << "Penalización total: " << penalty(vars) << "\n";
        std::cout << "Evaluación final: " << evaluate(vars) << "\n";
    }

   private:
    int n;
};

#endif  // PROBLEM1_HPP
