// -*- lsst-c++ -*-
//
//  kern_analyzer.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include "kern_analyzer.hpp"

#include <Headers/kern_util.hpp>
#include <i386/cpuid.h>

SystemAnalyzer *SystemAnalyzer::callbackAnal;

void SystemAnalyzer::init() {
    callbackAnal = this;
    
    SYSLOG("analyzer", "init: successfully initialised KEXT – fetching CPU info");
    
    callbackAnal->cpu_info();
}

void SystemAnalyzer::deinit() {
    SYSLOG("analyzer", "deinit: successfully deinitialised KEXT");
}

void SystemAnalyzer::cpu_info() {
    DBGLOG("anal_cpu", "obtaining data via cpuid_info()");
    
    i386_cpu_info_t *cpu = cpuid_info();
    
    if (!cpu) {
        DBGLOG(
            "anal_cpu",
            "failed to obtain CPUID data ???");
        
        return;
    }
    
    SYSLOG(
        "anal_cpu",
        "Model: %s",
        cpu->cpuid_brand_string);
    
    SYSLOG(
        "anal_cpu",
        "Cores: %d - Threads: %d",
        cpu->core_count, cpu->thread_count);
    
    char features[512];
    
    SYSLOG(
        "anal_cpuid",
        "Features: %s",
           cpuid_get_feature_names(cpu->cpuid_features, features, sizeof(features)));
    
    SYSLOG(
        "anal_cpu",
        "leaf7_features: %s",
        cpuid_get_feature_names(cpu->cpuid_leaf7_features, features, sizeof(features)));
}
