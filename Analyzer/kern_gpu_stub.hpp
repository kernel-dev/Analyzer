//
//  kern_gpu_stub.hpp
//  Analyzer
//
//  Copyright © 2022 Kernel. All rights reserved.
//

#ifndef kern_gpu_stub_hpp
#define kern_gpu_stub_hpp

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>

class EXPORT GPUStub : public IOService {
    OSDeclareDefaultStructors(GPUStub);
public:
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
};


#endif /* kern_gpu_stub_hpp */
