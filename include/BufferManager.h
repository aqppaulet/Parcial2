#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

/* Jose Paredes*/

#include <iostream>

#include "BufferPool.h"

class BufferManager {
   private:
    int numFrames;
    BufferPool bpool;
    char policy;

   public:
    // Constructor

    BufferManager(int numFrames, char  policy);

    // Métodos
    void loadPageFromDisk(Page page, char _mode);
    void killProcess(int pageID);
    void savePageToDisk(int pageID);
    void updatePage(int pageID);
    void deletePage(int pageID);
    void printTableFrame();
    void pinPage(int pageID);
    void unpinPage(int pageID);

    bool setRecord(int pageID, string record);
    void printAtribsPage(int pageID);

    bool hayEspaciosVacios(int pageID);
    void setContentInFreeList(int pageID, string record);

    Page rescuePage(int PageID);
    bool isPageDir(int pageID);
    void deleteReg(int BlockID, int pos);
};

#endif  // BUFFERMANAGER_H
