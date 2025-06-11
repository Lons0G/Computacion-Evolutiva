#ifndef INDIVIDUO_HPP
#define INDIVIDUO_HPP

#include <memory>
#include <random>
#include <string>
#include <vector>

class Individual {
   public:
    enum class Encoding { BINARY, GRAY, INTEGER_DECIMAL };

    // CONSTRUCTORES
    Individual(int length, Encoding enc);
    Individual(const std::string& cromo, Encoding enc);
    Individual(const std::vector<int>& digits, Encoding enc);

    // METODOS ESTATICOS
    static void setSeed(unsigned int seed);
    static void setDecoding(double min, double max, int vars, int bits, Encoding enc);

    // OBTENCION
    const std::string& getChromosome() const;
    const std::vector<int>& getDigitChromosome() const;
    double getFitness() const;
    const std::vector<double>& getVariables() const;
    Encoding getEncoding() const;
    std::shared_ptr<Individual> getParent1() const;
    std::shared_ptr<Individual> getParent2() const;
    bool isMutated() const;
    int size() const;

    // ACTUALIZACION
    void setChromosome(const std::string& chromo);
    void setDigitChromosome(const std::vector<int>& digits);
    void setFitness(double fit);
    void setVariables(const std::vector<double>& vars);
    void setParents(std::shared_ptr<Individual> p1, std::shared_ptr<Individual> p2);
    void setMutated(bool mut);

    // CODIFICACION / DECODIFICACION
    void decode();
    static std::string BinToGray(const std::string& bin);
    static std::string GrayToBin(const std::string& gray);

    void printIndividual() const;

   private:
    std::string _cromosoma;
    std::vector<int> _digitChromosome;
    std::vector<double> _variables;
    double _fitness = 0.0;
    bool _mutated = false;
    Encoding _encoding;

    std::shared_ptr<Individual> _padre1;
    std::shared_ptr<Individual> _padre2;

    // VARIABLES ESTATICAS
    static double min_val, max_val;
    static int num_vars, bits_var;
    static Encoding global_encoding;
    static std::mt19937 _globalGen;
};

#endif
