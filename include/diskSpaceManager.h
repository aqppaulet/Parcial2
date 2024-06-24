#ifndef DISKSPACEMANAGER_H
#define DISKSPACEMANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include "Page.h"  // Asegúrate de tener Page.h en el mismo directorio o ajusta el include según corresponda
#include "../RWBLOQUE.h"

/*Kenny Borja */


using namespace std;

class diskSpaceManager {
public:
    Page page;

    bool BuscarBloque(int id);
    void savePageInBlock(Page page);
    void SetPageContent(int id);
    void printActualPage();
    Page getPage();
    void setPage(Page page);
    void showBlock(int BlockID);
    
};

#endif  // DISKSPACEMANAGER_H
