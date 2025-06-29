#include <iostream>
#include <memory>
#include <vector>

#include "individuo.hpp"
#include "mutacion.hpp"
#include "utils.hpp"

// Dummy evaluador para la demostración
namespace Dummy {
double evaluate(const std::vector<int>& chromo) {
    // Ejemplo: suma simple de elementos, solo para ver diferencias
    double sum = 0.0;
    for (int val : chromo) {
        sum += val;
    }
    return sum;
}
}  // namespace Dummy

int main() {
    const int POP_SIZE = 3;
    const int CHROMO_LENGTH = 10;
    const double MUT_RATE = 1.0;  // 100% para mutar si se usa
    const double INV_RATE = 1.0;  // 100% para aplicar inversión
    int v1 = 5;
    int v2 = 8;  // Puntos de inversión

    // Semillas fijas para consistencia
    Individual::setSeed(42);
    InsertionMutation::setSeed(42);

    // Mutador (opcional para mostrar uso con duplicados)
    auto mutator = MutationFactory::create(MutationFactory::Type::INSERCION, MUT_RATE);

    // === Crea población ===
    std::vector<std::shared_ptr<Individual>> population;

    for (int i = 0; i < POP_SIZE; ++i) {
        auto ind =
            std::make_shared<Individual>(CHROMO_LENGTH, Individual::Encoding::INTEGER_DECIMAL);

        // Evalúa dummy fitness
        double fx = Dummy::evaluate(ind->getDigitChromosome());
        ind->setFitness(1.0 / (1.0 + fx));  // Fitness invertido

        population.push_back(ind);
    }

    // Muestra antes
    std::cout << "=== Población inicial ===" << std::endl;
    for (size_t i = 0; i < population.size(); ++i) {
        std::cout << "Individuo " << i << ": ";
        population[i]->printIndividual();
    }

    // === Aplica inversión ===
    std::cout << "\n=== Aplicando Inversión ===" << std::endl;
    for (size_t i = 0; i < population.size(); ++i) {
        std::cout << "\nIndividuo " << i << " ANTES: ";
        population[i]->printIndividual();

        Utils::applyInversion(
            population[i], INV_RATE, v1, v2, Utils::InversionPolicy::ALWAYS_REPLACE
            // Usa: ALWAYS_REPLACE o RANDOM_CHOICE para probar
        );

        std::cout << "Individuo " << i << " DESPUÉS: ";
        population[i]->printIndividual();
    }

    return 0;
}
