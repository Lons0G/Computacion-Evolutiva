#include "poblacion.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

// CONSTRUCTOR
Population::Population(int size, int chromosomeLength, Individual::Encoding encoding)
    : _encoding(encoding) {
    _individuos.reserve(size);

    for (int i = 0; i < size; ++i) {
        _individuos.push_back(std::make_shared<Individual>(chromosomeLength, encoding));
    }
}

// OBTENER INDIVIDUO
std::shared_ptr<Individual> Population::getIndividual(int index) const {
    return _individuos.at(index);
}

// OBTENER INDIVIDUOS
const std::vector<std::shared_ptr<Individual>>& Population::getIndividuals() const {
    return _individuos;
}

// OBTENER EL MEJOR FITNESS
std::shared_ptr<Individual> Population::getFittest() const {
    return *std::max_element(
        _individuos.begin(), _individuos.end(),
        [](const std::shared_ptr<Individual>& a, const std::shared_ptr<Individual>& b) {
            return a->getFitness() < b->getFitness();
        });
}

// ACTUALIZACION
void Population::setIndividual(int index, std::shared_ptr<Individual> indiv) {
    _individuos.at(index) = indiv;
}

// METODOS AUXILIARES
int Population::size() const { return _individuos.size(); }

void Population::addIndividual(std::shared_ptr<Individual> indiv) { _individuos.push_back(indiv); }

void Population::sortByFitness() {
    std::sort(_individuos.begin(), _individuos.end(),
              [](const std::shared_ptr<Individual>& a, const std::shared_ptr<Individual>& b) {
                  return a->getFitness() > b->getFitness();
              });
}

// ESTADISTICAS
double Population::getAverageFitness() const {
    if (_individuos.empty())
        return 0.0;

    double sum = std::accumulate(_individuos.begin(), _individuos.end(), 0.0,
                                 [](double acc, const std::shared_ptr<Individual>& indiv) {
                                     return acc + indiv->getFitness();
                                 });

    return sum / _individuos.size();
}

double Population::getBestFitness() const {
    if (_individuos.empty())
        return 0.0;
    return getFittest()->getFitness();
}

double Population::getWorstFitness() const {
    if (_individuos.empty())
        return 0.0;

    auto minIt = std::min_element(
        _individuos.begin(), _individuos.end(),
        [](const std::shared_ptr<Individual>& a, const std::shared_ptr<Individual>& b) {
            return a->getFitness() < b->getFitness();
        });

    return (*minIt)->getFitness();
}

// REEMPLAZAR AL PEOR INDIVIDUO CON EL ÉLITE
void Population::Replace(std::shared_ptr<Individual> elite) {
    if (_individuos.empty())
        return;

    auto minIt = std::min_element(
        _individuos.begin(), _individuos.end(),
        [](const std::shared_ptr<Individual>& a, const std::shared_ptr<Individual>& b) {
            return a->getFitness() < b->getFitness();
        });

    *minIt = elite;
}
// PRINT
void Population::printPopulation(const std::string& title) const {
    std::cout << "\n=== " << title << " (" << _individuos.size() << " individuos) ===\n";
    std::cout << "Mejor fitness: " << getBestFitness() << "\n";
    std::cout << "Peor fitness: " << getWorstFitness() << "\n";
    std::cout << "Fitness promedio: " << getAverageFitness() << "\n";

    std::cout << "\nIndividuos (primeros 5):\n";
    int limit = std::min(5, static_cast<int>(_individuos.size()));
    for (int i = 0; i < limit; ++i) {
        _individuos[i]->printIndividual();
    }
}
