#include "utils.hpp"

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "mutacion.hpp"

namespace Utils {

// CROMOSOMA A CADENA DE CARACTERES
std::string chromosomeToString(const std::vector<int>& chromo) {
    std::ostringstream oss;
    for (int val : chromo) {
        oss << val << "-";
    }
    return oss.str();
}

// REMOVER DUPLICADOS
void removeDuplicates(std::vector<std::shared_ptr<Individual>>& population,
                      IMutationOperator& mutator) {
    std::unordered_set<std::string> seen;

    // RECORRER LA POBLACION POR INDIVIDUO
    for (auto& individual : population) {
        // OBTENER CROMOSOMA
        std::string repr = chromosomeToString(individual->getDigitChromosome());

        // APLICAR MUTACION SI ENCUENTRA UN INDIVIDUO CON LA MISMA PERMUTACION
        while (seen.find(repr) != seen.end()) {
            mutator.mutate(individual);
            repr = chromosomeToString(individual->getDigitChromosome());
        }

        // INSERTA EL NUEVO INDIVIDUO
        seen.insert(repr);
    }
}

// INVERTIR SUBSECUENCIA
void invertSubsequence(std::vector<int>& chromosome, int value1, int value2) {
    // SE INSTANCIA LOS VALORES PARA DELIMITAR LA SUBSECUENCIA
    auto it1 = std::find(chromosome.begin(), chromosome.end(), value1);
    auto it2 = std::find(chromosome.begin(), chromosome.end(), value2);

    if (it1 == chromosome.end() || it2 == chromosome.end()) {
        throw std::invalid_argument("ERROR DE VALORES");
    }

    // INTERCAMBIA LOS VALORES DE LOS DELIMITADORES SI T1 > T2
    if (it1 > it2)
        std::swap(it1, it2);

    // INVIERTA LA SUBSECUENCIA
    std::reverse(it1, it2 + 1);
}

// INVERSION
void applyInversion(std::shared_ptr<Individual> individual, double inversionRate, int value1,
                    int value2, InversionPolicy policy) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // SI LA PROBABILIDAD ES MAYOR A LA INVERSION, NO HACE NADA
    if (probDist(gen) >= inversionRate) {
        return;
    }

    // GUARDA EL CROMOSOMA ORIGINAL
    auto originalChromo = individual->getDigitChromosome();

    // APLICA INVERSION
    auto newChromo = originalChromo;
    invertSubsequence(newChromo, value1, value2);

    // PARA LA POLITICA DE ONLY_IF_BETTER SE REQUIERE EVALUAR AL INDIVIDUO Y AL INDIVIDUO CON
    // INVERSION
    Individual temp(newChromo, individual->getEncoding());
    temp.decode();
    double originalFitness = individual->getFitness();
    double newFitness = 0.0;

    // SWITCH DE LAS POLITICAS DE INVERSION
    switch (policy) {
        // POLITICA: SIEMPRE REMPLAZO
        case InversionPolicy::ALWAYS_REPLACE:
            individual->setDigitChromosome(newChromo);
            individual->setFitness(newFitness);
            break;

            // POLITICA: REMPLAZAR SI ES MEJOR
        case InversionPolicy::ONLY_IF_BETTER:
            if (newFitness > originalFitness) {
                individual->setDigitChromosome(newChromo);
                individual->setFitness(newFitness);
            }
            break;
            // POLITICA: ALEATORIA
        case InversionPolicy::RANDOM_CHOICE:
            if (dist(gen) < 0.5) {
                individual->setDigitChromosome(newChromo);
                individual->setFitness(newFitness);
            }
            break;
    }
}
}  // namespace Utils
