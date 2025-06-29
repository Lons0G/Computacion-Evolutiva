
# -------------------------------
# Variables generales
# -------------------------------
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Todos los módulos comunes que NO tienen main()
SRCS = individuo.cpp cruza.cpp mutacion.cpp seleccion.cpp poblacion.cpp utils.cpp

# -------------------------------
# Targets
# -------------------------------

# Ejecutable para main_entero.cpp
entero: main_entero.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o algoritmo_entero

# Ejecutable para main_gray.cpp
gray: main_gray.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o algoritmo_gray

# Limpieza
clean:
	rm -f algoritmo_entero algoritmo_gray

# -------------------------------
# Opcional: por defecto compila ambos
# -------------------------------
all: entero gray

