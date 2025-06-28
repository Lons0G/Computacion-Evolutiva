#!/bin/bash

# Crear carpeta si no existe
mkdir -p CORRIDAS_ENTERA 

# Número de generaciones fijas
GENS=100

for i in {1..20}
do
    echo "Ejecutando corrida $i"

    # Semilla aleatoria entre 1 y 999
    SEED=$((1 + RANDOM % 999))

    # Archivos de salida
    LOG_FILE="corrida_entera_${i}.csv"
    ELITE_FILE="elite_entera_${i}.csv"

    # Ejecutar el programa y redirigir entrada interactiva simulada
    ./algoritmo <<EOF
10
0.5
0.1
$GENS
$SEED
$LOG_FILE
EOF

    # Mover archivos generados a la carpeta
    mv "$LOG_FILE" "CORRIDAS_ENTERA/$LOG_FILE"
    mv "elite_log_real.csv" "CORRIDAS_ENTERA/$ELITE_FILE"
done

echo "Todas las corridas fueron guardadas en la carpeta CORRIDAS_ENTERA"
