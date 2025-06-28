#include "cruza.hpp"

#include <memory>
#include <stdexcept>

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

        // if (enc == Individual::Encoding::GRAY) {
        //     p1 = Individual::GrayToBin(p1);
        //     p2 = Individual::GrayToBin(p2);
        // }

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

        // if (enc == Individual::Encoding::GRAY) {
        //     child1 = Individual::BinToGray(child1);
        //     child2 = Individual::BinToGray(child2);
        // }

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

// PATRON FACTORY DE LA CRUZA
std::unique_ptr<ICrossoverOperator> CrossoverFactory::create(Type type, double crossoverProb,
                                                             double crossoverGenProb) {
    switch (type) {
        case Type::UNIFORM:
            return std::make_unique<UniformCrossover>(crossoverProb, crossoverGenProb);
        default:
            throw std::invalid_argument("TIPO DE CRUZA DESCONOCIDO");
    }
}
