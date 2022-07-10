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
    "-debugmesenpai"
};

static const char *bootargBeta[] {
    "-analbeta"
};

PluginConfiguration ADDPR(config)
{
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal | LiluAPI::AllowInstallerRecovery,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Tiger,
    KernelVersion::Ventura,
    []() {
        // FIXME: Lilu attempting to request access
        // seems to return Error::Disabled, or Error::TooLate,
        // because I honestly have no idea which of the two
        // corresponds to error code 6.
        analyzer.init();
    }
};
