// -*- lsst-c++ -*-
//
//  kern_pci_utils.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_pci_utils_hpp
#define kern_pci_utils_hpp

void constructPaths(const char *path, char *paths[2]);
int hex2dec(char *hexVal);
bool strstr(char *big, const char *comp);

#endif /* kern_pci_utils_hpp */
