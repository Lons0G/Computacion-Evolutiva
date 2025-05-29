import numpy as np
import matplotlib.pyplot as plt

# Cargar datos desde el CSV
data = np.genfromtxt('convergencia.txt', delimiter=',', skip_header=1)  # Skip_header omite la cabecera
generaciones = data[:, 0]  # Primera columna (Generacion)
mejor_fitness = data[:, 1]  # Segunda columna (Mejor_Fitness)

# Crear gráfica
plt.plot(generaciones, mejor_fitness, color='b', label='Mejor Fitness')
plt.xlabel('Generación')
plt.ylabel('Mejor Fitness')
plt.title('Convergencia del Algoritmo Genético')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

# Guardar y mostrar
plt.savefig('convergencia_numpy.eps', dpi=300, bbox_inches='tight')
plt.show()
