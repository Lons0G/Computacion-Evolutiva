#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <memory>
#include <random>

#include "poblacion.hpp"

// Interfaz base para estrategias de selección
class ISelectionStrategy {
   public:
    virtual ~ISelectionStrategy() = default;
    virtual std::shared_ptr<Individual> select(const Population& population) const = 0;
};

// Implementación concreta: Selección por Ruleta
class Ruleta : public ISelectionStrategy {
   public:
    std::shared_ptr<Individual> select(const Population& population) const override;

   private:
    mutable std::mt19937 _gen{std::random_device{}()};
    double calculateTotalFitness(const Population& population) const;
};

// Implementación concreta: Selección por Torneo
class Torneo : public ISelectionStrategy {
   public:
    explicit Torneo(int tournamentSize);
    std::shared_ptr<Individual> select(const Population& population) const override;

   private:
    int _tournamentSize;
    mutable std::mt19937 _gen{std::random_device{}()};
};

// Implementación concreta: Selección Universal Estocástica
class Universal_Estocastica {
   public:
    std::vector<std::shared_ptr<Individual>> selectMany(const Population& population,
                                                        int numToSelect) const;

   private:
    mutable std::mt19937 _gen{std::random_device{}()};
};

class Torneo_Binario_Deterministico {
   public:
    std::vector<std::shared_ptr<Individual>> selectMany(const Population& population,
                                                        int numToSelect) const;

   private:
    mutable std::mt19937 _gen{std::random_device{}()};
};

// Factory para crear estrategias de selección
class SelectionFactory {
   public:
    enum class Type { RULETA, TORNEO, UNIVERSAL_ESTOCASTICA, TORNEO_BINARIO_DETERMINISTICO };

    static std::unique_ptr<ISelectionStrategy> create(Type type, int tournamentSize = 3);
};

#endif
