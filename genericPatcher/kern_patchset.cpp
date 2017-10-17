//
//  kern_patchset.cpp
//
//  Copyright © 2017 coderobe. All rights reserved.
//

#include <Headers/kern_api.hpp>
#include <Headers/kern_util.hpp>
#include <Library/LegacyIOService.h>

#include <mach/vm_map.h>
#include <IOKit/IORegistryEntry.h>

#include "kern_patchset.hpp"

KernelVersion KernelCheck = getKernelVersion();

static const char* kextPaths[] {
    "/System/Library/Extensions/AppleIntelFramebufferAzul.kext/Contents/MacOS/AppleIntelFramebufferAzul",
};

static KernelPatcher::KextInfo kextList[] {
    { "com.apple.driver.AppleIntelFramebufferAzul", &kextPaths[0], 1, {true, true}, {}, KernelPatcher::KextInfo::Unloaded },
};

template <typename T,unsigned S>
inline unsigned arraysize(const T (&v)[S]) { return S; }
static size_t kextListSize = arraysize(kextList);

bool PatchSet::init() {
	LiluAPI::Error error = lilu.onKextLoad(kextList, kextListSize,
    [](void* user, KernelPatcher& patcher, size_t index, mach_vm_address_t address, size_t size) {
        PatchSet* patchset = static_cast<PatchSet*>(user);
		patchset->processKext(patcher, index, address, size);
	}, this);
	
	if(error != LiluAPI::Error::NoError) {
		SYSLOG("coderobe.AzulPatcher4600", "failed to register onPatcherLoad method %d", error);
		return false;
	}
	
	return true;
}

void PatchSet::deinit() {
}

void PatchSet::processKext(KernelPatcher& patcher, size_t index, mach_vm_address_t address, size_t size){
    if(progressState != ProcessingState::EverythingDone) {
        for(size_t i = 0; i < kextListSize; i++) {
            if(kextList[i].loadIndex == index) {
                if(!(progressState & ProcessingState::EverythingDone) && !strcmp(kextList[i].id, kextList[0].id)) {
                    SYSLOG("coderobe.AzulPatcher4600", "found %s", kextList[i].id);

                    // Disable port 0204
                    const uint8_t find[]    = {0x02, 0x04, 0x09, 0x00, 0x00, 0x04, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00};
                    const uint8_t replace[] = {0xff, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
                    KextPatch kext_patch {
                        {&kextList[i], find, replace, sizeof(find), 2},
                        KernelVersion::Yosemite, KernelVersion::HighSierra
                    };
                    applyPatches(patcher, index, &kext_patch, 1);
                    SYSLOG("coderobe.AzulPatcher4600", "disabled port 0204");

                    // 9MB cursor bytes, 2 ports only
                    const uint8_t find1[]    = {0x06, 0x00, 0x26, 0x0a, 0x01, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x60, 0x00};
                    const uint8_t replace1[] = {0x06, 0x00, 0x26, 0x0a, 0x01, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x90, 0x00};
                    KextPatch kext_patch1 {
                        {&kextList[i], find1, replace1, sizeof(find1), 2},
                        KernelVersion::Yosemite, KernelVersion::HighSierra
                    };
                    applyPatches(patcher, index, &kext_patch1, 1);
                    SYSLOG("coderobe.AzulPatcher4600", "9MB cursor byte patch (2 ports only) applied");

                    // HDMI audio
                    const uint8_t find2[]    = {0x01, 0x05, 0x09, 0x00, 0x00, 0x04, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00};
                    const uint8_t replace2[] = {0x01, 0x05, 0x12, 0x00, 0x00, 0x08, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00};
                    KextPatch kext_patch2 {
                        {&kextList[i], find2, replace2, sizeof(find2), 2},
                        KernelVersion::Yosemite, KernelVersion::HighSierra
                    };
                    applyPatches(patcher, index, &kext_patch2, 1);
                    SYSLOG("coderobe.AzulPatcher4600", "HDMI audio patch applied");
                    progressState |= ProcessingState::EverythingDone;
                }
            }
        }
    }
	patcher.clearError();
}

void PatchSet::applyPatches(KernelPatcher& patcher, size_t index, const KextPatch* patches, size_t patchNum) {
    for (size_t p = 0; p < patchNum; p++) {
        auto &patch = patches[p];
        if (patch.patch.kext->loadIndex == index) {
            if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
                SYSLOG("coderobe.AzulPatcher4600", "patching %s (%ld/%ld)...", patch.patch.kext->id, p+1, patchNum);
                patcher.applyLookupPatch(&patch.patch);
                patcher.clearError();
            }
        }
    }
}

