#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono>

using namespace std;

const int N = 7;
int grafo[N][N] = {
    {0, 0, 0, 0, 0, 10, 13}, // Fabrica 1
    {0, 0, 4, 0, 15, 0, 7},  // Fabrica 2
    {0, 4, 0, 2, 5, 0, 0},   // Tienda 1
    {0, 0, 2, 0, 10, 0, 6},  // Tienda 2
    {0, 15, 5, 10, 0, 13, 8},// Tienda 3
    {10, 0, 0, 0, 13, 0, 7}, // Tienda 4
    {13, 7, 0, 6, 8, 7, 0}   // Tienda 5
};

int calcularDistanciaRuta(const vector<int>& ruta) {
    int distancia = 0;
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        distancia += grafo[ruta[i]][ruta[i + 1]];
    }
    return distancia;
}

vector<int> encontrarRutaMinima(const vector<int>& tiendas, int src) {
    vector<int> rutaMinima;
    int menorDistancia = INT_MAX;

    vector<int> perm;
    perm.push_back(src);
    perm.insert(perm.end(), tiendas.begin(), tiendas.end());

    sort(perm.begin() + 1, perm.end());

    do {
        int distancia = calcularDistanciaRuta(perm);
        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            rutaMinima = perm;
        }
    } while (next_permutation(perm.begin() + 1, perm.end()));

    return rutaMinima;
}

double calcularConsumoGasolina(int distancia, int numParadas) {
    double consumoBase = 0.1; // 0.1 litros por kilómetro
    double consumoParada = 0.5; // 0.5 litros por parada
    return (distancia * consumoBase) + (numParadas * consumoParada);
}

void procesarCamion(int id, int fabrica, const vector<int>& tiendas) {
    vector<int> ruta_optima = encontrarRutaMinima(tiendas, fabrica);
    int distancia = calcularDistanciaRuta(ruta_optima);
    int numParadas = tiendas.size();
    double gasolina_usada = calcularConsumoGasolina(distancia, numParadas);

    cout << "La ruta calculada para el camion " << id << " es: ";
    for (size_t j = 0; j < ruta_optima.size(); ++j) {
        if (j == 0) {
            cout << "Fabrica " << ruta_optima[j] + 1;
        } else {
            cout << " -> Tienda " << ruta_optima[j] + 1;
        }
    }
    cout << endl;
    cout << "Kilometros recorridos: " << distancia << " km" << endl;
    cout << "Numero de paradas: " << numParadas << endl;
    cout << "Calculo de gasolina:" << endl;
    cout << "  Consumo por distancia: " << (distancia * 0.1) << " litros" << endl;
    cout << "  Consumo por paradas: " << (numParadas * 0.5) << " litros" << endl;
    cout << "Gasolina total usada para el camion " << id << ": " << gasolina_usada << " litros." << endl;
    cout << endl;
}

int main() {
    while (true) {
        int camiones;
        vector<int> fabricas;
        vector<vector<int>> todas_tiendas;

        cout << "------------------------------------------------------------" << endl;
        cout << "BIENVENIDO AL PROGRAMA DE CALCULO DE RUTAS Y CONSUMO DE GASOLINA" << endl;
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
            fabricas[i] = fabrica - 1;

            cout << "A que tiendas debe ir a repartir? (Ingrese numeros separados por espacios): " << endl;
            int tienda;
            while (cin >> tienda) {
                todas_tiendas[i].push_back(tienda - 1);
                if (cin.peek() == '\n') break;
            }
        }

        auto start_time = chrono::high_resolution_clock::now();

        for (int i = 0; i < camiones; ++i) {
            procesarCamion(i + 1, fabricas[i], todas_tiendas[i]);
        }

        auto end_time = chrono::high_resolution_clock::now();
        auto execution_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        cout << "Tiempo de ejecucion del programa (solo calculos): " << execution_time << " milisegundos." << endl;

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
