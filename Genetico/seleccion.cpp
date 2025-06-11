#include "seleccion.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>

// IMPLEMENTACION DE RULETA
std::shared_ptr<Individual> RouletteWheelSelection::select(const Population& population) const {
    if (population.size() == 0) {
        throw std::runtime_error("Cannot select from empty population");
    }

    double totalFitness = calculateTotalFitness(population);

    if (totalFitness <= 0.0) {
        std::uniform_int_distribution<size_t> dist(0, population.size() - 1);
        return population.getIndividual(dist(_gen));
    }

    std::uniform_real_distribution<double> dist(0.0, totalFitness);
    double spin = dist(_gen);

    double cumulativeFitness = 0.0;
    for (size_t i = 0; i < population.size(); ++i) {
        cumulativeFitness += population.getIndividual(i)->getFitness();
        if (cumulativeFitness >= spin) {
            return population.getIndividual(i);
        }
    }

    return population.getIndividual(population.size() - 1);
}
// CALCULAR VALOR ESPERADO
double RouletteWheelSelection::calculateTotalFitness(const Population& population) const {
    return std::accumulate(
        population.getIndividuals().begin(), population.getIndividuals().end(), 0.0,
        [](double sum, const std::shared_ptr<Individual>& ind) { return sum + ind->getFitness(); });
}

// IMPLEMENTACION DE TORNEO
TournamentSelection::TournamentSelection(int tournamentSize) : _tournamentSize(tournamentSize) {
    if (tournamentSize < 1) {
        throw std::invalid_argument("Tournament size must be at least 1");
    }
}

std::shared_ptr<Individual> TournamentSelection::select(const Population& population) const {
    if (population.size() == 0) {
        throw std::runtime_error("Cannot select from empty population");
    }

    std::vector<std::shared_ptr<Individual>> tournament;
    std::sample(population.getIndividuals().begin(), population.getIndividuals().end(),
                std::back_inserter(tournament), _tournamentSize, _gen);

    return *std::max_element(
        tournament.begin(), tournament.end(),
        [](const auto& a, const auto& b) { return a->getFitness() < b->getFitness(); });
}

// IMPLEMENTACION UNIVERSAL ESTOCASTICA
std::vector<std::shared_ptr<Individual>> Universal_Estocastica::selectMany(
    const Population& population, int numToSelect) const {
    if (population.size() == 0 || numToSelect <= 0) {
        throw std::runtime_error("ERROR DE TAMANIO DE LA POBLACION");
    }

    double avgFitness = population.getAverageFitness();

    // CALCULAR VALORES ESPERADOS
    std::vector<double> expectedValues;
    for (const auto& ind : population.getIndividuals()) {
        expectedValues.push_back(ind->getFitness() / avgFitness);
    }

    std::vector<std::shared_ptr<Individual>> selected;

    double ptr = std::uniform_real_distribution<>(0.0, 1.0)(_gen);
    double cumulativeSum = 0.0;
    size_t i = 0;

    // SELECCIONAR N INDIVIDUOS
    while (selected.size() < static_cast<size_t>(numToSelect)) {
        if (i >= expectedValues.size())
            break;
        cumulativeSum += expectedValues[i];
        // SI PTR ES MENOR Y LA SELECCION DE INDIVIDUOS ES MENOR A LA CANTIDAD NECESARIA
        // SELECCIONAR(I)
        while (ptr < cumulativeSum && selected.size() < static_cast<size_t>(numToSelect)) {
            selected.push_back(population.getIndividual(i));
            ptr += 1.0;
        }
        ++i;
    }

    // RETORNAR VECTORES CON LOS INDICES DE LOS SELECCIONADOS
    return selected;
}

// SELECCION DE ESTRATEGIA
std::unique_ptr<ISelectionStrategy> SelectionFactory::create(Type type, int tournamentSize) {
    switch (type) {
        case Type::RULETA:
            return std::make_unique<RouletteWheelSelection>();
        case Type::TORNEO:
            return std::make_unique<TournamentSelection>(tournamentSize);
        default:
            throw std::invalid_argument("TIPO DE SELECCION DESCONOCIDA");
    }
}
