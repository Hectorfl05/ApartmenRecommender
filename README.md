<div align="center">

# 🏢 Apartment Recommender Engine
### Algoritmo de Optimización y Recomendación de Ubicaciones en C++

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey?style=flat)](#-compilación-y-ejecución)
[![Build](https://img.shields.io/badge/Build-CMake%20%7C%20Make%20%7C%20MSBuild-blue?style=flat)](#-compilación-y-ejecución)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat)](LICENSE)

*Motor algorítmico eficiente diseñado para resolver el problema de selección óptima de viviendas basado en proximidad multidireccional a servicios urbanos esenciales.*

</div>

---

## 📋 Tabla de Contenidos
- [Descripción General](#-descripción-general)
- [Algoritmo y Criterios de Selección](#-algoritmo-y-criterios-de-selección)
- [Análisis de Complejidad](#-análisis-de-complejidad)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Requisitos Previos](#-requisitos-previos)
- [Compilación y Ejecución](#-compilación-y-ejecución)
  - [En Linux / macOS](#en-linux--macos)
  - [En Windows (Visual Studio)](#en-windows-visual-studio)
- [Formato de Datos](#-formato-de-datos)
  - [Entrada (JSON Lines)](#entrada-json-lines)
  - [Salida](#salida)
- [Aspectos Destacados de Ingeniería](#-aspectos-destacados-de-ingeniería)
- [Autor](#-autor)

---

## 📌 Descripción General

El **Apartment Recommender Engine** es una solución de alto rendimiento desarrollada en **C++17** para abordar el problema clásico de toma de decisiones geoespaciales y localización óptima. 

Dado un entorno urbano lineal con múltiples opciones de apartamentos y una lista de negocios o comodidades de interés (gimnasios, farmacias, supermercados, escuelas, clínicas, etc.), el sistema procesa conjuntos de datos en formato **JSON Lines (.jsonl)** para evaluar el costo de desplazamiento desde cada ubicación y seleccionar automáticamente el apartamento ideal.

---

## 🧠 Algoritmo y Criterios de Selección

Para cada apartamento evaluado, el algoritmo realiza una búsqueda bidireccional (hacia adelante y hacia atrás en la avenida o sector) calculando la distancia mínima (en pasos o bloques) requerida para alcanzar cada servicio solicitado.

La selección del apartamento ganador sigue una jerarquía estricta de dos fases:

1. **Minimización de la Distancia Total (Criterio Principal)**:
   $$\text{Score}(A_i) = \sum_{b \in B} \text{dist}(A_i, b)$$
   Donde $B$ representa el conjunto de negocios deseados. Se prioriza el apartamento con la menor sumatoria total de pasos.

2. **Criterio Minimax de Desempate (Robustez ante Casos Extremos)**:
   Si dos o más apartamentos poseen exactamente la misma sumatoria de distancias, se selecciona aquel que minimice la peor distancia individual:
   $$\text{Minimax}(A_i) = \max_{b \in B} (\text{dist}(A_i, b))$$
   Esto garantiza que el usuario nunca quede excesivamente lejos de ninguno de sus servicios prioritarios.

3. **Manejo de Casos Inválidos**:
   Si alguno de los negocios requeridos no existe en ningún punto del sector evaluado, el caso se reporta como no viable (`[]`).

---

## ⏱️ Análisis de Complejidad

- **Complejidad Temporal**:
  - Para un conjunto de $N$ apartamentos y $M$ negocios requeridos, la búsqueda bidireccional evalúa como máximo $O(N)$ posiciones por cada requerimiento.
  - La complejidad por caso es $O(N \cdot M)$.
  - En escenarios de procesamiento por lotes (*batch streaming*), cada línea del archivo se evalúa independientemente sin acumulación innecesaria en memoria.

- **Complejidad Espacial**:
  - $O(N \cdot K)$ donde $K$ es el número promedio de servicios presentes por apartamento, almacenados eficientemente en memoria mediante estructuras STL optimizadas (`std::vector` y `std::list`).

---

## 📁 Estructura del Proyecto

El repositorio sigue las mejores prácticas de organización de software en C++:

```text
ApartmentRecommender/
├── data/
│   └── input_example.jsonl       # Dataset de prueba con múltiples casos en JSON Lines
├── include/
│   └── json/                     # Cabeceras de la librería JsonCpp
├── src/
│   ├── Apartment.h               # Declaración de la clase Apartment (POO)
│   ├── Apartment.cpp             # Implementación de métodos de la entidad
│   └── main.cpp                  # Algoritmo de recomendación, CLI y parser JSONL
├── .gitignore                    # Filtro de artefactos binarios y temporales
├── ApartmentRecommender.sln       # Solución de Microsoft Visual Studio 2022
├── ApartmentRecommender.vcxproj   # Proyecto MSBuild configurado con C++17
├── ApartmentRecommender.vcxproj.filters # Organización de filtros para IDEs
├── CMakeLists.txt                # Configuración de compilación multiplataforma
├── Makefile                      # Script de compilación rápida para UNIX/Linux
└── README.md                     # Documentación técnica del proyecto
```

---

## 🛠️ Requisitos Previos

### Dependencias Principales
- **Compilador C++**: Soporte para C++17 (`g++` >= 9, `clang++` >= 10, o MSVC v143+).
- **Librería JsonCpp**: Utilizada para el análisis sintáctico seguro de JSON.
  - **Debian / Ubuntu**:
    ```bash
    sudo apt-get install libjsoncpp-dev
    ```
  - **macOS (Homebrew)**:
    ```bash
    brew install jsoncpp
    ```
  - **Windows (vcpkg)**:
    ```cmd
    vcpkg install jsoncpp:x64-windows
    ```

---

## 🚀 Compilación y Ejecución

### En Linux / macOS

#### Opción 1: Mediante `Makefile` (Rápido)
```bash
# Compilar el ejecutable
make

# Ejecutar el programa
./ApartmentRecommender

# Limpiar archivos compilados
make clean
```

#### Opción 2: Mediante `CMake` (Recomendado para entornos cruzados)
```bash
# Generar archivos de construcción
cmake -B build -S .

# Compilar
cmake --build build

# Ejecutar
./build/ApartmentRecommender
```

#### Opción 3: Compilación directa con `g++`
```bash
g++ -std=c++17 -O2 -Iinclude -Isrc src/main.cpp src/Apartment.cpp -o ApartmentRecommender -ljsoncpp
./ApartmentRecommender
```

---

### En Windows (Visual Studio)

1. Abrir `ApartmentRecommender.sln` en **Visual Studio 2022**.
2. Seleccionar la configuración de destino (`Release` o `Debug`) y la arquitectura (`x64`).
3. Compilar la solución con `Ctrl + Shift + B` o desde el menú **Compilar > Compilar solución**.
4. Ejecutar el proyecto presionando `F5` o `Ctrl + F5`.

---

## 📊 Formato de Datos

### Entrada (JSON Lines)
Cada línea del archivo representa una configuración urbana completa con dos claves principales:
- `input1`: Arreglo de apartamentos (ordenados espacialmente del índice $0$ al $N-1$). Cada elemento es un objeto JSON donde las claves son nombres de servicios y los valores son booleanos indicando presencia.
- `input2`: Arreglo de cadenas de texto con los servicios requeridos por el usuario.

**Ejemplo (`data/input_example.jsonl`):**
```json
{"input1":[{"Pharmacy":false},{"Restaurant":true},{"Gym":true},{"Pharmacy":true,"Restaurant":false}],"input2":["Pharmacy","Gym"]}
```

### Salida
El programa genera un archivo de resultados `Recomendacion Apartamentos.txt` y presenta en consola el índice del apartamento recomendado por cada caso:

```text
Caso 1: [108]
Caso 2: [57]
Caso 3: [] (Sin apartamento viable)
Caso 4: [128]
...
```

- `[i]`: Índice base-0 del apartamento con la ubicación óptima calculada.
- `[]`: Indica que al menos uno de los servicios requeridos no está presente en todo el mapa.

---

## ✨ Aspectos Destacados de Ingeniería

- **Diseño Modular y Limpio**: Separación clara de responsabilidades entre la entidad de dominio (`Apartment`), la lógica de resolución algorítmica y la interfaz de usuario por consola.
- **Eficiencia y Const-Correctness**: Paso de colecciones complejas por referencia constante (`const &`) para evitar copias profundas innecesarias durante las llamadas cíclicas del algoritmo.
- **Compatibilidad Multiplataforma**: Integración de archivos de proyecto para Visual Studio (`.sln`, `.vcxproj`) junto con `CMakeLists.txt` y `Makefile` para compatibilidad en sistemas POSIX.
- **Limpieza de Control de Versiones**: Exclusión rigurosa de archivos binarios compilados (`.exe`, `.pdb`, `.obj`) a través de un archivo `.gitignore` estandarizado.

---

## 👤 Autor

**Héctor Flores**

---

<div align="center">
  <sub>Desarrollado como proyecto de aplicación de Estructuras de Datos y Algoritmos en C++.</sub>
</div>
