#include <fstream>
#include <iostream>
#include <string>

#include "F5DeJong.hpp"
#include "cruza.hpp"
#include "individuo.hpp"
#include "mutacion.hpp"
#include "poblacion.hpp"
#include "seleccion.hpp"

int main() {
    // PARAMETROS
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
    std::ofstream eliteFile("elite_log.csv");

    // INICIALIZACION DE LOS OPERADORES
    const int DIM = 2;
    const int BITS_PER_VAR = 17;

    Individual::setSeed(SEED);
    UniformCrossover::setSeed(SEED);
    UniformMutation::setSeed(SEED);
    Individual::setDecoding(-65.536, 65.536, DIM, BITS_PER_VAR, Individual::Encoding::GRAY);

    auto crossover = CrossoverFactory::create(CrossoverFactory::Type::UNIFORM, CROSS_PROB, 0.5);
    auto mutator = MutationFactory::create(MutationFactory::Type::UNIFORME, MUT_RATE);
    Universal_Estocastica susSelector;

    // CREA LA POBLACION CON EL TIPO DE REPRESENTACION
    Population population(POP_SIZE, DIM * BITS_PER_VAR, Individual::Encoding::GRAY);

    for (int i = 0; i < population.size(); ++i) {
        auto ind = population.getIndividual(i);
        ind->decode();
        double fx = DeJong::F5::evaluate(ind->getVariables());
        ind->setFitness(1.0 / (1.0 + fx));
    }

    Individual bestGlobal = *population.getFittest();

    logFile << "Generación,Media,Mejor,Peor,Cruzas,Mutaciones,Mejor_FX,Variables\n";
    eliteFile << "Generación,Fx,Var1,Var2\n";

    // CICLO PRINCIPAL PARA LAS GENERACIONES
    for (int gen = 1; gen <= MAX_GENERATIONS; ++gen) {
        // MANTIENE AL ELITE
        auto elite = population.getFittest();
        Population newPopulation;

        // SELECCIONA N INDIVIDUOS CON SELECCION UNIVERSAL ESTOCASTICA
        auto selected = susSelector.selectMany(population, POP_SIZE);

        int crosses = 0;
        int mutations = 0;

        // CICLO PARA REALIZAR LAS CRUZAS Y MUTACIONES
        for (size_t i = 0; i + 1 < selected.size(); i += 2) {
            // CRUZA UNIFORME
            auto [h1, h2] = crossover->crossover(selected[i], selected[i + 1]);
            ++crosses;

            // MUTACION  UNIFORME
            mutator->mutate(h1);
            mutations++;
            mutator->mutate(h2);
            mutations++;

            // EVALUACION DE LOS HIJOS
            h1->decode();
            double fx1 = DeJong::F5::evaluate(h1->getVariables());
            h1->setFitness(1.0 / (1.0 + fx1));

            h2->decode();
            double fx2 = DeJong::F5::evaluate(h2->getVariables());
            h2->setFitness(1.0 / (1.0 + fx2));

            // AGREGA A LOS HIJOS A LA NUEVA POBLACION
            newPopulation.addIndividual(h1);
            if (newPopulation.size() < POP_SIZE)
                newPopulation.addIndividual(h2);
        }

        // AGREGA AL ELITE EN LA POSICION DEL PEOR INDIVIDUO
        newPopulation.Replace(elite);
        population = newPopulation;

        // ESTADISTICAS
        double avg = population.getAverageFitness();
        double min = population.getWorstFitness();
        double max = population.getFittest()->getFitness();

        auto bestGen = population.getFittest();
        double fxBest = DeJong::F5::evaluate(bestGen->getVariables());
        if (bestGen->getFitness() > bestGlobal.getFitness()) {
            bestGlobal = *bestGen;
        }

        logFile << gen << "," << avg << "," << max << "," << min << "," << crosses << ","
                << mutations << "," << fxBest << ",";
        for (double v : bestGen->getVariables())
            logFile << v << " ";
        logFile << "\n";

        eliteFile << gen << "," << fxBest << "," << bestGen->getVariables()[0] << ","
                  << bestGen->getVariables()[1] << "\n";
    }

    std::cout << "\n=== MEJOR INDIVIDUO GLOBAL ===\n";
    bestGlobal.printIndividual();
    std::cout << "EVALUACION F5 DEL MEJOR INDIVIDUO GLOBAL: ";
    DeJong::F5::printFxWithVariables(bestGlobal.getVariables());

    logFile.close();
    eliteFile.close();
    return 0;
}
