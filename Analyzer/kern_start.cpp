// -*- lsst-c++ -*-
//
//  kern_start.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include "kern_analyzer.hpp"

static SystemAnalyzer analyzer;

static const char *bootargOff[] {
    "-analoff"
};

static const char *bootargDebug[] {
    "-analdbg"
};

static const char *bootargBeta[] {
    "-analbeta"
};

PluginConfiguration ADDPR(config)
{
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal |
    LiluAPI::AllowInstallerRecovery |
    LiluAPI::AllowSafeMode,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Tiger,
    KernelVersion::Ventura,
    []() {
        analyzer.getGpuInfo();
    }
};
