#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "cruza.hpp"
#include "individuo.hpp"
#include "mutacion.hpp"
#include "poblacion.hpp"
#include "seleccion.hpp"
#include "utils.hpp"

// === Lectura archivo .dat ===
void readQAPFile(const std::string& filename, int& n, std::vector<std::vector<int>>& dist,
                 std::vector<std::vector<int>>& flujo) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("No se pudo abrir archivo: " + filename);

    file >> n;
    dist.assign(n, std::vector<int>(n));
    flujo.assign(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> dist[i][j];

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> flujo[i][j];
}

// === Evaluación de costo QAP ===
double evaluateQAP(const std::vector<int>& perm, const std::vector<std::vector<int>>& dist,
                   const std::vector<std::vector<int>>& flujo) {
    double cost = 0.0;
    int n = perm.size();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cost += dist[i][j] * flujo[perm[i]][perm[j]];
    return cost;
}

int main() {
    std::string filename;
    std::cout << "Ingrese nombre del archivo .dat: ";
    std::cin >> filename;

    int n;
    std::vector<std::vector<int>> dist, flujo;
    readQAPFile(filename, n, dist, flujo);

    std::cout << "Archivo cargado. Tamaño n = " << n << "\n";

    const int POP_SIZE = 6;
    const int MAX_GENERATIONS = 3;

    Individual::setSeed(42);
    OrderBasedCrossover::setSeed(42);
    InsertionMutation::setSeed(42);

    OrderBasedCrossover obx(1.0, 0.5);  // 100% cruza, 50% genes
    auto mutator = MutationFactory::create(MutationFactory::Type::INSERCION, 1.0);
    Torneo_Binario_Deterministico selector;

    // === Población inicial ===
    Population population(POP_SIZE, n, Individual::Encoding::INTEGER_DECIMAL);

    for (int i = 0; i < population.size(); ++i) {
        auto ind = population.getIndividual(i);
        double fx = evaluateQAP(ind->getDigitChromosome(), dist, flujo);
        ind->setFitness(1.0 / (1.0 + fx));
    }

    Utils::removeDuplicates(population.getIndividuals(), *mutator);

    population.printPopulation("Generación 0 (sin duplicados)");

    for (int gen = 1; gen <= MAX_GENERATIONS; ++gen) {
        std::cout << "\n=== Generación " << gen << " ===\n";

        // Selección por Torneo Binario Determinístico
        auto selected = selector.selectMany(population, POP_SIZE);

        Population newPop;

        for (size_t i = 0; i + 1 < selected.size(); i += 2) {
            auto p1 = selected[i];
            auto p2 = selected[i + 1];

            std::cout << "\nPadre 1:\n";
            p1->printIndividual();
            std::cout << "Padre 2:\n";
            p2->printIndividual();

            auto [h1, h2] = obx.crossover(p1, p2);

            std::cout << "Hijo 1 (cruza):\n";
            h1->printIndividual();
            std::cout << "Hijo 2 (cruza):\n";
            h2->printIndividual();

            mutator->mutate(h1);
            mutator->mutate(h2);

            std::cout << "Hijo 1 (mutado):\n";
            h1->printIndividual();
            std::cout << "Hijo 2 (mutado):\n";
            h2->printIndividual();

            Utils::applyInversion(h1, 1.0, p1->getDigitChromosome()[0],
                                  p1->getDigitChromosome()[n - 1],
                                  Utils::InversionPolicy::ALWAYS_REPLACE);
            Utils::applyInversion(h2, 1.0, p2->getDigitChromosome()[0],
                                  p2->getDigitChromosome()[n - 1],
                                  Utils::InversionPolicy::ALWAYS_REPLACE);

            std::cout << "Hijo 1 (inversión):\n";
            h1->printIndividual();
            std::cout << "Hijo 2 (inversión):\n";
            h2->printIndividual();

            double fx1 = evaluateQAP(h1->getDigitChromosome(), dist, flujo);
            double fx2 = evaluateQAP(h2->getDigitChromosome(), dist, flujo);
            h1->setFitness(1.0 / (1.0 + fx1));
            h2->setFitness(1.0 / (1.0 + fx2));

            newPop.addIndividual(h1);
            if (newPop.size() < POP_SIZE)
                newPop.addIndividual(h2);
        }

        Utils::removeDuplicates(newPop.getIndividuals(), *mutator);
        newPop.printPopulation("Población tras remover duplicados");

        population = newPop;
    }

    std::cout << "\n=== Mejor individuo final ===\n";
    population.getFittest()->printIndividual();
    double bestFx = evaluateQAP(population.getFittest()->getDigitChromosome(), dist, flujo);
    std::cout << "Costo QAP: " << bestFx << "\n";

    return 0;
}
