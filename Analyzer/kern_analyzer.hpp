// -*- lsst-c++ -*-
//
//  kern_analyzer.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_analyzer_h
#define kern_analyzer_h

#include <sys/types.h>
#include <sys/sysctl.h>
#include "kern_data.hpp"

/**
 *  @brief An instance for handling, extracting and
 *         constructing CPU & GPU information.
 */
class SystemAnalyzer {
    
public:
    /**
     *  @brief  Obtains data, which conforms to
     *          the data defined in the `CPU`
     *          structure, of the current machine's
     *          CPU(s).
     *
     *  @returns Data about the CPU.
     */
    CPU                 getCpuInfo();
    
    /**
     *  @brief  Obtains data, which conforms to
     *          the data defined in the `GPU`
     *          structure, of the current machine's
     *          GPU(s).
     *
     *  @returns An array of the data.
     */
    GPU                 *getGpuInfo();

    /**
     * @brief   Replaces the ref value,
     *          provided by `sysctlbyname()`
     *          without needing to provide
     *          additional data.
     *
     * @param name  The path to the sysctl value (e.g. 'machdep.cpu.core_count')
     */
    template<typename T>
    T                   sysctlLookup(const char *name);
};

#endif /* kern_analyzer_h */
