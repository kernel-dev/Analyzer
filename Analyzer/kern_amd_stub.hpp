//
//  kern_device_stub.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_amd_stub_h
#define kern_amd_stub_h

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>

class EXPORT AMDStub : public IOService {
    OSDeclareDefaultStructors(AMDStub);
public:
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
};

#endif /* kern_amd_stub.h */
