// -*- lsst-c++ -*-
//
//  kern_gpu_stub.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include "kern_gpu_stub.hpp"
#include "kern_pci_utils.hpp"

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/acpi/IOACPITypes.h>
#include <IOKit/IODeviceTreeSupport.h>
#include <IOKit/IOPlatformExpert.h>
#include <libkern/libkern.h>

// Due to the limitations of Lilu, I had to take this
// approach in order to properly attach to a device
// and read data inherent to it.
//
// The current approach is that I have a PCI device stub
// in place, where the kernel will attach the PCI device
// (hopefully the GPU) to that stub, and from there-on,
// the device can be probed and I can read the model, device id,
// vendor id, PCI locations, etc.

// GPU device stub
OSDefineMetaClassAndStructors(GPUStub, IOService);

IOService *GPUStub::probe(IOService *provider, SInt32 *score)
{
    auto pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!pciDevice)
    {
        DBGLOG("gpu_stub", "probe: pciDevice is NULL - aborting");
        return nullptr;
    }
    
    DBGLOG("gpu_stub", "probe: attempting to obtain pci device model");
    
    auto modelProp = pciDevice->copyProperty("model");
    
    if (!modelProp) DBGLOG("gpu_stub", "probe: failed to obtain pci device model property");
    else {
        auto modelData = OSDynamicCast(OSData, modelProp);
        
        if (!modelData) {
            DBGLOG("gpu_stub", "probe: failed to cast OSObject -> OSData - ignoring");
        } else {
            auto bytes = modelData->getBytesNoCopy();
            
            if (!bytes) DBGLOG("gpu_stub", "probe: failed to get bytes for property");
            else {
                unsigned int length = modelData->getLength();
                char *model = (char *)IOMalloc(length + 1);
                
                memcpy(model, bytes, length);
                model[length] = '\0';
                
                SYSLOG(
                    "gpu_stub",
                    "probe: pci device model: %s",
                    model);
                
                IOFree(model, length + 1);
            }
        }
        
        modelProp->release();
    }
   
    DBGLOG("gpu_stub", "probe: attempting to obtain device and vendor ids");
    
    // This actually returns both IDs, in a single integer.
    //
    // So we extract the first 4 hex values as follows:
    //      - vendor ID: bitwise AND with '0xFFFF' (65535)
    //      - device ID: right-shift 16 times, alongside bitwise AND with '0xFFFF'
    uint32_t ids = pciDevice->configRead32(kIOPCIConfigurationOffsetVendorID);
    uint32_t deviceId = ids >> 16 & 0xFFFF;
    uint32_t vendorId = ids & 0xFFFF;
    
    SYSLOG(
        "gpu_stub",
        "probe: Device ID: 0x%x - Vendor ID: 0x%x",
        deviceId, vendorId);
    
    DBGLOG("gpu_stub", "probe: copying device's acpi path");
    
    auto pathSymbol = pciDevice->copyProperty("acpi-path");
    
    if (!pathSymbol) DBGLOG("gpu_stub", "probe: acpi path is NULL – ignoring");
    else {
        auto pathString = OSDynamicCast(OSString, pathSymbol);
        
        if (!pathString) DBGLOG("gpu_stub", "probe: ACPIPATH - failed to cast OSObject -> OSString, ignoring");
        else {
            const char *path = pathString->getCStringNoCopy();
            static char *paths[2] = {};
            
            constructPaths(path, paths);
            
            if (paths[0]) {
                SYSLOG(
                    "gpu_stub",
                    "probe: acpi path: %s",
                    paths[0]);
                
                IOFree(paths[0], strlen(path) / 2);
            }
            
            if (paths[1]) {
                SYSLOG(
                    "gpu_stub",
                    "probe: pci path: %s",
                    paths[1]);
                
                IOFree(paths[1], strlen(path) / 2);
            }
        }
        
        pathSymbol->release();
    }
    
    auto parentBridge = OSDynamicCast(IOPCIDevice, pciDevice->getParentEntry(gIODTPlane));
    
    if (!parentBridge) {
        DBGLOG("gpu_stub", "probe: can't find the parent bridge's IOPCIDevice");
        return nullptr;
    }
    
    DBGLOG("gpu_stub", "probe: requesting parent bridge rescan");
    
    // Mark this device and the parent bridge as needing scanning, then trigger the rescan.
    //
    // Thank you, WhateverGreen devs, for saving my ass.
    pciDevice->kernelRequestProbe(kIOPCIProbeOptionNeedsScan);
    parentBridge->kernelRequestProbe(kIOPCIProbeOptionNeedsScan | kIOPCIProbeOptionDone);

    // Must mark as failed so that the original driver can take over.
    return nullptr;
} // GPUStub::probe()

bool GPUStub::start(IOService *provider)
{
    SYSLOG("gpu_stub", "start: shouldn't be enabled - aborting");
    return false;
} // GPUStub::start()
