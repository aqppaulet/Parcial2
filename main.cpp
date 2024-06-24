#include <cstdlib>
#include <windows.h>

#include "regFijos.h"
#include "RWesquema.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

#include "include/BufferManager.h"
#include "include/page.h"
#include "include/diskSpaceManager.h"


using namespace std;

//lee y extrae rutas
class FileManager {
public:

        vector<string> bloquePesoAcargar(int pesoRegistro, string nombreSquema) {
            ifstream archivo("bloques.txt");
            string linea;
            vector<string> result(3); // Inicializamos un vector de tamaño 3

            while (getline(archivo, linea)) {
                stringstream ss(linea);
                string parte;
                string nombreBloque;
                int pesoBloque;
                string esquemaEnBloque;

                // Leer nombre del bloque
                if (getline(ss, parte, '|')) {
                    nombreBloque = parte;
                } else {
                    continue;
                }

                // Leer peso del bloque
                if (getline(ss, parte, '|')) {
                    pesoBloque = stoi(parte);
                } else {
                    continue;
                }

                // Leer esquema en bloque (si existe)
                if (getline(ss, parte, '|')) {
                    esquemaEnBloque = parte;
                } else {
                    esquemaEnBloque = "";
                }

                // Verificar condiciones
                if ((esquemaEnBloque == nombreSquema || esquemaEnBloque == "") && pesoBloque >= pesoRegistro) {
                    result[0] = nombreBloque;
                    result[1] = to_string(pesoBloque);
                    result[2] = esquemaEnBloque;
                    return result;
                }
            }

            // Si no se encontró ningún bloque que cumpla las condiciones, retornar un vector vacío
        return vector<string>();
    }

         bool verificaresquema(string esquema)
    {
        ifstream archivo("esquemas.txt");
        string linea;
        size_t pos = 0;
        string token;
        while (getline(archivo, linea))
        {

            pos = linea.find('#', pos);
            token = linea.substr(0, pos);
            pos = 0; // reiniciar la pocicion
            cout << endl;
            if (token == esquema)
            {
                return true;
            }
        }
        return false;
    }

        string extraerEsquema(const string &nombreTabla)
        {
            string fila;
            ifstream archivoExistente("esquemas.txt");

            if (!archivoExistente.is_open())
            {
                cerr << "Error al abrir el archivo 'esquemas.txt'." << endl;
                return fila;
            }

            string linea;
            while (getline(archivoExistente, linea))
            {
                size_t indice = linea.find('#');
                if (indice != string::npos)
                {
                    string nombreTablaExistente = linea.substr(0, indice);
                    if (nombreTablaExistente == nombreTabla)
                    {
                        // Extraer los tipos de datos de la línea
                        stringstream ss(linea.substr(indice + 1)); // Saltar el nombre de la tabla
                        getline(ss, fila);                         // Leer toda la línea desde el stringstream
                        break;                                     // Salir del bucle una vez que se ha encontrado la línea
                    }
                }
            }

            archivoExistente.close();
            return fila;
        }

        void IngresarEsquema(string &nombre)
        {
            string nombreColumna;
            string tipoDato;
            string peso;

            std::ofstream archivo("esquemas.txt", std::ios::app); // Abre en modo append

            if (archivo.is_open())
            {
                int continua = 1;
                std::string esquema;
                esquema = nombre;

                while (continua == 1)
                {
                    cout << "Ingrese el nombre de la columna: ";
                    cin >> nombreColumna;

                    cout << "Ingrese el tipo: ";
                    cin >> tipoDato;

                    cout << "Ingrese cantidad: ";
                    cin >> peso;

                    // Concatenar los datos de fila
                    esquema += "#" + nombreColumna + "#" + tipoDato + "#" + peso;

                    cout << "¿Desea agregar otra columna? (1 = sí, 0 = no): ";
                    cin >> continua;

                    while (continua != 0 && continua != 1)
                    {
                        cout << "Por favor, ingrese 1 si desea continuar o 0 si desea salir: ";
                        cin.clear();
                        cin >> continua;
                    }
                }
                archivo << esquema << std::endl;

                archivo.close();
            }
            else
            {
                std::cerr << "Error al abrir el archivo 'esquema.txt'" << std::endl;
            }
        }

        void GuardarEsquemaDesdeArchivo(string &nombreTabla)
{
    string tipoDato;
    string resultado = nombreTabla.substr(0, nombreTabla.size() - 4);

    ifstream archivo(nombreTabla);

    if (!archivo.is_open())
    {
        cerr << "Error al abrir el archivo '" << nombreTabla << "'." << endl;
        return; // Devolver un string vacío si no se puede abrir el archivo
    }

    ofstream archivoSalida("esquemas.txt", ios::app);

    string linea;
    getline(archivo, linea);
    istringstream ss(linea);
    string base;
    int count = 0;
    int max = 0;

    getline(archivo, linea);
    istringstream sss(linea);
    string base2;

    while (getline(ss, base, ';') && getline(sss, base2, ';'))
    {
        resultado += "#" + base;
        tipoDato = RWhelper::determinarTipoDato(base2);

        //obtener la longitud maxima de cada columna
        max = RWhelper::MayorLongitudCol(nombreTabla, count);
        if (tipoDato == "int" && max < 100000000)
        {
            resultado += "#int#8";
        }
        else
        {
            resultado += "#" + tipoDato + "#" + to_string(max);
        }
        count++;
    }

    archivo.close();
    archivoSalida << resultado << '\n';
    archivoSalida.close();
}


};

//crea disco carpetas y txt
class SimuladorDisco
{
public:
    int num_platos;
    int num_caras;
    int num_pistas;
    int num_sectores;
    int tamanioSector;
    int sectxBloque;

    SimuladorDisco()
        : num_platos(3), num_caras(2), num_pistas(4), num_sectores(4), tamanioSector(1024), sectxBloque(4)
    {
        long long tamDiskoBytes = num_platos * num_caras * num_pistas * num_sectores * tamanioSector;
        double tamDiskokB = static_cast<double>(tamDiskoBytes) / 1024; // Convertir bytes a kilobytes

        cout << "Tamaño total del disco: " << tamDiskokB << " kB" << std::endl;
        guardarCaracteristicasDisco();
        crearCarpetas();
        bloque();
        mostrarCaracteristicasDisco();
    }

    SimuladorDisco(int platos, int caras, int pistas, int sectores, int tamSector, int sectoresBloque)
        : num_platos(platos), num_caras(caras), num_pistas(pistas), num_sectores(sectores), tamanioSector(tamSector), sectxBloque(sectoresBloque)
    {
        long long tamDiskoBytes = num_platos * num_caras * num_pistas * num_sectores * tamanioSector;
        double tamDiskokB = static_cast<double>(tamDiskoBytes) / 1024; // Convertir bytes a kilobytes

        cout << "Tamaño total del disco: " << tamDiskokB << " kB" << std::endl;
        guardarCaracteristicasDisco();
        crearCarpetas();
        bloque();
        mostrarCaracteristicasDisco();
    }

    void crearCarpetas()
    {
        string nombredisko = "disk";

        if (existeCarpeta(nombredisko))
        {
            cout << "La carpeta '" << nombredisko << "' ya existe." << endl;
            return; // Salir de la función si la carpeta ya existe
        }

        crearCarpeta(nombredisko);
        for (int plato = 1; plato <= num_platos; ++plato)
        {
            string nombreCarpetaPlato = nombredisko + "\\Plato" + std::to_string(plato);
            crearCarpeta(nombreCarpetaPlato);

            for (int cara = 1; cara <= num_caras; ++cara)
            {
                string nombreCarpetaCara = nombreCarpetaPlato + "\\Cara" + std::to_string(cara);
                crearCarpeta(nombreCarpetaCara);

                for (int pista = 1; pista <= num_pistas; ++pista)
                {
                    string nombreCarpetaPista = nombreCarpetaCara + "\\Pista" + std::to_string(pista);
                    crearCarpeta(nombreCarpetaPista);

                    for (int sector = 1; sector <= num_sectores; ++sector)
                    {
                        string nombreArchivoSector = nombreCarpetaPista + "\\Sector" + std::to_string(sector) + ".txt";
                        crearArchivo(nombreArchivoSector);
                    }
                }
            }
        }
    }

    void crearCarpeta(const std::string &nombre)
    {
        if (CreateDirectoryA(nombre.c_str(), NULL) != 0)
        {
            // std::cout << "Carpeta '" << nombre << "' creada correctamente." << std::endl;
        }
        else
        {
            DWORD error = GetLastError();
            if (error == ERROR_ALREADY_EXISTS)
            {
                std::cout << "La carpeta '" << nombre << "' ya existe." << std::endl;
            }
            else
            {
                std::cerr << "Error al crear la carpeta '" << nombre << "'. Código de error: " << error << std::endl;
            }
        }
    }

    void crearArchivo(const std::string &nombre)
    {
        ofstream archivo(nombre);
        if (archivo.is_open())
        {
            archivo << string(tamanioSector, '0'); // Escribir 1024 '0' para simular el tamaño del sector
            archivo.close();
        }
        else
        {
            cerr << "Error al crear el archivo '" << nombre << "'" << endl;
        }
    }

    bool existeCarpeta(const string &nombre)
    {
        DWORD atributos = GetFileAttributesA(nombre.c_str());
        return (atributos != INVALID_FILE_ATTRIBUTES && (atributos & FILE_ATTRIBUTE_DIRECTORY));
    }

    void caracteristicasDisco()
    {
        ofstream archivo("caracteristicasDisco.txt");
        string linea = to_string(tamanioSector) + "-" + to_string(sectxBloque);
        linea += "-" + to_string(num_platos) + "-" + to_string(num_caras) + "-" + to_string(num_pistas) + "-" + to_string(num_sectores);
        archivo << linea;
        archivo.close();
    }
    
    void bloque()
    {
        string carpetaBloques = "disk\\bloques";

        if (existeCarpeta(carpetaBloques))
        {
            cout << "La carpeta 'bloques' ya existe." << endl;
            return;
        }

        crearCarpeta(carpetaBloques);

        ifstream verificarArchivo("bloques.txt");
        if (verificarArchivo.good())
        {
            cout << "El archivo 'bloques.txt' ya existe." << endl;
            return;
        }

        ofstream archivoBloques("bloques.txt");
        string bloque_actual;
        int contador_bloque = 0;
        int suma_bloque = 0;
        int b = 0;

        for (int sector = 1; sector <=num_sectores; ++sector)
        {
            for (int pista = 1; pista <= num_pistas; ++pista)
            {
                for (int cara = 1; cara <=num_caras; ++cara)
                {
                    for (int plato = 1; plato <=num_platos; ++plato)
                    {
                        string linea = to_string(tamanioSector) + "-" + to_string(plato) + "-" + to_string(cara) + "-" + to_string(pista) + "-" + to_string(sector);
                        bloque_actual += linea + "|";
                        suma_bloque += tamanioSector;
                        contador_bloque++;

                        if (contador_bloque == sectxBloque)
                        {
                            string nombreBloque = carpetaBloques + "\\" + to_string(b) + ".txt";
                            archivoBloques << to_string(b) + "|" + to_string(suma_bloque) + "\n";

                            ofstream archivoBloque(nombreBloque);
                            if (archivoBloque.is_open())
                            {
                                archivoBloque <<  bloque_actual.substr(0, bloque_actual.length() - 1) << endl;
                                archivoBloque.close();
                            }

                            bloque_actual = "";
                            suma_bloque = 0;
                            contador_bloque = 0;
                            b++;
                        }
                    }
                }
            }
        }

        if (!bloque_actual.empty())
        {
            string nombreBloque = carpetaBloques + "\\bloque" + to_string(b) + ".txt";
            archivoBloques << "bloque" + to_string(b) + "|" + to_string(suma_bloque) + "\n";

            ofstream archivoBloque(nombreBloque);
            if (archivoBloque.is_open())
            {
                archivoBloque << to_string(suma_bloque) + "|" + bloque_actual.substr(0, bloque_actual.length() - 1) << endl;
                archivoBloque.close();
            }
        }

        caracteristicasDisco();
        archivoBloques.close();
    }

    void guardarCaracteristicasDisco()
    {
        ofstream archivo("caracteristicasDisco.txt");
        string linea = to_string(tamanioSector) + "-" + to_string(sectxBloque);
        linea += "-" + to_string(num_platos) + "-" + to_string(num_caras) + "-" + to_string(num_pistas) + "-" + to_string(num_sectores);
        archivo << linea;
        archivo.close();
    }

   static void mostrarCaracteristicasDisco()
    {
        ifstream archivo("caracteristicasDisco.txt");
        string linea;
        stringstream ss;
        getline(archivo, linea);
        ss << linea;
        int tamanioSector, sectxBloque, num_platos, num_caras, num_pistas, num_sectores;
        char delim = '-';
        string token;

        getline(ss, token, delim);
            tamanioSector = stoi(token);
        getline(ss, token, delim);
            sectxBloque = stoi(token);
        getline(ss, token, delim);
            num_platos = stoi(token);
        getline(ss, token, delim);
            num_caras = stoi(token);
        getline(ss, token, delim);
            num_pistas = stoi(token);
        getline(ss, token, delim);
            num_sectores = stoi(token);
        
        cout<<"Tamaño del sector: "<<tamanioSector<<"/t";
        cout<<"Sectores por bloque: "<<sectxBloque<<"/t";
        cout<<"Número de platos: "<<num_platos<<"/t";
        cout<<"Número de caras: "<<num_caras<<"/t";
        cout<<"Número de pistas: "<<num_pistas<<"/t";
        cout<<"Número de sectores: "<<num_sectores<<"/t";
        archivo.close();
    }

};



class DatabaseManagementSystem {
private:
    
    
    FileManager fileManager;

    BufferManager BfManger;
    diskSpaceManager diskManager;

    


public:

   DatabaseManagementSystem(int frames = 3, char policy = 'C') 
        : BfManger(frames, policy), fileManager(), diskManager(){
        // Inicialización adicional si es necesaria
        cout << "=====================database===========================================" << endl;
    }
    
    void cargarBloque(int BloqueID, char mode){
        if(diskManager.BuscarBloque(BloqueID)){
            BfManger.loadPageFromDisk(diskManager.getPage(), mode);
            BfManger.printAtribsPage(BloqueID);
        }
        
    }
    
    
    void addRegistrosBloque(int pageID, const std::string& archivo, int cantidad) {
        std::ifstream archivoStream(archivo);

        if (!archivoStream.is_open()) {
            std::cerr << "Error abriendo el archivo " << archivo << std::endl;
            return;
        }

        std::string nombreEsq = archivo.substr(0, archivo.size() - 4);
        std::string esquema = RWesquema::extraerEsquema(nombreEsq);

        // Saltar la primera línea del archivo CSV (cabecera)
        std::string linea;
        std::getline(archivoStream, linea);

        int registrosInsertados = 0;

        while (registrosInsertados < cantidad) {
            std::string reg = RegistroFijo::guardarRegDesdeCsv(archivoStream, esquema);

            if(!BfManger.hayEspaciosVacios(pageID)){
                
                BfManger.setContentInFreeList(pageID, reg);
                registrosInsertados++;
                continue;
                
            }

            if (!reg.empty() && BfManger.setRecord(pageID, reg)) {
                std::cout << "Registro añadido correctamente" << std::endl;
                registrosInsertados++;
            } else {
                std::cout << "No se pudo añadir el registro, página llena o error." << std::endl;
                break;  // Detenerse si la página está llena
            }
        }

        archivoStream.close();

        if (registrosInsertados < cantidad) {
            std::cout << "Se añadieron solo " << registrosInsertados << " registros de " << cantidad << " solicitados." << std::endl;
        } else {
            std::cout << "Se añadieron los " << cantidad << " registros solicitados." << std::endl;
        }
    }


    void MostrarPage(int pageID) {
        BfManger.printAtribsPage(pageID);
    }

    void guardarEnBloque(int pageID){
        if(BfManger.isPageDir(pageID)){
            Page page = BfManger.rescuePage(pageID);
        }
    }

    void savePage(int pageID){
        if(BfManger.isPageDir(pageID)){
            Page page = BfManger.rescuePage(pageID);
            diskManager.savePageInBlock(page);
            BfManger.killProcess(pageID);
        }
    }

    void showBloackAndPage(int pageID){
        MostrarPage(pageID);
        diskManager.showBlock(pageID);
    }

    void deleteReg(int pageID, int regID){
        BfManger.deleteReg(pageID, regID);
    }
};



class Menu
{
public:

    DatabaseManagementSystem pruev;
    
    Menu() {
        cout << "Menu constructor called" << endl;
        // Inicialización explícita de pruev
        //pruev = DatabaseManagementSystem(); // Llama al constructor de DatabaseManagementSystem
    }

    void mostrar()
    {

        int opcion;
        do
        {
            std::cout << "\n--- Menú ---" << std::endl;
            std::cout << "1. Agregar esquema" << std::endl;
            std::cout << "2. Agregar esquema desde csv" << std::endl;
            std::cout << "3. Agregar datos por teclado a esquema" << std::endl;
            std::cout << "4. cargar bloque" << std::endl;
            std::cout << "5. agregar registros a pagina" << std::endl;
            std::cout << "6. guardar page" << std::endl;
            std::cout << "7. Mostrar page" << std::endl;
            std::cout << "8. gestionar disco" << std::endl;
            std::cout << "9. Mostrar page y bloque" << std::endl;
            std::cout << "10. Eliminar Registro"<<endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:
                agregarEsquema();
                break;
            case 2:
                agregarEsquemaDesdeCSV();
                break;
            case 3:
                agregarDatosPorTeclado();
                break;
            case 4:
                CargarBloque();
                break;
            case 5:
                añadirRegistrosCSV();
                break;
            case 6:
                guardarPage();
                break;
            case 7:
                mostrarPage();
                break;
                
            case 8:
                crearDisco();
                break;
                
            case 9:
                mostrarPageBlock();
                break;
            case 10:
                borrarRegistro();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 8);
    }

    void agregarEsquema()
    {
        system("cls");
        std::string nombreTabla;
        std::cout << "Ingrese el nombre de la tabla: ";
        std::cin >> nombreTabla;
        RWesquema::IngresarEsquema(nombreTabla);
    }

    void agregarEsquemaDesdeCSV()
    {
        system("cls");
        std::string nombreTabla;
        std::cout << "Ingrese el nombre del archivo: ";
        std::cin >> nombreTabla;
        RWesquema::GuardarEsquemaDesdeArchivo(nombreTabla);
    }

    void agregarDatosPorTeclado()
    {
        std::string nombreTabla;
        std::cout << "Ingrese el nombre de la tabla: ";
        std::cin >> nombreTabla;
        RegistroFijo::guardarRegDesdeTeclado(nombreTabla);
    }

    void CargarBloque()
    {
        int BloqueID;
        cout << "Ingrese el nombre del Bloque: ";
        cin >> BloqueID;
        cout<<"Ingrese el modo de carga (W/R): ";
        char modo;
        cin>>modo;
        pruev.cargarBloque(BloqueID, modo);
       //pruev.ingresarRegistroFijosDesdeCsv(nombreTabla,2);
    }

    void añadirRegistrosCSV()
    {
        system("cls");
        int pageID, cantidad;
        std::string nombreTabla="Titanic.csv";
        std::cout << "en que pagina desea añadir los registros:";
        cin>>pageID;
        cout<<"Cuantos registros desea añadir: ";
        cin>>cantidad;
        pruev.addRegistrosBloque(pageID, nombreTabla, cantidad);
    }

    void guardarPage()
    {
        system("cls");
        int pageID;
        std::cout << "Ingrese la pagina a guardar: ";
        std::cin >> pageID;
       pruev.savePage(pageID);
    }

    void mostrarPage()
    {
        system("cls");
        int pageID;
        std::cout << "Ingrese la pagina a mostrar: ";
        std::cin >> pageID;
        pruev.MostrarPage(pageID);
    }

    void mostrarPageBlock()
    {
        system("cls");
        int pageID;
        std::cout << "Ingrese la pagina y bloque a mostrar: ";
        std::cin >> pageID;
        pruev.showBloackAndPage(pageID);
    }

    void crearDisco()
    {
        int caso;
        std::cout << "1. Disco predefinido" << std::endl;
        std::cout << "2. Crear disco por teclado" << std::endl;
        std::cout << "3. mostrar caracteristicas del disco" << std::endl;
        std::cin >> caso;
        system("cls");

        if (caso == 1)
        {
            SimuladorDisco disco;
            cout<<"gaa";
        }
        if (caso == 2)
        {
            int num_platos, num_caras, num_pistas, num_sectores, tamanioSector, sectxBloque;

            cout << "Ingrese el número de platos: "; cin >> num_platos;
            cout << "Ingrese el número de caras por plato: "; cin >> num_caras;
            cout << "Ingrese el número de pistas por cara: "; cin >> num_pistas;
            cout << "Ingrese el número de sectores por pista: "; cin >> num_sectores;
            cout << "Ingrese el tamaño del sector (en bytes): ";cin >> tamanioSector;
            cout << "Ingrese el número de sectores por bloque: ";cin >> sectxBloque;

            // Crear el objeto SimuladorDisco con los valores ingresados
            SimuladorDisco disco(num_platos, num_caras, num_pistas, num_sectores, tamanioSector, sectxBloque);

        }
        if (caso == 3)
        {
            SimuladorDisco::mostrarCaracteristicasDisco();
        }
    }

    void borrarRegistro()
    {
        int pageID, regID;
        cout << "Ingrese el ID de la página: ";
        cin >> pageID;
        cout << "Ingrese el ID del registro: ";
        cin >> regID;
        pruev.deleteReg(pageID, regID);
    }

};



int main() {
    
  Menu menu;
  menu.mostrar();
 
    return 0;
}