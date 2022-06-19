// -*- lsst-c++ -*-
//
//  kern_analyzer.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOService.h>
#include <libkern/c++/OSDictionary.h>
#include <libkern/c++/OSString.h>
#include <libkern/sysctl.h>
#include <Headers/kern_util.hpp>
#include "kern_data.hpp"
#include "kern_analyzer.hpp"

Analyzer *Analyzer::callbackAnalyzer;

struct GPU *Analyzer::getGpuInfo()
{
    const OSSymbol *keyClass  = OSSymbol::withCString("IOPCIClassMatch");
    const OSString *valClass  = OSString::withCString("0x03000000&0xff000000");

    OSIterator *iter = IOService::getMatchingServices(IOService::propertyMatching(keyClass, valClass));
    IOPCIDevice *device;
    
    while ( (device = (IOPCIDevice *)iter->getNextObject()) )
    {
//        uint32_t deviceId;
//        uint32_t vendorId;
//
//        // Device and Vendor ID
//        device->ConfigurationRead32(kIOPCIConfigurationOffsetDeviceID, &deviceId);
//        device->ConfigurationRead32(kIOPCIConfigurationOffsetVendorID, &vendorId);
        
        // ACPI Plane
        UInt8 dev  = device->getDeviceNumber();
        UInt8 func = device->getFunctionNumber();
        const char *childLocation = device->getChildEntry(gIOServicePlane)->getLocation();
        
        SYSLOG(
            "analyzer",
            "LOCATION VALUE: %s",
            childLocation);
        
        SYSLOG(
            "analyzer",
            "DEV NUM: %d ",
            dev);
        
        SYSLOG(
            "analyzer",
            "FUNC NUM: %d",
            func);
        
    }
    
    OSSafeReleaseNULL(iter);
    
    return NULL;
};
