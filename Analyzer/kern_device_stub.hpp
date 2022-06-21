//
//  kern_device_stub.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef KERN_DEVICE_STUB_H
#define KERN_DEVICE_STUB_H

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>

class EXPORT PCIDeviceStub : public IOService {
    OSDeclareDefaultStructors(PCIDeviceStub);
public:
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
};

#endif /* kern_device_stub.h */
