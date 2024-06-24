#include "../include/diskSpaceManager.h"


bool diskSpaceManager::BuscarBloque(int id) {
    ifstream archivo("bloques.txt");
    string linea;
    
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string parte;
        int idBloque;
        int espacio;
        getline(ss, parte, '|');
        idBloque = stoi(parte);
        getline(ss, parte, '|');
        espacio = stoi(parte);

        if (idBloque == id) {
            page.setPageId(idBloque);
            page.setSize(espacio);
            SetPageContent(idBloque);
            return true;
        }
    }
    return false;
}

void diskSpaceManager::savePageInBlock(Page page) {
    page.updateHeader();
    cout<<"================"<<page.getSize()<<endl;
    RWBloque::ActualizarBloques(page.getPageId(), page.getSize());
    RWBloque::ActualizarBloque(page.getPageId(),page.getName(),page.getCabeceraSpaciosPage(),page.getContent());
}

void diskSpaceManager::SetPageContent(int id) {
    ifstream archivo("disk//bloques//" + to_string(id) + ".txt");
    string linea;

    if (getline(archivo, linea)) {
        page.setName(linea);
        if (getline(archivo, linea)) {
            page.setCabeceraSpaciosPage(linea);

            if (getline(archivo, linea)) {
                page.setContent(linea);
            } else {
                page.setContent(""); // Asigna un valor por defecto si no hay tercera línea
            }
        } else {
            page.setCabeceraSpaciosPage(""); // Asigna un valor por defecto si no hay segunda línea
            page.setContent(""); // Asigna un valor por defecto si no hay tercera línea
        }
    } else {
        page.setContent(""); // Asigna un valor por defecto si no hay primera línea
        page.setCabeceraSpaciosPage(""); // Asigna un valor por defecto si no hay segunda línea
        page.setName(""); // Asigna un valor por defecto si no hay tercera línea
    }
}

void diskSpaceManager::printActualPage() {
    page.imprimirPage();
}

Page diskSpaceManager::getPage() {
    return page;
}

void diskSpaceManager::setPage(Page page) {
    this->page = page;
}

void diskSpaceManager::showBlock(int blockID) {
    BuscarBloque(blockID);
    printActualPage();
}
