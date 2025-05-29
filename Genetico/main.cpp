#include <iostream>
#include <vector>

#include "individuo.hpp"

void printIndividual(const Individual& ind, const std::string& name) {
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Cromosoma: " << ind.getChromosome() << "\n";
    std::cout << "Fitness: " << ind.getFitness() << "\n";
    std::cout << "Mutado: " << (ind.isMutated() ? "Sí" : "No") << "\n";

    const auto& vars = ind.getVariables();
    std::cout << "Variables decodificadas:\n";
    for (size_t i = 0; i < vars.size(); ++i) {
        std::cout << "x" << i << " = " << vars[i] << "\n";
    }
}

void testBasicFunctionality() {
    std::cout << "\n***** TEST FUNCIONALIDAD BÁSICA *****\n";

    // Configuración de decodificación (3 variables entre -5 y 5, 8 bits cada
    // una)
    Individual::setDecoding(-5.0, 5.0, 3, 8);

    // Crear individuo con cromosoma específico
    Individual ind1("001011011101100001100010");
    ind1.setFitness(0.85);
    ind1.setMutated(true);

    printIndividual(ind1, "Individuo 1");

    // Verificar longitud
    std::cout << "\nLongitud cromosoma: " << ind1.size() << " bits\n";
    std::cout << "Bit en posición 5: " << ind1.getBit(5) << "\n";

    // Modificar bits
    std::cout << "\nAntes de flipBit(3): " << ind1.getChromosome();
    ind1.flipBit(3);
    std::cout << "\nDespués de flipBit(3): " << ind1.getChromosome() << "\n";
}

void testParentRelationship() {
    std::cout << "\n***** TEST RELACIONES PADRE-HIJO *****\n";

    // Configuración
    Individual::setDecoding(-10.0, 10.0, 2, 10);

    // Crear padres
    auto parent1 = std::make_shared<Individual>("11110000001111000000");
    auto parent2 = std::make_shared<Individual>("00001111110000111111");

    parent1->setFitness(0.9);
    parent2->setFitness(0.8);

    // Crear hijo
    auto child = std::make_shared<Individual>("11001100110011001100");
    child->setParents(parent1, parent2);
    child->setFitness(0.95);
    child->setMutated(false);

    printIndividual(*parent1, "Padre 1");
    printIndividual(*parent2, "Padre 2");
    printIndividual(*child, "Hijo");

    // Verificar relaciones
    std::cout << "\nPadre1 del hijo: " << child->getParent1()->getChromosome()
              << "\n";
    std::cout << "Padre2 del hijo: " << child->getParent2()->getChromosome()
              << "\n";
}

void testRandomGeneration() {
    std::cout << "\n***** TEST GENERACIÓN ALEATORIA *****\n";

    // Configuración (2 variables entre 0 y 100, 16 bits cada una)
    Individual::setDecoding(0.0, 100.0, 2, 16);

    // Crear individuo aleatorio
    Individual ind(32);  // 2 variables * 16 bits

    printIndividual(ind, "Individuo Aleatorio");

    // Verificar valores decodificados están en rango
    const auto& vars = ind.getVariables();
    for (size_t i = 0; i < vars.size(); ++i) {
        std::cout << "\nVariable " << i << " en rango [0, 100]? ";
        std::cout << (vars[i] >= 0.0 && vars[i] <= 100.0 ? "Sí" : "No");
        std::cout << " (" << vars[i] << ")";
    }
}

void testEdgeCases() {
    std::cout << "\n\n***** TEST CASOS LÍMITE *****\n";

    try {
        // Cromosoma vacío
        Individual ind("");
        printIndividual(ind, "Cromosoma Vacío");
    } catch (const std::exception& e) {
        std::cout << "Error esperado: " << e.what() << "\n";
    }

    // Configuración sin inicializar
    Individual::setDecoding(0, 0, 0, 0);  // Reset
    try {
        Individual ind("01");
        ind.decode();  // Debe fallar
    } catch (const std::exception& e) {
        std::cout << "Error esperado (parámetros no configurados): " << e.what()
                  << "\n";
    }

    // Configuración válida
    Individual::setDecoding(-1.0, 1.0, 1, 2);
    Individual ind("11");  // Valor máximo
    printIndividual(ind, "Valor Máximo (1.0)");
}

int main() {
    std::cout << "=== PRUEBAS UNITARIAS PARA LA CLASE INDIVIDUAL ===\n";

    testBasicFunctionality();
    testParentRelationship();
    testRandomGeneration();
    testEdgeCases();

    std::cout << "\n\n=== TODAS LAS PRUEBAS COMPLETADAS ===" << std::endl;
    return 0;
}
