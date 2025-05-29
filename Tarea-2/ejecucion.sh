#!/bin/bash

# Configuración
NUM_CORRIDAS=20
PROGRAMA="./algoritmo"  # Reemplaza con el nombre de tu ejecutable
DIR_SALIDA="Corridas"
PARAM_PM="0.07"
PARAM_PC="0.6"
PARAM_GENERACIONES="150"

# Crear directorio de salida si no existe
mkdir -p "$DIR_SALIDA"

# Función para generar semilla aleatoria con 4 decimales
generar_semilla() {
    printf "0.%04d" $((RANDOM % 10000))
}

# Ejecutar las 20 corridas
for ((i=1; i<=NUM_CORRIDAS; i++)); do
    # Generar semilla
    SEMILLA=$(generar_semilla)
    
    # Nombre del archivo de salida
    ARCHIVO_SALIDA="$DIR_SALIDA/corrida_$i.txt"
    
    # Escribir la semilla como comentario en la primera línea del archivo
    echo "# Semilla utilizada: $SEMILLA" > "$ARCHIVO_SALIDA"
    
    # Ejecutar el programa y redirigir entrada/salida (agregando al archivo, no sobrescribiendo)
    echo "Ejecutando corrida $i con semilla $SEMILLA..."
    echo -e "$PARAM_PM\n$PARAM_PC\n$PARAM_GENERACIONES\n$SEMILLA" | $PROGRAMA >> "$ARCHIVO_SALIDA"
    
    echo "-> Resultados guardados en $ARCHIVO_SALIDA"
done

echo "¡Todas las corridas completadas! Verifica los archivos en $DIR_SALIDA/"
