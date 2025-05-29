#include "individuo.hpp"

#include <random>
#include <stdexcept>

// INICIALIZACION DE VARIABLES ESTATICAS
double Individual::min_val = 0.0;
double Individual::max_val = 0.0;
int Individual::num_vars = 0;
int Individual::bits_var = 0;

// CONSTRUCTOR
Individual::Individual(int cromosomaLength)
    : _cromosoma(cromosomaLength, '0'), _fitness(0.0), _mutado(false) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    for (int i = 0; i < cromosomaLength; ++i) {
        _cromosoma[i] = distrib(gen) ? '1' : '0';
    }
    decode();
}

// CONSTRUCTOR
Individual::Individual(const std::string& cromo) {
    _cromosoma = cromo;
    _fitness = 0.0;
    _mutado = false;
    decode();
}
// OBTENCION DEL CROMOSOMA
const std::string& Individual::getChromosome() const { return _cromosoma; }
// OBTENCION DE LA APTITUD
double Individual::getFitness() const { return _fitness; }
// OBTENCION DE LA(S) VARIABLES DECODIFCADAS
const std::vector<double>& Individual::getVariables() const {
    return _variables;
}

// OBTENCION DE LOS PADRES DEL INDIVIDUO
std::shared_ptr<Individual> Individual::getParent1() const { return _padre_1; }
std::shared_ptr<Individual> Individual::getParent2() const { return _padre_2; }

bool Individual::isMutated() const { return _mutado; }

// ACTUALIZACION DEL CROMOSOMA
void Individual::setChromosome(const std::string& chromo) {
    _cromosoma = chromo;
}

// ACTUALIZACION DE LA APTITUD
void Individual::setFitness(double fit) { _fitness = fit; }

// ACTUALIZACION DE LA(S) VARIABLES DECODIFICADAS
void Individual::setVariables(const std::vector<double>& vars) {
    _variables = vars;
}

// ACTUALIZACION DE LOS PADRES DEL INDIVIDUO
void Individual::setParents(std::shared_ptr<Individual> p1,
                            std::shared_ptr<Individual> p2) {
    _padre_1 = p1;
    _padre_2 = p2;
}

// ACTUALIZACION DEL ESTADO DE MUTACION
void Individual::setMutated(bool mut) { _mutado = mut; }

// METODOS UTILES
int Individual::size() const { return _cromosoma.length(); }

// INVERTIR BIT
void Individual::flipBit(int posicion) {
    if (posicion < 0 || posicion >= _cromosoma.length()) {
        throw std::out_of_range("Posición de bit inválida");
    }
    //_cromosoma[posicion] = (_cromosoma[posicion] == '0') ? '1' : '0';
    if (_cromosoma[posicion] == '0') {
        _cromosoma[posicion] = '1';
    } else {
        _cromosoma[posicion] = '0';
    }
}
//  BIT EN UNA POSICION ESPECIFICA
bool Individual::getBit(int posicion) const {
    if (posicion < 0 || posicion >= _cromosoma.length()) {
        throw std::out_of_range("Posición de bit inválida");
    }
    return _cromosoma[posicion] == '1';
}

// DECODIFICACION
void Individual::setDecoding(double min, double max, int vars, int bits) {
    min_val = min;
    max_val = max;
    num_vars = vars;
    bits_var = bits;
}

// Decodificación simplificada
void Individual::decode() {
    if (bits_var == 0 || num_vars == 0) {
        throw std::runtime_error(
            "Parámetros de decodificación no configurados");
    }

    int length = num_vars * bits_var;
    if (_cromosoma.length() < length) {
        throw std::runtime_error("Longitud de cromosoma insuficiente");
    }

    _variables.clear();
    _variables.reserve(num_vars);

    for (int i = 0; i < num_vars; ++i) {
        int startPos = i * bits_var;
        std::string varBits = _cromosoma.substr(startPos, bits_var);

        // CONVERSION DE CADENA A VALORES CON LA ECUCACION DE LA RECTA
        unsigned long intValue = std::stoul(varBits, nullptr, 2);
        double value = min_val + (max_val - min_val) * intValue /
                                     (std::pow(2, bits_var) - 1);

        _variables.push_back(value);
    }
}
