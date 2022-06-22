//
//  kern_intel_stub.cpp
//  Analyzer
//
//  Created by kernel-hubby on 22. 6. 2022..
//

#include "kern_intel_stub.hpp"

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
// I can probe the device and read the model, device id,
// vendor id, PCI locations, etc.

// Intel GPU device stub
OSDefineMetaClassAndStructors(IntelStub, IOService);

IOService *IntelStub::probe(IOService *provider, SInt32 *score)
{
    IOPCIDevice *pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if ( pciDevice == NULL )
    {
        DBGLOG("intel_stub", "probe: pciDevice is NULL - aborting");
        return nullptr;
    }
    
    DBGLOG("intel_stub", "probe: obtaining pci device model");
    
    OSString *modelProp = OSDynamicCast(OSString, pciDevice->getProperty("model"));
    
    if ( modelProp == NULL ) DBGLOG("intel_stub", "probe: failed to obtain pci device model");
    else {
        const char *model = modelProp->getCStringNoCopy();
        
        DBGLOG(
            "intel_stub",
            "probe: obtained model: %s",
            model ? model : "UNKNOWN");
    }
   
    DBGLOG("intel_stub", "probe: attempting to obtain device and vendor ids");
    
    // This actually returns both IDs, in a single integer.
    //
    // So we extract the first 4 hex values as follows:
    //      - vendor ID: bitwise AND with '0xFFFF' (65535)
    //      - device ID: right-shift 16 times, alongisde bitwise AND with '0xFFFF'
    uint32_t ids = pciDevice->configRead32(kIOPCIConfigurationOffsetVendorID);
    uint32_t deviceId = ids >> 16 & 0xFFFF;
    uint32_t vendorId = ids & 0xFFFF;
    
    SYSLOG(
        "intel_stub",
        "probe: Device ID: 0x%x - Vendor ID: 0x%x",
        deviceId, vendorId);
    
    const OSSymbol *locationSymbol = pciDevice->copyLocation(gIOACPIPlane);
    const char *path = locationSymbol->getCStringNoCopy();
    
    DBGLOG(
        "intel_stub",
        "probe: location in plane: %s",
        path ? path : "UNKNOWN");
    
    locationSymbol->release();
    
    DBGLOG("intel_stub", "probe: finalized probe, letting real driver take over");

    // Must mark as failed so that the original driver can take over.
    return nullptr;
} // IntelStub::probe()

bool IntelStub::start(IOService *provider)
{
    SYSLOG("intel_stub", "start: shouldn't be enabled - aborting");
    return false;
} // IntelStub::start()
