#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include <memory>  // Para shared_ptr
#include <string>
#include <vector>

class Individual {
   private:
    // CADENA DE BITS
    std::string _cromosoma;
    // APTITUD
    double _fitness;
    // VECTOR CON LAS VARIBLES DECODIFICADAS
    std::vector<double> _variables;
    // PUNTEROS INTELIGENTES DE LOS PADRES
    std::shared_ptr<Individual> _padre_1;
    std::shared_ptr<Individual> _padre_2;
    // SI EL INDIVIDUO FUE MUTADO
    bool _mutado;

    // PARAMETROS DE DECODIFICACION
    static double min_val;
    static double max_val;
    static int num_vars;
    static int bits_var;

   public:
    // CONSTRUCTOR
    Individual(int cromosomaLength);
    Individual(const std::string& cromo);

    // OBTENCION DE ATRIBUTOS(GETTERS)
    const std::string& getChromosome() const;
    double getFitness() const;
    const std::vector<double>& getVariables() const;
    std::shared_ptr<Individual> getParent1() const;
    std::shared_ptr<Individual> getParent2() const;
    bool isMutated() const;

    // ACTUALIZACION DE ATRIBUTOS (SETTERS)
    void setChromosome(const std::string& cromo);
    void setFitness(double fit);
    void setVariables(const std::vector<double>& vars);
    void setParents(std::shared_ptr<Individual> p1,
                    std::shared_ptr<Individual> p2);
    void setMutated(bool mut);

    // METODOS AUXILIARES
    int size() const;
    void flipBit(int posicion);
    bool getBit(int posicion) const;
    void setBit(int position, bool valor);

    void decode();
    static void setDecoding(double min, double max, int vars, int bits);
};

#endif  // INDIVIDUAL_HPP
