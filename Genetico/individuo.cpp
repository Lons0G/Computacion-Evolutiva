#include "individuo.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <sstream>

// INICIALIZACION DE VARIABLES ESTATICAS
double Individual::min_val = 0.0;
double Individual::max_val = 0.0;
int Individual::num_vars = 0;
int Individual::bits_var = 0;
Individual::Encoding Individual::global_encoding = Individual::Encoding::BINARY;
std::mt19937 Individual::_globalGen = std::mt19937(std::random_device{}());

// INICIALIZAR SEMILLA
void Individual::setSeed(unsigned int seed) { _globalGen.seed(seed); }

// CONSTRUCTORES
Individual::Individual(int length, Encoding enc) : _encoding(enc) {
    switch (_encoding) {
        // CASO BINARIO
        case Encoding::BINARY:
        case Encoding::GRAY:
            _cromosoma = std::string(length, '0');
            for (int i = 0; i < length; ++i) {
                _cromosoma[i] = (std::uniform_int_distribution<>(0, 1)(_globalGen)) ? '1' : '0';
            }
            if (_encoding == Encoding::GRAY) {
                _cromosoma = BinToGray(_cromosoma);
            }
            break;

            // CASO ENTERO
        case Encoding::INTEGER_DECIMAL: {
            _digitChromosome.resize(length);
            for (int i = 0; i < length; ++i) {
                _digitChromosome[i] = i;
            }
            std::shuffle(_digitChromosome.begin(), _digitChromosome.end(), _globalGen);
            break;
        }
    }

    decode();
}

Individual::Individual(const std::string& cromo, Encoding enc) : _cromosoma(cromo), _encoding(enc) {
    decode();
}

Individual::Individual(const std::vector<int>& digits, Encoding enc)
    : _digitChromosome(digits), _encoding(enc) {
    decode();
}

// OBTENCION
const std::string& Individual::getChromosome() const { return _cromosoma; }
const std::vector<int>& Individual::getDigitChromosome() const { return _digitChromosome; }
double Individual::getFitness() const { return _fitness; }
const std::vector<double>& Individual::getVariables() const { return _variables; }
Individual::Encoding Individual::getEncoding() const { return _encoding; }

std::shared_ptr<Individual> Individual::getParent1() const { return _padre1; }
std::shared_ptr<Individual> Individual::getParent2() const { return _padre2; }
bool Individual::isMutated() const { return _mutated; }

// ACTUALIZACION
void Individual::setChromosome(const std::string& chromo) { _cromosoma = chromo; }
void Individual::setDigitChromosome(const std::vector<int>& digits) { _digitChromosome = digits; }
void Individual::setFitness(double fit) { _fitness = fit; }
void Individual::setVariables(const std::vector<double>& vars) { _variables = vars; }
void Individual::setParents(std::shared_ptr<Individual> p1, std::shared_ptr<Individual> p2) {
    _padre1 = p1;
    _padre2 = p2;
}
void Individual::setMutated(bool mut) { _mutated = mut; }

// DECODIFICACION
void Individual::decode() {
    _variables.clear();
    _variables.reserve(num_vars);

    switch (_encoding) {
        case Encoding::BINARY:
        case Encoding::GRAY: {
            std::string cromosoma = _cromosoma;
            // CONVIERTE DE GRAY A DECIMAL
            if (_encoding == Encoding::GRAY) {
                cromosoma = GrayToBin(_cromosoma);
            }
            // CONVIERTE DE BINARIO A REAL
            for (int i = 0; i < num_vars; ++i) {
                int start = i * bits_var;
                std::string segment = cromosoma.substr(start, bits_var);
                unsigned long intValue = std::stoul(segment, nullptr, 2);
                double value =
                    min_val + (max_val - min_val) * intValue / (std::pow(2, bits_var) - 1);
                _variables.push_back(value);
            }
            break;
        }

        // REPRESENTACION ENTERA NO SE CODIFICA
        case Encoding::INTEGER_DECIMAL: {
            _variables.clear();
            for (int val : _digitChromosome) {
                _variables.push_back(static_cast<double>(val));
            }
            break;
        }
    }
}

// REPRESENTACION
void Individual::setDecoding(double min, double max, int vars, int bits, Encoding encoding) {
    min_val = min;
    max_val = max;
    num_vars = vars;
    bits_var = bits;
    global_encoding = encoding;
}

// CODIGO GRAY
std::string Individual::BinToGray(const std::string& bin) {
    std::string gray;
    gray += bin[0];
    for (size_t i = 1; i < bin.size(); ++i) {
        gray += (bin[i - 1] != bin[i]) ? '1' : '0';
    }
    return gray;
}

std::string Individual::GrayToBin(const std::string& gray) {
    std::string bin;
    bin += gray[0];
    for (size_t i = 1; i < gray.size(); ++i) {
        bin += (gray[i] != bin[i - 1]) ? '1' : '0';
    }
    return bin;
}

// METODOS AUXILIARES
int Individual::size() const {
    switch (_encoding) {
        case Encoding::BINARY:
        case Encoding::GRAY:
            return static_cast<int>(_cromosoma.size());
        case Encoding::INTEGER_DECIMAL:
            return static_cast<int>(_digitChromosome.size());
    }
    return 0;
}

void Individual::printIndividual() const {
    std::cout << "Cromosoma: ";
    if (_encoding == Encoding::INTEGER_DECIMAL) {
        for (int digit : _digitChromosome) {
            std::cout << digit;
        }
    } else {
        std::cout << _cromosoma;
    }
    std::cout << "\nFitness: " << _fitness << "\nVariables: ";
    for (double var : _variables) {
        std::cout << var << " ";
    }
    std::cout << std::endl;
}
