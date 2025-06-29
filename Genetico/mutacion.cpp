#include "mutacion.hpp"

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

// MUTACION POR INSERCION
std::mt19937 InsertionMutation::_globalGen = std::mt19937(std::random_device{}());

void InsertionMutation::setSeed(unsigned int seed) { _globalGen.seed(seed); }

InsertionMutation::InsertionMutation(double mutationRate) : _mutationRate(mutationRate) {
    if (_mutationRate < 0.0 || _mutationRate > 1.0) {
        throw std::invalid_argument("LA TASA DE MUTACION DEBE DE ESTAR EN EL RANGO [0, 1]");
    }
}

void InsertionMutation::mutate(std::shared_ptr<Individual> individual) const {
    std::uniform_real_distribution<double> probDist(0.0, 1.0);

    // SI MUTATIONRATE < PROBDIST -> NO MUTAR
    if (probDist(_globalGen) >= _mutationRate) {
        return;
    }

    // OBTENER CODIFICACION DEL INDIVIDUO
    auto encoding = individual->getEncoding();
    // VERIFICAR SI LA CODIFICACION ES ENTERA
    if (encoding != Individual::Encoding::INTEGER_DECIMAL) {
        throw std::runtime_error("InsertionMutation SOLO ES VALIDA PARA REPRESENTACION ENTERA");
    }

    // OBTENER CROMOSOMA DEL INDIVIDUO
    auto chromo = individual->getDigitChromosome();
    // LONGUITUD DEL CROMOSOMA
    int n = static_cast<int>(chromo.size());

    // SI SOLO HAY 1 CROMOSOMA, NO HACER NADA
    if (n < 2)
        return;

    std::uniform_int_distribution<int> distPos(0, n - 1);

    // GENERAR POSICION ALEATORIA
    int pos_valor = distPos(_globalGen);
    int pos_insercion = distPos(_globalGen);

    // SI LA POSICION DE INSERCION ES LA MISMA QUE POSICION DEL VALOR, VOLVER A GENERAR UN NUMERO
    // ALEATORIO
    while (pos_insercion == pos_valor) {
        pos_insercion = distPos(_globalGen);
    }

    // OBTENER EL VALOR DE LA POSICION ALEATORIA
    int val = chromo[pos_valor];

    // ELIMINA E INSERTA LA POSICION EL VALOR SELECCIONADO
    // DESPLAZA LOS VALORES POSTERIORES DE LA POSICION DE INSERCION
    chromo.erase(chromo.begin() + pos_valor);
    chromo.insert(chromo.begin() + pos_insercion, val);

    individual->setDigitChromosome(chromo);
    individual->decode();
}

// FACTORY
std::unique_ptr<IMutationOperator> MutationFactory::create(Type type, double mutationRate) {
    switch (type) {
        case Type::UNIFORME:
            return std::make_unique<UniformMutation>(mutationRate);
        case Type::INSERCION:
            return std::make_unique<InsertionMutation>(mutationRate);
        default:
            throw std::invalid_argument("TIPO DE MUTACION DESCONOCIDO");
    }
}
