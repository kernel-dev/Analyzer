// -*- lsst-c++ -*-
//
//  kern_analyzer.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_analyzer_h
#define kern_analyzer_h

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
     *          CPU(s), and prints it.
     */
    void                getCpuInfo();
};

#endif /* kern_analyzer_h */
