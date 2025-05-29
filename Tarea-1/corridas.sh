#!/bin/bash

# Nombre del ejecutable de tu programa
PROGRAMA="./algoritmo 100"  # Cambia esto

# Directorio para guardar las corridas
DIR_RESULTADOS="resultados_corridas"
mkdir -p "$DIR_RESULTADOS"

# Archivo temporal que genera tu programa (ajusta el nombre)
ARCHIVO_TMP="resultados.txt"  # Cambia si tu programa usa otro nombre

# Ejecutar 20 corridas
for i in {1..20}; do
    echo "Ejecutando corrida $i..."
    
    # 1. Ejecutar el programa (genera ARCHIVO_TMP)
    $PROGRAMA
    
    # 2. Copiar el archivo generado a corrida_n.txt
    cp "$ARCHIVO_TMP" "$DIR_RESULTADOS/corrida_$i.txt"
    
    # 3. Opcional: Borrar el temporal (si no lo necesitas)
    rm "$ARCHIVO_TMP"
    
    echo "Resultados copiados a $DIR_RESULTADOS/corrida_$i.txt"
	sleep 1.5
done

echo "¡Todas las corridas completadas! Ver resultados en $DIR_RESULTADOS/"
