#include <iostream>     //Importar bibliotecas
#include <vector>       
#include <climits>      
#include <algorithm>    
#include <chrono>       
#include <omp.h>        

using namespace std;

const int N = 7; 

// Matriz de adyacencia que representa las distancias entre las fábricas y las tiendas
int grafo[N][N] = {
    {0, 0, 0, 0, 0, 10, 13}, // Fabrica 1
    {0, 0, 4, 0, 15, 0, 7},  // Fabrica 2
    {0, 4, 0, 2, 5, 0, 0},   // Tienda 1
    {0, 0, 2, 0, 10, 0, 6},  // Tienda 2
    {0, 15, 5, 10, 0, 13, 8},// Tienda 3
    {10, 0, 0, 0, 13, 0, 7}, // Tienda 4
    {13, 7, 0, 6, 8, 7, 0}   // Tienda 5
};

// Función que calcula la distancia total de una ruta dada
int calcularDistanciaRuta(const vector<int>& ruta) {
    int distancia = 0;
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        distancia += grafo[ruta[i]][ruta[i + 1]]; // Suma las distancias entre nodos consecutivos
    }
    return distancia; 
}

// Función que encuentra la ruta mínima para un conjunto de tiendas, partiendo de una fábrica
vector<int> encontrarRutaMinima(const vector<int>& tiendas, int src) {
    vector<int> rutaMinima; // Vector para almacenar la ruta mínima
    int menorDistancia = INT_MAX; // Inicializa la menor distancia con el valor máximo posible

    vector<int> perm;
    perm.push_back(src); // Añade la fábrica de origen al inicio
    perm.insert(perm.end(), tiendas.begin(), tiendas.end()); // Añade las tiendas al vector

    sort(perm.begin() + 1, perm.end()); // Ordena las tiendas para generar las permutaciones

    do {
        int distancia = calcularDistanciaRuta(perm); // Calcula la distancia de la ruta actual
        if (distancia < menorDistancia) { // Si la distancia es menor que la menor registrada
            menorDistancia = distancia; // Actualiza la menor distancia
            rutaMinima = perm; // Guarda la ruta actual como la mínima
        }
    } while (next_permutation(perm.begin() + 1, perm.end())); 

    return rutaMinima; 
}

// Función que calcula el consumo de gasolina basado en la distancia y el número de paradas
double calcularConsumoGasolina(int distancia, int numParadas) {
    double consumoBase = 0.1; // 0.1 litros por kilómetro
    double consumoParada = 0.5; // 0.5 litros por parada
    return (distancia * consumoBase) + (numParadas * consumoParada); 
}

// Función que procesa la información de un camión específico
void procesarCamion(int id, int fabrica, const vector<int>& tiendas) {
    vector<int> ruta_optima = encontrarRutaMinima(tiendas, fabrica); // Encuentra la ruta óptima
    int distancia = calcularDistanciaRuta(ruta_optima); // Calcula la distancia de la ruta óptima
    int numParadas = tiendas.size(); // Calcula el número de paradas
    double gasolina_usada = calcularConsumoGasolina(distancia, numParadas); // Calcula el consumo de gasolina

    // Muestra la ruta calculada y el consumo de gasolina
    cout << "La ruta calculada para el camion " << id << " es: ";
    for (size_t j = 0; j < ruta_optima.size(); ++j) {
        if (j == 0) {
            cout << "Fabrica " << ruta_optima[j] + 1;
        } else {
            cout << " -> Tienda " << ruta_optima[j] + 1;
        }
    }
    cout << endl;
    cout << "Numero de paradas: " << numParadas << endl;
    cout << "Gasolina total usada para el camion " << id << ": " << gasolina_usada << " litros." << endl;
    cout << endl;
}

int main() {
    while (true) { // Main
        int camiones; 
        vector<int> fabricas; 
        vector<vector<int>> todas_tiendas; 

        cout << "------------------------------------------------------------" << endl;
        cout << "BIENVENIDO AL PROGRAMA DE CALCULO DE RUTAS Y CONSUMO DE GASOLINA" << endl;
        cout << "Hay 2 fabricas y 5 tiendas" << endl;
        cout << "Por favor, responda las siguientes preguntas: " << endl;
        cout << "Cuantos camiones saldran a repartir? " << endl;
        cin >> camiones;

        fabricas.resize(camiones); 
        todas_tiendas.resize(camiones); 

        
        for (int i = 0; i < camiones; ++i) {
            cout << "Camion " << i + 1 << ": " << endl;
            cout << "Desde que fabrica saldra? (1 o 2): " << endl;
            int fabrica;
            cin >> fabrica;
            fabricas[i] = fabrica - 1; // Guarda la fábrica (restando 1 para convertir a índice de matriz)
    
            cout << "A que tiendas debe ir a repartir? (Ingrese numeros separados por espacios): " << endl;
            int tienda;
            while (cin >> tienda) {
                todas_tiendas[i].push_back(tienda - 1); 
                if (cin.peek() == '\n') break; 
            }
        }

        // Inicia la medición del tiempo de ejecución
        auto start_time = chrono::high_resolution_clock::now();

        // Paraleliza el cálculo de rutas para los camiones
        #pragma omp parallel for
        for (int i = 0; i < camiones; ++i) {
            #pragma omp critical // Asegura que solo un hilo a la vez acceda a la sección crítica
            {
                procesarCamion(i + 1, fabricas[i], todas_tiendas[i]); 
            }
        }

        // Finaliza la medición del tiempo de ejecución
        auto end_time = chrono::high_resolution_clock::now();
        auto execution_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        cout << "Tiempo de ejecucion del programa (solo calculos): " << execution_time << " milisegundos." << endl;

        // Pregunta al usuario si desea realizar otro cálculo y si si repite el bucle
        cout << "Desea realizar otro calculo? s/n" << endl;
        char resp;
        cin >> resp;
        if (resp == 'n') { 
            cout << "Nos vemos pronto, feliz dia" << endl;
            break;
        } else if (resp != 's') { 
            cout << "Respuesta invalida, ingrese 's' o 'n' " << endl;
        }
    }

    return 0; 
}
