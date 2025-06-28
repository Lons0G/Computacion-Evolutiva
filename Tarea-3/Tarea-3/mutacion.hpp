
#ifndef MUTACION_HPP
#define MUTACION_HPP

#include <memory>
#include <random>

#include "individuo.hpp"

// INTERFAZ DE OPERADOR DE MUTACION
class IMutationOperator {
   public:
    virtual ~IMutationOperator() = default;
    virtual void mutate(std::shared_ptr<Individual> individual) const = 0;
};

// MUTACION UNIFORME
class UniformMutation : public IMutationOperator {
   public:
    explicit UniformMutation(double mutationRate);

    void mutate(std::shared_ptr<Individual> individual) const override;

    static void setSeed(unsigned int seed);

   private:
    double _mutationRate;
    static std::mt19937 _globalGen;
};

// FACTORY DE MUTACIONES
class MutationFactory {
   public:
    enum class Type { UNIFORME };

    static std::unique_ptr<IMutationOperator> create(Type type, double mutationRate);
};

#endif  // MUTACION_HPP
