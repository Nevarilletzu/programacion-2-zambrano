#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

// 1. Estructura de datos del Pokémon
struct Pokemon {
    int numeroPokedex;
    char nombre[50];
    char tipo1[30];
    char tipo2[30];
    int total;
    int hp;
    int ataque;
    int defensa;
    int ataqueEsp;
    int defensaEsp;
    int velocidad;
    int generacion;
    char legendario[10];
};

// Estructura controladora para manejar el arreglo dinámico (como en tu proyecto)
struct BasePokemon {
    Pokemon* registros;
    int numPokemon;
    int capacidad;
};

// Inicializar la base de datos dinámica
void inicializarBase(BasePokemon* bp, int capacidadInicial) {
    bp->capacidad = capacidadInicial;
    bp->numPokemon = 0;
    bp->registros = new Pokemon[bp->capacidad];
}

// Liberar memoria dinámica al cerrar el programa (Evita fugas de memoria)
void liberarBase(BasePokemon* bp) {
    delete[] bp->registros;
    bp->registros = nullptr;
    bp->numPokemon = 0;
    bp->capacidad = 0;
}

// Redimensionar automáticamente duplicando la capacidad
void redimensionarBase(BasePokemon* bp) {
    bp->capacidad *= 2;
    Pokemon* nuevoArreglo = new Pokemon[bp->capacidad];
    for (int i = 0; i < bp->numPokemon; i++) {
        nuevoArreglo[i] = bp->registros[i];
    }
    delete[] bp->registros;
    bp->registros = nuevoArreglo;
}

// 2. FUNCIÓN DE FICHEROS: Cargar los Pokémon desde el archivo CSV
bool cargarPokemonsDesdeArchivo(BasePokemon* bp, const char* nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << "\n";
        return false;
    }

    string linea;
    // Leer y descartar la primera línea (la cabecera: Nº Pokedex,Nombre,Tipo 1...)
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        // Si el arreglo se llena, redimensionamos dinámicamente
        if (bp->numPokemon >= bp->capacidad) {
            redimensionarBase(bp);
        }

        stringstream ss(linea);
        string item;
        Pokemon p;

        // Parsear cada columna separada por comas
        getline(ss, item, ','); p.numeroPokedex = stoi(item);
        getline(ss, item, ','); strcpy_s(p.nombre, item.c_str());
        getline(ss, item, ','); strcpy_s(p.tipo1, item.c_str());
        
        getline(ss, item, ','); 
        strcpy_s(p.tipo2, item.c_str()); // Puede venir vacío

        getline(ss, item, ','); p.total = stoi(item);
        getline(ss, item, ','); p.hp = stoi(item);
        getline(ss, item, ','); p.ataque = stoi(item);
        getline(ss, item, ','); p.defensa = stoi(item);
        getline(ss, item, ','); p.ataqueEsp = stoi(item);
        getline(ss, item, ','); p.defensaEsp = stoi(item);
        getline(ss, item, ','); p.velocidad = stoi(item);
        getline(ss, item, ','); p.generacion = stoi(item);
        getline(ss, item, ','); strcpy_s(p.legendario, item.c_str());

        // Guardar en el arreglo dinámico
        bp->registros[bp->numPokemon] = p;
        bp->numPokemon++;
    }

    archivo.close();
    return true;
}

// --- FUNCIONES DE BÚSQUEDA Y CONSULTA ---

void buscarPorPokedex(BasePokemon bp, int numBuscado) {
    bool encontrado = false;
    for (int i = 0; i < bp.numPokemon; i++) {
        if (bp.registros[i].numeroPokedex == numBuscado) {
            cout << "\n--- INFORMACION DEL POKEMON (#" << bp.registros[i].numeroPokedex << ") ---\n";
            cout << "Nombre: " << bp.registros[i].nombre << "\n";
            cout << "Tipo 1: " << bp.registros[i].tipo1 << " | Tipo 2: " << (strlen(bp.registros[i].tipo2) > 0 ? bp.registros[i].tipo2 : "Ninguno") << "\n";
            cout << "Total Estadisticas: " << bp.registros[i].total << "\n";
            cout << "HP: " << bp.registros[i].hp << " | Ataque: " << bp.registros[i].ataque << " | Defensa: " << bp.registros[i].defensa << "\n";
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "No se encontro ningun Pokemon con el numero " << numBuscado << ".\n";
}

void buscarPorNombre(BasePokemon bp, const char* nombreBuscado) {
    bool encontrado = false;
    for (int i = 0; i < bp.numPokemon; i++) {
        if (_stricmp(bp.registros[i].nombre, nombreBuscado) == 0) {
            cout << "\n--- POKEMON ENCONTRADO POR NOMBRE ---\n";
            cout << "Nº Pokedex: " << bp.registros[i].numeroPokedex << "\n";
            cout << "Nombre: " << bp.registros[i].nombre << "\n";
            cout << "Tipo 1: " << bp.registros[i].tipo1 << " | Tipo 2: " << (strlen(bp.registros[i].tipo2) > 0 ? bp.registros[i].tipo2 : "Ninguno") << "\n";
            cout << "Total Stats: " << bp.registros[i].total << "\n";
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "No se encontro un Pokemon llamado " << nombreBuscado << ".\n";
}

void buscarPorTipo(BasePokemon bp, const char* tipoBuscado) {
    cout << "\n--- POKEMONS DE TIPO: " << tipoBuscado << " ---\n";
    int contador = 0;
    for (int i = 0; i < bp.numPokemon; i++) {
        if (_stricmp(bp.registros[i].tipo1, tipoBuscado) == 0 || _stricmp(bp.registros[i].tipo2, tipoBuscado) == 0) {
            cout << "#" << bp.registros[i].numeroPokedex << " - " << bp.registros[i].nombre 
                 << " (Total Stats: " << bp.registros[i].total << ")\n";
            contador++;
        }
    }
    if (contador == 0) cout << "No se encontraron Pokemons de ese tipo.\n";
}

void mostrarTopFuerteYDebil(BasePokemon bp) {
    if (bp.numPokemon == 0) return;

    // Copia temporal en memoria dinámica para ordenar
    Pokemon* temp = new Pokemon[bp.numPokemon];
    for (int i = 0; i < bp.numPokemon; i++) temp[i] = bp.registros[i];

    // Ordenamiento burbuja de mayor a menor por el 'total'
    for (int i = 0; i < bp.numPokemon - 1; i++) {
        for (int j = 0; j < bp.numPokemon - i - 1; j++) {
            if (temp[j].total < temp[j + 1].total) {
                Pokemon aux = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = aux;
            }
        }
    }

    cout << "\n=== TOP 3 POKEMONS MAS FUERTES ===\n";
    for (int i = 0; i < 3 && i < bp.numPokemon; i++) {
        cout << i + 1 << ". " << temp[i].nombre << " (#" << temp[i].numeroPokedex << ") - Total: " << temp[i].total << "\n";
    }

    cout << "\n=== TOP 3 POKEMONS MAS DEBILES ===\n";
    for (int i = 0; i < 3 && i < bp.numPokemon; i++) {
        int idx = bp.numPokemon - 1 - i;
        cout << i + 1 << ". " << temp[idx].nombre << " (#" << temp[idx].numeroPokedex << ") - Total: " << temp[idx].total << "\n";
    }

    delete[] temp;
}

int main() {
    BasePokemon pokedexSistema;
    inicializarBase(&pokedexSistema, 10); // Capacidad inicial pequeña para probar redimensionamiento

    // Cargar automáticamente desde el archivo CSV
    if (!cargarPokemonsDesdeArchivo(&pokedexSistema, "pokemon_150_registros.csv")) {
        cout << "No se pudo cargar la base de datos de los pokemones.\n";
        return 1;
    }

    cout << "¡Se cargaron " << pokedexSistema.numPokemon << " Pokemons exitosamente desde el archivo!\n";

    int opcion;
    do {
        cout << "\n==============================\n";
        cout << "      MENU TAREA POKEMON      \n";
        cout << "==============================\n";
        cout << "1. Ver info de Pokemon por Pokedex\n";
        cout << "2. Buscar Pokemon por nombre\n";
        cout << "3. Ver Pokemons por tipo\n";
        cout << "4. Ver Top 3 mas fuertes y debiles\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int num;
            cout << "Ingrese el numero de Pokedex: ";
            cin >> num;
            buscarPorPokedex(pokedexSistema, num);
        } 
        else if (opcion == 2) {
            char nombre[50];
            cout << "Ingrese el nombre del Pokemon: ";
            cin >> nombre;
            buscarPorNombre(pokedexSistema, nombre);
        } 
        else if (opcion == 3) {
            char tipo[30];
            cout << "Ingrese el tipo (ej. Fire, Water, Grass, Electric): ";
            cin >> tipo;
            buscarPorTipo(pokedexSistema, tipo);
        } 
        else if (opcion == 4) {
            mostrarTopFuerteYDebil(pokedexSistema);
        }
    } while (opcion != 5);

    // Liberar memoria antes de salir
    liberarBase(&pokedexSistema);
    cout << "\n¡Memoria liberada y programa finalizado con exito!\n";
    return 0;
}