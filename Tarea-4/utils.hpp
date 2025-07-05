#ifndef UTILS_HPP
#define UTILS_HPP

#include <memory>
#include <vector>

#include "individuo.hpp"
#include "mutacion.hpp"

namespace Utils {

// REMOVER DUPLICADOS.
void removeDuplicates(std::vector<std::shared_ptr<Individual>>& population,
                      IMutationOperator& mutator);
// POLITICA DE REMPLAZO
enum class InversionPolicy { ALWAYS_REPLACE, ONLY_IF_BETTER, RANDOM_CHOICE };

// FUNCION DE INVERSION
void applyInversion(std::shared_ptr<Individual> individual, double inversionRate, int value1,
                    int value2, InversionPolicy policy);

}  // namespace Utils
#endif
