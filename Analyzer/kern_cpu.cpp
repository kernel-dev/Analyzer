//
//  kern_cpuid.cpp
//  Analyzer
//
//  Created by kernel-hubby on 22. 6. 2022..
//

#include "kern_cpu.hpp"

#include <Headers/kern_util.hpp>
#include <i386/cpuid.h>

void cpuInfo() {
    DBGLOG("analyzer", "cpu: obtaining data via cpuid_info()");
    
    i386_cpu_info_t *cpu = cpuid_info();
    
    if ( cpu == NULL ) {
        DBGLOG(
            "anal_stub",
            "cpu: failed to obtain CPUID data ???");
        
        return;
    }
    
    SYSLOG(
        "anal_stub",
        "cpu: Model: %s",
        cpu->cpuid_brand_string);
    
    SYSLOG(
        "anal_stub",
        "cpu: Cores: %d - Threads: %d",
        cpu->core_count, cpu->thread_count);
    
    char features[512];
    
    SYSLOG(
        "anal_stub",
        "cpu: Features: %s",
           cpuid_get_feature_names(cpuid_features(), features, sizeof(features)));
    
    SYSLOG(
        "anal_stub",
        "cpu: leaf7_features: %s",
        cpuid_get_feature_names(cpuid_leaf7_features(), features, sizeof(features)));
}
