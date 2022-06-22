// -*- lsst-c++ -*-
//
//  kern_analyzer.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include "kern_analyzer.hpp"
#include "kern_cpu.hpp"

void SystemAnalyzer::init() {
    cpuInfo();
}

void SystemAnalyzer::deinit() {}
