#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;
/*Kenny Borja */

class Page {
private:

  int Size;
  int pageID;
  

  string Name;
  string cabeceraSpaciosPage; int numRegistros;int tamanio; vector<int> posicionesvacias;
  string content;

public:
  Page();

  Page(int Size, string Name);

  void setAllAttributes(int Size, string Name, int pageID, string content);
  
  void setPageId(int pageId);
  void setSize(int size);

  void setName(string name);
  void setCabeceraSpaciosPage(string cabeceraSpaciosPage);
  void setContent(string content);
  
  int getSize();
  int getPageId();
  string getName();
  string getCabeceraSpaciosPage();
  string getContent();

  

  // Añade el contenido de un registro a la página si hay suficiente espacio
  bool addRecordInContent(string & record);

  // Elimina un registro de la página en el índice especificado
  bool deleteRecordInContent(int index);

  void imprimirPage();
  
  // Nueva  para verificar si el vector posicionesvacias está vacío
    bool isPosicionesVaciasEmpty();

    void SetRecordtInEmptyPoscition(string record);

    void updateHeader();
  

};

#endif  // PAGE_H
