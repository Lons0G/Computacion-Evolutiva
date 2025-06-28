#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "F5DeJong.hpp"
#include "cruza.hpp"
#include "individuo.hpp"
#include "mutacion.hpp"
#include "poblacion.hpp"
#include "seleccion.hpp"

int main() {
    // Parámetros interactivos
    int POP_SIZE, MAX_GENERATIONS, SEED;
    double CROSS_PROB, MUT_RATE;
    std::string outputFilename;

    std::cout << "Ingrese el tamaño de la población: ";
    std::cin >> POP_SIZE;
    std::cout << "Ingrese el porcentaje de cruza (0-1): ";
    std::cin >> CROSS_PROB;
    std::cout << "Ingrese el porcentaje de mutación (0-1): ";
    std::cin >> MUT_RATE;
    std::cout << "Ingrese el número máximo de generaciones: ";
    std::cin >> MAX_GENERATIONS;
    std::cout << "Ingrese un numero entero para la semilla: ";
    std::cin >> SEED;
    std::cout << "Ingrese el nombre del archivo de salida: ";
    std::cin >> outputFilename;

    std::cout << "semilla: " << SEED << std::endl;

    std::ofstream logFile(outputFilename);
    std::ofstream eliteFile("elite_log_real.csv");

    const int DIM = 2;
    const int DIGITS_PER_VAR = 5;

    Individual::setSeed(SEED);
    UniformCrossover::setSeed(SEED);
    UniformMutation::setSeed(SEED);
    Individual::setDecoding(-65.536, 65.536, DIM, DIGITS_PER_VAR * DIM,
                            Individual::Encoding::INTEGER_DECIMAL);

    auto crossover = CrossoverFactory::create(CrossoverFactory::Type::UNIFORM, CROSS_PROB, 0.5);
    auto mutator = MutationFactory::create(MutationFactory::Type::UNIFORME, MUT_RATE);
    Universal_Estocastica susSelector;

    Population population(POP_SIZE, DIGITS_PER_VAR * DIM, Individual::Encoding::INTEGER_DECIMAL);

    for (int i = 0; i < population.size(); ++i) {
        auto ind = population.getIndividual(i);
        ind->decode();
        double fx = DeJong::F5::evaluate(ind->getVariables());
        ind->setFitness(1.0 / (1.0 + fx));
    }

    Individual bestGlobal = *population.getFittest();

    logFile << "Generación,Media,Mejor,Peor,Cruzas,Mutaciones,Mejor_FX,Variables\n";
    eliteFile << "Generación,Fx,Var1,Var2\n";

    for (int gen = 1; gen <= MAX_GENERATIONS; ++gen) {
        auto elite = population.getFittest();
        Population newPopulation;
        auto selected = susSelector.selectMany(population, POP_SIZE);

        int crosses = 0;
        int mutations = 0;

        for (size_t i = 0; i + 1 < selected.size(); i += 2) {
            auto [h1, h2] = crossover->crossover(selected[i], selected[i + 1]);
            ++crosses;

            mutator->mutate(h1);
            mutations++;
            mutator->mutate(h2);
            mutations++;

            h1->decode();
            double fx1 = DeJong::F5::evaluate(h1->getVariables());
            h1->setFitness(1.0 / (1.0 + fx1));

            h2->decode();
            double fx2 = DeJong::F5::evaluate(h2->getVariables());
            h2->setFitness(1.0 / (1.0 + fx2));

            newPopulation.addIndividual(h1);
            if (newPopulation.size() < POP_SIZE)
                newPopulation.addIndividual(h2);
        }

        newPopulation.Replace(elite);
        population = newPopulation;

        double avg = population.getAverageFitness();
        double min = population.getWorstFitness();
        double max = population.getFittest()->getFitness();

        auto bestGen = population.getFittest();
        double fxBest = DeJong::F5::evaluate(bestGen->getVariables());
        if (bestGen->getFitness() > bestGlobal.getFitness()) {
            bestGlobal = *bestGen;
        }

        logFile << gen << "," << std::fixed << std::setprecision(6) << avg << "," << max << ","
                << min << "," << crosses << "," << mutations << "," << std::setprecision(6)
                << fxBest << ",";
        for (double v : bestGen->getVariables())
            logFile << std::setprecision(3) << v << " ";
        logFile << "\n";

        eliteFile << gen << "," << std::fixed << std::setprecision(6) << fxBest << ","
                  << std::setprecision(3) << bestGen->getVariables()[0] << ","
                  << std::setprecision(3) << bestGen->getVariables()[1] << "\n";
    }

    std::cout << "\n=== MEJOR INDIVIDUO GLOBAL ===\n";
    bestGlobal.printIndividual();
    std::cout << "Evaluación F5 del mejor individuo global: ";
    DeJong::F5::printFxWithVariables(bestGlobal.getVariables());

    logFile.close();
    eliteFile.close();
    return 0;
}
