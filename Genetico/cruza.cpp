#include "cruza.hpp"

#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <unordered_set>

#include "individuo.hpp"

std::mt19937 UniformCrossover::_globalGen = std::mt19937(std::random_device{}());

// INICIALIZACION DE LA SEMILLA
void UniformCrossover::setSeed(unsigned int seed) { _globalGen.seed(seed); }

// CONSTRUCTOR DE LA CRUZA UNIFORME
UniformCrossover::UniformCrossover(double crossoverProb, double crossoverGenProb)
    : _crossoverProb(crossoverProb), _crossoverGenProb(crossoverGenProb) {
    if (crossoverProb < 0.0 || crossoverProb > 1.0 || crossoverGenProb < 0.0 ||
        crossoverGenProb > 1.0) {
        throw std::invalid_argument("PARAMETROS DE PROBABILIDADES FUERA DEL RANGO [0, 1]");
    }
}

// FUNCION CRUZA UNIFORME
std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> UniformCrossover::crossover(
    const std::shared_ptr<Individual>& parent1, const std::shared_ptr<Individual>& parent2) const {
    if (parent1->size() != parent2->size()) {
        throw std::invalid_argument("NO COINCIDEN LA LONGUITUDES DE LOS CROMOSOMAS DE LOS PADRES");
    }

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Individual::Encoding enc = parent1->getEncoding();

    // PASAR LOS PADRES INTACTOS A LOS HIJOS
    if (dist(_globalGen) >= _crossoverProb) {
        auto child1 = std::make_shared<Individual>(*parent1);
        auto child2 = std::make_shared<Individual>(*parent2);
        child1->setParents(parent1, parent2);
        child2->setParents(parent1, parent2);
        return {child1, child2};
    }

    // CRUZA PARA REPRESENTACION BINARIA
    if (enc == Individual::Encoding::BINARY || enc == Individual::Encoding::GRAY) {
        std::string p1 = parent1->getChromosome();
        std::string p2 = parent2->getChromosome();

        std::string child1, child2;
        child1.reserve(p1.size());
        child2.reserve(p1.size());

        for (size_t i = 0; i < p1.size(); ++i) {
            if (dist(_globalGen) < _crossoverGenProb) {
                child1 += p1[i];
                child2 += p2[i];
            } else {
                child1 += p2[i];
                child2 += p1[i];
            }
        }

        auto offspring1 = std::make_shared<Individual>(child1, enc);
        auto offspring2 = std::make_shared<Individual>(child2, enc);
        offspring1->setParents(parent1, parent2);
        offspring2->setParents(parent1, parent2);
        return {offspring1, offspring2};
    }

    // CRUZA PARA REPRESENTACION ENTERA
    if (enc == Individual::Encoding::INTEGER_DECIMAL) {
        const auto& p1 = parent1->getDigitChromosome();
        const auto& p2 = parent2->getDigitChromosome();

        std::vector<int> child1(p1.size()), child2(p2.size());
        for (size_t i = 0; i < p1.size(); ++i) {
            if (dist(_globalGen) < _crossoverGenProb) {
                child1[i] = p1[i];
                child2[i] = p2[i];
            } else {
                child1[i] = p2[i];
                child2[i] = p1[i];
            }
        }

        // CREA A LOS HIJOS Y LOS RETORNA
        auto offspring1 = std::make_shared<Individual>(child1, enc);
        auto offspring2 = std::make_shared<Individual>(child2, enc);
        offspring1->setParents(parent1, parent2);
        offspring2->setParents(parent1, parent2);
        return {offspring1, offspring2};
    }

    throw std::runtime_error("SE DESCONOCE LA CODIFICACION");
}

// ORDERED BASED CROSSOVER
std::mt19937 OrderBasedCrossover::_globalGen = std::mt19937(std::random_device{}());

void OrderBasedCrossover::setSeed(unsigned int seed) { _globalGen.seed(seed); }

OrderBasedCrossover::OrderBasedCrossover(double crossoverProb, double crossoverGenProb)
    : _crossoverProb(crossoverProb), _crossoverGenProb(crossoverGenProb) {
    if (crossoverProb < 0.0 || crossoverProb > 1.0 || crossoverGenProb < 0.0 ||
        crossoverGenProb > 1.0) {
        throw std::invalid_argument("PARAMETROS DE PROBABILIDADES FUERA DEL RANGO [0, 1]");
    }
}

// ORDER BASED CROSSOVER
std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> OrderBasedCrossover::crossover(
    const std::shared_ptr<Individual>& parent1, const std::shared_ptr<Individual>& parent2) const {
    if (parent1->size() != parent2->size()) {
        throw std::invalid_argument("NO COINCIDEN LAS LONGITUDES DE LOS CROMOSOMAS DE LOS PADRES");
    }

    Individual::Encoding enc = parent1->getEncoding();
    if (enc != Individual::Encoding::INTEGER_DECIMAL) {
        throw std::runtime_error("OrderBasedCrossover SOLO FUNCIONA CON REPRESENTACION ENTERA");
    }

    // OBTENCION DE LOS CROMOSOMAS DE LOS PADRES
    const auto& P1 = parent1->getDigitChromosome();
    const auto& P2 = parent2->getDigitChromosome();
    size_t n = P1.size();

    std::uniform_real_distribution<double> distReal(0.0, 1.0);

    // CREA EL VECTOR PARA LOS HIJOS
    std::vector<int> child1(n, -1);
    std::vector<int> child2(n, -1);

    // ESTRUCTURAS PARA SELECCIONAR LOS VALORES
    std::vector<int> seleccionados1;
    std::unordered_set<int> sel1;

    std::vector<int> seleccionados2;
    std::unordered_set<int> sel2;

    // SELECCIONA LOS VALORES CON PROBABILIDAD
    for (size_t i = 0; i < n; ++i) {
        if (distReal(_globalGen) < _crossoverGenProb) {
            seleccionados1.push_back(P1[i]);
            sel1.insert(P1[i]);
        }
    }

    // SI EL VALOR DE LA POSICION i EN EL CROMOSOMA DE P2 NO ESTA EN LOS VALORES SELECCIONADOS
    // AGREGAR VALOR AL HIJO
    for (size_t i = 0; i < n; ++i) {
        if (sel1.find(P2[i]) == sel1.end()) {
            child1[i] = P2[i];
        }
    }

    // INSERTAR LOS VALORES FALTANTES AL CROMOSOMA DE IZQUIERDA A DERECHA
    size_t k1 = 0;
    for (size_t i = 0; i < n && k1 < seleccionados1.size(); ++i) {
        if (child1[i] == -1) {
            child1[i] = seleccionados1[k1++];
        }
    }

    // SELECCIONA LOS VALORES CON PROBABILIDAD
    for (size_t i = 0; i < n; ++i) {
        if (distReal(_globalGen) < _crossoverGenProb) {
            seleccionados2.push_back(P2[i]);
            sel2.insert(P2[i]);
        }
    }
    // SI EL VALOR DE LA POSICION i EN EL CROMOSOMA DE P1 NO ESTA EN LOS VALORES SELECCIONADOS
    // AGREGAR VALOR AL HIJO
    for (size_t i = 0; i < n; ++i) {
        if (sel2.find(P1[i]) == sel2.end()) {
            child2[i] = P1[i];
        }
    }

    // INSERTAR LOS VALORES FALTANTES AL CROMOSOMA DE IZQUIERDA A DERECHA
    size_t k2 = 0;
    for (size_t i = 0; i < n && k2 < seleccionados2.size(); ++i) {
        if (child2[i] == -1) {
            child2[i] = seleccionados2[k2++];
        }
    }

    // CREA LOS HIJOS CON LOS CROMOSOMAS OBTENIDOS
    auto offspring1 = std::make_shared<Individual>(child1, enc);
    auto offspring2 = std::make_shared<Individual>(child2, enc);

    // EMPAREJA A LOS PADRES CON HIJOS
    offspring1->setParents(parent1, parent2);
    offspring2->setParents(parent1, parent2);

    // DEVUELVE A LOS DOS HIJOS
    return {offspring1, offspring2};
}

// PATRON FACTORY DE LA CRUZA
std::unique_ptr<ICrossoverOperator> CrossoverFactory::create(Type type, double crossoverProb,
                                                             double crossoverGenProb) {
    switch (type) {
        case Type::UNIFORM:
            return std::make_unique<UniformCrossover>(crossoverProb, crossoverGenProb);
        case Type::ORDER_BASED:
            return std::make_unique<OrderBasedCrossover>(crossoverProb, crossoverGenProb);
        default:
            throw std::invalid_argument("TIPO DE CRUZA DESCONOCIDO");
    }
}
