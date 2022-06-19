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

struct GPU *SystemAnalyzer::getGpuInfo()
{
    DBGLOG(
        "analyzer",
        "Instantiating dictionary with IOPCIClassMatch: 0x03000000&0xff000000");
    
    OSObject *val = OSString::withCString("0x03000000&0xff000000");
    
    if ( val == NULL ) {
        DBGLOG(
            "analyzer",
            "Unable to instantiate PCI class match value - val=NULL");
        
        return NULL;
    }
    
    OSDictionary *dict = OSDictionary::withCapacity(1);

    dict->setObject(kIOPCIClassMatchKey, val);
    
    if ( dict == NULL ) {
        OSSafeReleaseNULL(val);
        
        DBGLOG(
            "analyzer",
            "Failed to instantiate dictionary for match - dict=NULL");
        
        return NULL;
    }

    OSIterator *iter = IOService::getMatchingServices(dict);
    
    if ( iter == NULL ) {
        OSSafeReleaseNULL(dict);
        OSSafeReleaseNULL(val);
        
        DBGLOG(
            "analyzer",
            "Failed to obtain iterator from IOPCIClassMatch match - iter=NULL");
        
        return NULL;
    }
    
    DBGLOG(
        "analyzer",
        "Successfully obtained iterator from IOPCIClassMatch match - Iterator: %d",
        iter);
    
    IOPCIDevice *device = NULL;
    
    while ( (device = OSDynamicCast(IOPCIDevice, iter->getNextObject())) != NULL )
    {
        // Device and Vendor ID
        uint32_t deviceId = device->configRead32(kIOPCIConfigurationOffsetDeviceID);
        uint32_t vendorId = device->configRead32(kIOPCIConfigurationOffsetVendorID);
        
        DBGLOG(
            "analyzer",
            "Device ID: %d - Vendor ID: %d",
            deviceId, vendorId);
        
        // ACPI Plane
        UInt8 dev  = device->getDeviceNumber();
        UInt8 func = device->getFunctionNumber();
        const char *childLocation = device->getChildEntry(gIOServicePlane)->getLocation();
        
        DBGLOG(
            "analyzer",
            "LOCATION VALUE: %s - DEV NUM: %d - FUNC NUM: %d",
            childLocation, dev, func);
        
    }
    
    OSSafeReleaseNULL(iter);
    OSSafeReleaseNULL(dict);
    OSSafeReleaseNULL(val);
    
    return NULL;
};
