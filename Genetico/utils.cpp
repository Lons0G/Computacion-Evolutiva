#include "utils.hpp"

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "mutacion.hpp"

namespace Utils {

std::string chromosomeToString(const std::vector<int>& chromo) {
    std::ostringstream oss;
    for (int val : chromo) {
        oss << val << "-";
    }
    return oss.str();
}

void removeDuplicates(std::vector<std::shared_ptr<Individual>>& population,
                      IMutationOperator& mutator) {
    std::unordered_set<std::string> seen;

    for (auto& individual : population) {
        std::string repr = chromosomeToString(individual->getDigitChromosome());

        while (seen.find(repr) != seen.end()) {
            mutator.mutate(individual);
            repr = chromosomeToString(individual->getDigitChromosome());
        }

        seen.insert(repr);
    }
}

void invertSubsequence(std::vector<int>& chromosome, int value1, int value2) {
    auto it1 = std::find(chromosome.begin(), chromosome.end(), value1);
    auto it2 = std::find(chromosome.begin(), chromosome.end(), value2);

    if (it1 == chromosome.end() || it2 == chromosome.end()) {
        throw std::invalid_argument("Uno de los valores no se encontró en el cromosoma");
    }

    if (it1 > it2)
        std::swap(it1, it2);

    std::reverse(it1, it2 + 1);
}

void applyInversion(std::shared_ptr<Individual> individual, double inversionRate, int value1,
                    int value2, InversionPolicy policy) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::uniform_real_distribution<double> coin(0.0, 1.0);

    if (probDist(gen) >= inversionRate) {
        // No se aplica inversión
        return;
    }

    // Guarda el cromosoma original
    auto originalChromo = individual->getDigitChromosome();

    // Aplica inversión
    auto newChromo = originalChromo;
    invertSubsequence(newChromo, value1, value2);

    // Evalúa fitness nuevo
    // Usa tu función evaluadora real aquí. Ejemplo con De Jong:
    Individual temp(newChromo, individual->getEncoding());
    double originalFitness = individual->getFitness();
    double newFitness = 0.0;
    switch (policy) {
        case InversionPolicy::ALWAYS_REPLACE:
            individual->setDigitChromosome(newChromo);
            individual->setFitness(newFitness);
            break;

        case InversionPolicy::ONLY_IF_BETTER:
            if (newFitness > originalFitness) {
                individual->setDigitChromosome(newChromo);
                individual->setFitness(newFitness);
            }
            break;

        case InversionPolicy::RANDOM_CHOICE:
            if (coin(gen) < 0.5) {
                individual->setDigitChromosome(newChromo);
                individual->setFitness(newFitness);
            }
            break;
    }
}
}  // namespace Utils
