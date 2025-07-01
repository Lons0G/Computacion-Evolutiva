#!/bin/bash

# Crear carpeta si no existe
mkdir -p CORRIDAS_TAI30

# Número de ejecuciones
NUM_EJECUCIONES=20  # Ajusta a cuántas corridas quieres

# Nombre del archivo de entrada QAP
INPUT_FILE="tai30.dat"

# Configuración fija
POP_SIZE=100
GENERATIONS=1000
CROSS_PROB=0.7
MUT_PROB=0.1
INV_PROB=0.1

# Loop de ejecuciones
for ((i=1; i<=NUM_EJECUCIONES; i++)); do
  # Semilla aleatoria entre 0 y 999
  SEED=$((RANDOM % 1000))

  # Nombre de archivos de salida
  OUTPUT_FILE="CORRIDAS_TAI30/tai30_${i}.txt"
  ELITE_FILE="CORRIDAS_TAI30/elite_tai_${i}.csv"

  echo "Ejecución #$i | Semilla: $SEED"

  # Ejecutar el algoritmo
  ./algoritmo_entero <<EOF
$INPUT_FILE
$POP_SIZE
$CROSS_PROB
$MUT_PROB
$INV_PROB
$GENERATIONS
$SEED
$OUTPUT_FILE
EOF

  # Mover y renombrar el archivo elite generado
  mv elite_log_.csv "$ELITE_FILE"

done

echo "Todas las ejecuciones se completaron. Archivos guardados en CORRIDAS_TAI/"
