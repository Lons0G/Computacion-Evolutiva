#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <memory>
#include <vector>

#include "individuo.hpp"

class Population {
   private:
    std::vector<std::shared_ptr<Individual>> _individuos;
    Individual::Encoding _encoding;

   public:
    // CONSTRUCTORES
    Population() = default;
    Population(int size, int chromosomeLength, Individual::Encoding encoding);

    // OBTENCION
    std::shared_ptr<Individual> getIndividual(int index) const;
    const std::vector<std::shared_ptr<Individual>>& getIndividuals() const;
    std::vector<std::shared_ptr<Individual>>& getIndividuals();
    std::shared_ptr<Individual> getFittest() const;

    // ACTUALIZACION
    void setIndividual(int index, std::shared_ptr<Individual> indiv);

    // METODOS AUXILIARES
    int size() const;
    void addIndividual(std::shared_ptr<Individual> indiv);
    void sortByFitness();
    void Replace(std::shared_ptr<Individual> elite);

    // ESTADISTICAS
    double getAverageFitness() const;
    double getBestFitness() const;
    double getWorstFitness() const;

    // PRINT
    void printPopulation(const std::string& title = "Población") const;
};

#endif
