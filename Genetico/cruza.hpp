
#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP

#include <memory>
#include <random>
#include <utility>

#include "individuo.hpp"

// BASE PARA LOS TIPOS DE CRUZA
class ICrossoverOperator {
   public:
    virtual ~ICrossoverOperator() = default;
    virtual std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> crossover(
        const std::shared_ptr<Individual>& parent1,
        const std::shared_ptr<Individual>& parent2) const = 0;
};

// CRUZA UNIFORME
class UniformCrossover : public ICrossoverOperator {
   public:
    explicit UniformCrossover(double crossoverProb = 0.5, double crossoverGenProb = 0.5);

    std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> crossover(
        const std::shared_ptr<Individual>& parent1,
        const std::shared_ptr<Individual>& parent2) const override;

    static void setSeed(unsigned int seed);

   private:
    double _crossoverProb;
    double _crossoverGenProb;
    static std::mt19937 _globalGen;
};

// PATRON FACTORY PARA CRUZA
class CrossoverFactory {
   public:
    enum class Type { UNIFORM };

    static std::unique_ptr<ICrossoverOperator> create(Type type, double crossoverProb = 0.5,
                                                      double crossoverGenProb = 0.5);
};

#endif  // CROSSOVER_HPP
