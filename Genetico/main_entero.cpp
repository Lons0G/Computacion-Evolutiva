#include <fstream>
#include <iostream>
#include <string>

#include "cruza.hpp"
#include "individuo.hpp"
#include "mutacion.hpp"
#include "poblacion.hpp"
#include "seleccion.hpp"
#include "utils.hpp"

// LEER ARCHIVO
void readFile(const std::string& filename, int& n, std::vector<std::vector<int>>& dist,
              std::vector<std::vector<int>>& flujo) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("NO SE PUDO ABRIR EL ARCHIVO: " + filename);

    file >> n;
    dist.assign(n, std::vector<int>(n));
    flujo.assign(n, std::vector<int>(n));

    // MATRIZ DE DISTANCIA
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> dist[i][j];

    // MATRIZ DE FLUJO
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> flujo[i][j];
}

// FUNCION DE EVALUACION
double evaluate(const std::vector<int>& perm, const std::vector<std::vector<int>>& dist,
                const std::vector<std::vector<int>>& flujo) {
    int n = perm.size();
    double cost = 0.0;

    // SUMATORIA
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cost += dist[i][j] * flujo[perm[i]][perm[j]];
    return cost;
}

int main() {
    // PARAMETROS
    int POP_SIZE, MAX_GENERATIONS, SEED;
    double CROSS_PROB, MUT_RATE, INVERSION_RATE;
    std::string outputFilename, filename;

    std::cout << "INGRESE EL NOMBRE DEL ARCHIVO (ejemplo.dat): ";
    std::cin >> filename;
    std::cout << "INGRESE EL TAMANIO DE LA POBLACION: ";
    std::cin >> POP_SIZE;
    std::cout << "INGRESE EL PORCENTAJE DE CRUZA (0-1): ";
    std::cin >> CROSS_PROB;
    std::cout << "INGRESE EL PORCENTAJE DE MUTACION (0-1): ";
    std::cin >> MUT_RATE;
    std::cout << "INGRESE EL PORCENTAJE DE INVERSION (0-1): ";
    std::cin >> INVERSION_RATE;
    std::cout << "INGRESE EL NUMERO MAXIMO DE GENERACIONES: ";
    std::cin >> MAX_GENERATIONS;
    std::cout << "INGRESE UN NUMERO ENTERO PARA LA SEMILLA: ";
    std::cin >> SEED;
    std::cout << "INGRESE NOMBRE DEL ARCHIVO DE SALIDA: ";
    std::cin >> outputFilename;

    std::cout << "SEMILLA: " << SEED << "\n";

    int n;
    std::vector<std::vector<int>> dist, flujo;

    // ARCHIVO PARA GUARDAR EL PROGRESO
    readFile(filename, n, dist, flujo);

    std::ofstream logFile(outputFilename);
    std::ofstream eliteFile("elite_log_.csv");

    // INICIALIZACION DEL ALGORITMO GENETICO
    Individual::setSeed(SEED);
    OrderBasedCrossover::setSeed(SEED);
    InsertionMutation::setSeed(SEED);

    OrderBasedCrossover obx(CROSS_PROB, 0.5);
    auto mutator = MutationFactory::create(MutationFactory::Type::INSERCION, MUT_RATE);
    Torneo_Binario_Deterministico selector;

    // INICIALIZACION DE LA POBLACION
    Population population(POP_SIZE, n, Individual::Encoding::INTEGER_DECIMAL);
    for (int i = 0; i < population.size(); ++i) {
        auto ind = population.getIndividual(i);
        double fx = evaluate(ind->getDigitChromosome(), dist, flujo);
        ind->setFitness(1.0 / (1.0 + fx));
    }

    // REMOVER INDIVIDUOS DUPLICADOS
    Utils::removeDuplicates(population.getIndividuals(), *mutator);

    // MEJOR INDIVIDUO
    Individual bestGlobal = *population.getFittest();

    // COLUMNAS DE LOS ARCHIVOS
    logFile << "Generación,Media,Mejor,Peor,Cruzas,Mutaciones,Inversiones,Costo,Permutación\n";
    eliteFile << "Generación,Costo,Permutación\n";

    // CICLO
    int gen = 1;
    while (gen < MAX_GENERATIONS) {
        // std::cout << "\n=== GENERACION " << gen << " ===" << std::endl;

        // OBTENER EL MEJOR INDIVIDUO DE LA GENERACION
        auto elite = population.getFittest();

        // CREAR NUEVA POBLACION
        Population newPop;

        // SELECCIONAR N INDIVIDUOS
        auto selected = selector.selectMany(population, POP_SIZE);

        int crosses = 0, mutations = 0, inversions = 0;

        // CRUZAR, MUTAR, INVERTIR Y EVALUAR LOS HIJOS
        for (size_t i = 0; i + 1 < selected.size(); i += 2) {
            // OBTENER PADRES
            auto p1 = selected[i];
            auto p2 = selected[i + 1];

            // GENERAR HIJOS
            auto [h1, h2] = obx.crossover(p1, p2);
            ++crosses;

            // MUTAR LOS HIJOS
            mutator->mutate(h1);
            mutator->mutate(h2);
            mutations += 2;

            // INVERSIO CON POLITICA DE REMPLAZAR SIEMPRE
            Utils::applyInversion(h1, INVERSION_RATE, p1->getDigitChromosome()[0],
                                  p1->getDigitChromosome()[n - 1],
                                  Utils::InversionPolicy::ALWAYS_REPLACE);
            Utils::applyInversion(h2, INVERSION_RATE, p2->getDigitChromosome()[0],
                                  p2->getDigitChromosome()[n - 1],
                                  Utils::InversionPolicy::ALWAYS_REPLACE);
            inversions += 2;

            // EVALUACION DE LOS HIJOS
            double fx1 = evaluate(h1->getDigitChromosome(), dist, flujo);
            double fx2 = evaluate(h2->getDigitChromosome(), dist, flujo);
            h1->setFitness(1.0 / (1.0 + fx1));
            h2->setFitness(1.0 / (1.0 + fx2));

            // AGREGAR INDIVIDUOS A LA POBLACION
            newPop.addIndividual(h1);
            if (newPop.size() < POP_SIZE)
                newPop.addIndividual(h2);
        }

        // REMOVER DUPLICADOS
        Utils::removeDuplicates(newPop.getIndividuals(), *mutator);

        // REMPLAZAR AL PEOR INDIVIDUO POR EL MEJOR DE LA GENERACION ANTERIOR
        newPop.Replace(elite);

        // ESTADISTICAS
        double avg = newPop.getAverageFitness();
        double min = newPop.getWorstFitness();
        double max = newPop.getFittest()->getFitness();

        // OBTENER AL MEJOR INDIVIDUO
        auto bestGen = newPop.getFittest();
        double bestCost = evaluate(bestGen->getDigitChromosome(), dist, flujo);
        bestGen->setFitness(1.0 / (1.0 + bestCost));  // RE-SINCRONIZA fitness

        if (bestGen->getFitness() > bestGlobal.getFitness()) {
            bestGlobal = *bestGen;
            bestGlobal.setFitness(1.0 / (1.0 + bestCost));
        }

        // COLOCAR ESTADISTICAS EN LOS ARCHIVOS
        logFile << gen << "," << avg << "," << max << "," << min << "," << crosses << ","
                << mutations << "," << inversions << "," << bestCost << ",";
        for (int val : bestGen->getDigitChromosome()) {
            logFile << val << " ";
        }
        logFile << "\n";

        eliteFile << gen << "," << bestCost << ",";
        for (int val : bestGen->getDigitChromosome()) {
            eliteFile << val << " ";
        }
        eliteFile << "\n";

        // IMPRIMIR ESTADISTICAS EN PANTALLA
        // std::cout << "Media aptitud: " << avg << "\n";
        // std::cout << "Máxima aptitud: " << max << "\n";
        // std::cout << "Mínima aptitud: " << min << "\n";
        // std::cout << "Cruzas: " << crosses << ", Mutaciones: " << mutations
        //          << ", Inversiones: " << inversions << "\n";

        //// IMPRESION DEL MEJOR INDIVIDUO DE LA GENERACION
        // std::cout << "|--- MEJOR INDIVIDUO DE LA GENERACION " << gen << " ---|" << std::endl;
        // bestGen->printIndividual();
        // std::cout << "COSTO : " << bestCost << std::endl;

        // REMPLAZO DE LA GENERACION
        population = newPop;
        gen++;
    }

    // IMPRIMIR ESTADISTICAS FINALES
    std::cout << "\n|--- MEJOR INDIVIDUO GLOBAL ---|" << std::endl;
    bestGlobal.printIndividual();
    std::cout << "COSTO : " << std::fixed << evaluate(bestGlobal.getDigitChromosome(), dist, flujo)
              << "\n";
    // CERRAR ARCHIVOS
    logFile.close();
    eliteFile.close();

    return 0;
}
