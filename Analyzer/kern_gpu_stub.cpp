//
//  kern_gpu_stub.cpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#include "kern_gpu_stub.hpp"

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/acpi/IOACPITypes.h>
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
    IOPCIDevice *pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if ( pciDevice == NULL )
    {
        DBGLOG("gpu_stub", "probe: pciDevice is NULL - aborting");
        return nullptr;
    }
    
    DBGLOG("gpu_stub", "probe: obtaining pci device model");
    
    OSString *modelProp = OSDynamicCast(OSString, pciDevice->getProperty("model"));
    
    if ( modelProp == NULL ) DBGLOG("gpu_stub", "probe: failed to obtain pci device model");
    else {
        const char *model = modelProp->getCStringNoCopy();
        
        DBGLOG(
            "gpu_stub",
            "probe: obtained model: %s",
            model != NULL ? model : "UNKNOWN");
    }
   
    DBGLOG("gpu_stub", "probe: attempting to obtain device and vendor ids");
    
    // This actually returns both IDs, in a single integer.
    //
    // So we extract the first 4 hex values as follows:
    //      - vendor ID: bitwise AND with '0xFFFF' (65535)
    //      - device ID: right-shift 16 times, alongisde bitwise AND with '0xFFFF'
    uint32_t ids = pciDevice->configRead32(kIOPCIConfigurationOffsetVendorID);
    uint32_t deviceId = ids >> 16 & 0xFFFF;
    uint32_t vendorId = ids & 0xFFFF;
    
    SYSLOG(
        "gpu_stub",
        "probe: Device ID: 0x%x - Vendor ID: 0x%x",
        deviceId, vendorId);
    
    const OSSymbol *locationSymbol = pciDevice->copyLocation(gIOACPIPlane);
    
    if ( locationSymbol != NULL ) {
        const char *path = locationSymbol->getCStringNoCopy();
        
        SYSLOG(
            "gpu_stub",
            "probe: location in plane: %s",
            path != NULL ? path : "UNKNOWN");
    } else
        DBGLOG(
            "gpu_stub",
            "probe: failed to obtain location in plane - ignoring");
    
    OSSafeReleaseNULL(locationSymbol);
    
    DBGLOG("gpu_stub", "probe: finalized probe, letting real driver take over");

    // Must mark as failed so that the original driver can take over.
    return nullptr;
} // GPUStub::probe()

bool GPUStub::start(IOService *provider)
{
    SYSLOG("gpu_stub", "start: shouldn't be enabled - aborting");
    return false;
} // GPUStub::start()
