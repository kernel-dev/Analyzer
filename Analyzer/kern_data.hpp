// -*- lsst-c++ -*-
//
//  kern_data.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_data_h
#define kern_data_h

/**
 *  @brief A simple representation of CPU data.
 *
 *  @member model       The model of this CPU.
 *  @member cores       The amount of physical cores this CPU has.
 *  @member threads     The amount of threads (virtual cores) this CPU has.
 *  @member features    The supported features (instruction sets)
 *                          this CPU has (x86-only.)
 *  @member codename    The codename of this CPU (µarch, to be precise.)
 */
struct CPU {
    char           *model;
    int            cores;
    int            threads;
    char           *codename;
    char           *features;
};

/**
 *  @brief A simple representation of (non-AGX) GPU data.
 *
 *  @member model       The model of this GPU device.
 *  @member devId       The device ID of this GPU device.
 *  @member venId       The vendor ID of this GPU device.
 *  @member acpiPath    The ACPI path of this GPU device.
 *  @member pciPath     The PCI path of this GPU device.
 *  @member codename    The codename of this GPU device.
 */
struct GPU {
    char           *model;
    int             devId;
    int             venId;
    char           *acpiPath;
    char           *pciPath;
    char           *codename;
};

/**
 *  @brief A full list of data to be displayed.
 *
 *  @member cpus     A pointer to a vector of CPU data
 *  @member gpus     A pointer to a vector of GPU data
 *  @member agx      A pointer to AGX data
 */
struct SystemData
{
    int             cpus;
    int             gpus;
};

#endif /* kern_data_h */
