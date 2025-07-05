import numpy as np
import matplotlib.pyplot as plt

# Cargar datos desde el CSV
data = np.genfromtxt('CORRIDAS_TAI30/elite_tai_2.csv', delimiter=',', skip_header=1)  # Skip_header omite la cabecera
generaciones = data[:, 0]  # Primera columna (Generacion)
mejor_costo = data[:, 1]  # Segunda columna (Mejor_Fitness)
mejor_fitness = 1 / (1 + mejor_costo)

# Transformar el costo a fitness



# Crear gráfica
plt.plot(generaciones, mejor_fitness, color='b', label='Mejor Fitness')
plt.xlabel('Generación')
plt.ylabel('Mejor Fitness')
plt.title('Convergencia del Algoritmo Genético (TAI 30)')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

# Guardar y mostrar
plt.savefig('elite_tai30.eps', dpi=300, bbox_inches='tight')
plt.show()
