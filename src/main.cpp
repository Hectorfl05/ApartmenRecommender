#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
#include "Apartment.h"

using namespace std;

// Función para limpiar la pantalla de manera multiplataforma
void ClearScreen()
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

/**
 * Busca la distancia mínima (en número de pasos) hacia un negocio deseado
 * desde la posición del apartamento dado (index), buscando tanto hacia atrás
 * como hacia adelante.
 */
int BuscarApartamento(const vector<Apartment>& Mapa, const string& negocio, int index)
{
    int pasosBack = -1;
    int pasosFront = -1;

    // Recorre el mapa de apartamentos desde el índice seleccionado hacia atrás
    for (int j = index; j >= 0; --j)
    {
        const auto& businesses = Mapa[j].getBusiness();
        if (find(businesses.begin(), businesses.end(), negocio) != businesses.end())
        {
            pasosBack = index - j;
            break;
        }
    }

    // Recorre el mapa de apartamentos desde el índice seleccionado hacia adelante
    for (int j = index; j < static_cast<int>(Mapa.size()); ++j)
    {
        const auto& businesses = Mapa[j].getBusiness();
        if (find(businesses.begin(), businesses.end(), negocio) != businesses.end())
        {
            pasosFront = j - index;
            break;
        }
    }

    // Si no encontró el negocio en una de las dos direcciones, devuelve la distancia válida
    if (pasosFront == -1 || pasosBack == -1)
    {
        return max(pasosBack, pasosFront);
    }

    // Retorna la cantidad mínima de pasos necesarios para encontrar el negocio
    return min(pasosBack, pasosFront);
}

/**
 * Evalúa todos los apartamentos y devuelve el índice del apartamento óptimo
 * que minimiza la suma total de distancias y, en caso de empate, minimiza
 * la distancia máxima individual a cualquiera de los negocios requeridos.
 * Si algún negocio no se encuentra disponible en ningún apartamento, retorna -1.
 */
int Recomendacion(const vector<Apartment>& Mapa, const vector<string>& B)
{   
    int MinPasos = numeric_limits<int>::max();
    int BestIndex = -1;
    int BestMaxpasitos = 0;

    // Recorre cada apartamento candidato
    for (int i = 0; i < static_cast<int>(Mapa.size()); ++i)
    {
        int totalPasos = 0;
        int Maxpasitos = 0;
        bool missingBusiness = false;

        // Itera sobre los negocios requeridos
        for (const string& negocio : B)
        {
            int pasitos = BuscarApartamento(Mapa, negocio, i);

            // Si el negocio no existe en el mapa, no es posible satisfacer la búsqueda
            if (pasitos == -1)
            {
                missingBusiness = true;
                break;
            }

            if (pasitos > Maxpasitos)
            {
                Maxpasitos = pasitos;
            }

            totalPasos += pasitos;
        }

        // Si falta algún negocio, se descarta este apartamento
        if (missingBusiness)
        {
            continue;
        }

        // Criterio 1: Minimizar la suma de distancias
        if (totalPasos < MinPasos)
        {
            MinPasos = totalPasos;
            BestIndex = i;
            BestMaxpasitos = Maxpasitos;
        }
        // Criterio 2 (Desempate): Minimizar la distancia máxima individual
        else if (totalPasos == MinPasos) 
        {
            if (Maxpasitos < BestMaxpasitos)
            {
                MinPasos = totalPasos;
                BestIndex = i;
                BestMaxpasitos = Maxpasitos;
            }
        }       
    }

    return BestIndex;
}

template<typename T>
void EmptyVector(vector<T>& vec)
{
    vec.clear();
}

int main()
{ 
    string ruta;
    ifstream file;
    ofstream Archivo;
    string ruta2;

    cout << "=====================================================" << endl;
    cout << "        SISTEMA DE RECOMENDACION DE APARTAMENTOS     " << endl;
    cout << "=====================================================" << endl;

    // Bucle para solicitar la ruta del archivo JSON Line de entrada
    while (true)
    {
        cout << "\nIngrese la ruta del archivo JSON Line (o presione ENTER para usar 'data/input_example.jsonl'): " << endl;
        getline(cin, ruta);

        if (ruta.empty())
        {
            ruta = "data/input_example.jsonl";
        }

        file.open(ruta, ios::in);
        if (file.is_open())
        {
            break;
        }

        cout << "[!] Error: No se pudo abrir el archivo en la ruta especificada: " << ruta << endl;
    }

    // Bucle para solicitar la ruta de exportación de resultados
    while (true)
    {
        cout << "\nIngrese la carpeta de destino para el archivo de salida (o presione ENTER para la carpeta actual): " << endl;
        getline(cin, ruta2);

        string outputPath = ruta2.empty() ? "Recomendacion Apartamentos.txt" : (ruta2 + "/Recomendacion Apartamentos.txt");
        Archivo.open(outputPath);

        if (!Archivo.fail())
        {
            cout << "\n[+] Archivo de salida creado en: " << outputPath << endl;
            break;
        }

        cout << "[!] Error: No se pudo generar el archivo en: " << outputPath << endl;
    }

    // Crea el lector de JSON
    Json::Reader reader;
    Json::Value json;
    string line;
    int lineNum = 0;

    cout << "\nProcesando recomendaciones...\n" << endl;

    // Lee el archivo línea por línea
    while (getline(file, line))
    {
        if (line.empty()) continue;
        lineNum++;

        // Parsea la línea JSON
        bool parsed = reader.parse(line, json, false);
        if (!parsed)
        {
            cerr << "Error al analizar la linea " << lineNum << ": " << reader.getFormattedErrorMessages() << endl;
            return 1;
        }

        const Json::Value input1Array = json["input1"];
        const Json::Value input2Array = json["input2"];

        vector<Apartment> Map;
        vector<string> input2;

        // Procesa el listado de apartamentos (input1)
        for (const auto& item : input1Array) 
        {  
            list<string> negocios;
            for (const auto& itemB : item.getMemberNames())
            {
                if (item[itemB].asBool())
                {
                    negocios.push_back(itemB);
                }
            }
            Map.emplace_back(negocios);
        }

        // Procesa el listado de requerimientos (input2)
        for (const auto& item : input2Array)
        {
            input2.push_back(item.asString());
        }
            
        // Calcula la recomendación
        int resultado = Recomendacion(Map, input2);

        if (resultado == -1)
        {
            Archivo << "[]" << endl;
            cout << "Caso " << lineNum << ": [] (Sin apartamento viable)" << endl;
        }
        else
        {
            Archivo << "[" << resultado << "]" << endl;
            cout << "Caso " << lineNum << ": [" << resultado << "]" << endl;
        }

        EmptyVector(Map);
        EmptyVector(input2);
        Archivo.clear();
    }

    file.close();
    Archivo.close();

    cout << "\n[+] Proceso completado exitosamente." << endl;
    return 0;
}
