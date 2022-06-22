//
//  kern_nv_stub.hpp
//  Analyzer
//
//  Created by kernel-hubby on 22. 6. 2022..
//

#ifndef kern_nv_stub_hpp
#define kern_nv_stub_hpp

#include <Headers/kern_util.hpp>
#include <IOKit/IOService.h>

class EXPORT NVStub : public IOService {
    OSDeclareDefaultStructors(NVStub);
public:
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
};

#endif /* kern_nv_stub_hpp */
