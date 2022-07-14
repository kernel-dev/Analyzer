// -*- lsst-c++ -*-
//
//  kern_pci_utils.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include "kern_pci_utils.hpp"

#include <cstring>
#include <IOKit/IOLib.h>
#include <libkern/libkern.h>

bool strstr(char *big, const char *comp)
{
    if (strlen(comp) < 1) return false;
    
    for (int i = 0; i < strlen(big); i++) {
        if ((i + strlen(comp)) > strlen(big)) break;
        
        // Thanks, Oofer.
        if (strncmp(&big[i], comp, strlen(comp)) == 0) return true;
    }
    
    return false;
}

int hex2dec(char *hexVal)
{
  int x;

  x = (int)strtol(hexVal, NULL, 16);

  return x;
}

void constructPaths(const char *path, char *paths[2])
{
  char *acpiPath  = (char *)IOMalloc(strlen(path) / 2);
  char *pciPath   = (char *)IOMalloc(strlen(path) / 2);
  char *tempPath  = (char *)IOMalloc(strlen(path) + 1);
  char *found     = (char *)IOMalloc(256);

  memcpy(tempPath, path, strlen(path) + 1);

    while ( (found = strsep(&tempPath, "@")) != NULL ) {
        if (strstr(found, "_SB")) {
            void(strsep(&found, "/"));
            void(strsep(&found, "/"));
            
            snprintf(acpiPath, 256, "\\_SB.%s", found);
            continue;
        }

        int tempPci = hex2dec(strsep(&found, "/"));
        char *tempAcpi = strsep(&found, "/");

        if (strlen(pciPath) < 1) {
            snprintf(
                pciPath,
                256,
                "PciRoot(0x%x)",
                tempPci & 0xFFFF);
        } else {
            snprintf(
                pciPath,
                256,
                "%s/Pci(0x%x,0x%x)",
                pciPath, tempPci >> 16 & 0xFFFF, tempPci & 0xFFFF);
        }

        if (tempAcpi)
            snprintf(acpiPath, 256, "%s.%s", acpiPath, tempAcpi);
    }

    if (strlen(acpiPath) > 0)
        paths[0] = acpiPath;
    else {
        IOFree(acpiPath, strlen(path) / 2);
    }
  
    if (strlen(pciPath) > 0)
        paths[1] = pciPath;
    else {
        IOFree(pciPath, strlen(path) / 2);
    }
    
    IOFree(tempPath, strlen(path) + 1);
    IOFree(found, 256);
}
