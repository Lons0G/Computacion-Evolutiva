#include "mutacion.hpp"

#include <algorithm>
#include <stdexcept>

std::mt19937 UniformMutation::_globalGen = std::mt19937(std::random_device{}());
// ASIGNACION DE SEMILLA
void UniformMutation::setSeed(unsigned int seed) { _globalGen.seed(seed); }

// CONSTRUCTOR
UniformMutation::UniformMutation(double mutationRate) : _mutationRate(mutationRate) {
    if (_mutationRate < 0.0 || _mutationRate > 1.0) {
        throw std::invalid_argument("LA TASA DE MUTACION DEBE DE ESTAR EN EL RANGO [0, 1]");
    }
}

// OPERADOR DE MUTACION
void UniformMutation::mutate(std::shared_ptr<Individual> individual) const {
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    auto encoding = individual->getEncoding();

    switch (encoding) {
        case Individual::Encoding::BINARY:
        case Individual::Encoding::GRAY: {
            std::string chromo = individual->getChromosome();
            // HACE LA INVERSION DE UN BIT (0 => 1) O (1 => 0)
            for (char& bit : chromo) {
                if (probDist(_globalGen) < _mutationRate) {
                    bit = (bit == '0') ? '1' : '0';
                }
            }
            individual->setChromosome(chromo);
            break;
        }

        // MUTACION PARA REPRESENTACION ENTERA
        case Individual::Encoding::INTEGER_DECIMAL: {
            auto digits = individual->getDigitChromosome();
            std::uniform_int_distribution<int> digitDist(0, 9);
            // GENERA UN DIGITO ALEATORIO EN EL RANGO [0, 9]
            for (int& digit : digits) {
                if (probDist(_globalGen) < _mutationRate) {
                    digit = digitDist(_globalGen);
                }
            }
            individual->setDigitChromosome(digits);
            break;
        }
    }

    individual->decode();
}

// FACTORY
std::unique_ptr<IMutationOperator> MutationFactory::create(Type type, double mutationRate) {
    switch (type) {
        case Type::UNIFORME:
            return std::make_unique<UniformMutation>(mutationRate);
        default:
            throw std::invalid_argument("TIPO DE MUTACION DESCONOCIDO");
    }
}
