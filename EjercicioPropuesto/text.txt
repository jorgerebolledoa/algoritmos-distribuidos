
### 1. Carpeta: `lab1`

Esta carpeta contiene un algoritmo que determina si un grafo es **fuertemente conexo** utilizando programación paralela y memoria compartida.

- **Archivo principal:** `grafo_fuertemente_conexo.c`
  - Implementa la comprobación de conectividad fuerte dividiendo las tareas entre múltiples hilos.
  - Utiliza estructuras de memoria compartida para optimizar el rendimiento.

**Objetivo:** Mostrar cómo resolver problemas de teoría de grafos utilizando paralelismo y sincronización eficiente.

### 2. Carpeta: `lectura-concurrente`

Esta carpeta contiene un ejercicio propuesto que implementa un código para leer el mismo archivo de forma concurrente con múltiples hilos, **sin usar mecanismos de sincronización**.

- **Archivo principal:** `lectura_sin_sincronizacion.c`
  - Cada hilo abre el mismo archivo y lee su contenido simultáneamente.
  - Permite observar los efectos de la falta de sincronización en la lectura de un recurso compartido.

**Objetivo:** Analizar el comportamiento de la lectura concurrente sin sincronización y comprender sus limitaciones y posibles problemas.

---

## Requisitos

- **Compilador:** GCC (o cualquier compilador con soporte para POSIX threads)
- **Librería:** `pthread`

## Compilación y Ejecución

1. Navega a la carpeta correspondiente, por ejemplo:
   ```bash
   cd lab1
