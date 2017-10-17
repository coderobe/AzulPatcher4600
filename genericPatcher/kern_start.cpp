//
//  kern_start.cpp
//  genericPatcher
//
//  Copyright © 2017 coderobe. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "kern_patchset.hpp"

static PatchSet patchset;

static const char *bootargOff[] {
	"-cdrpatchoff"
};

static const char *bootargDebug[] {
	"-cdrpatchdbg"
};

static const char *bootargBeta[] {
	"-cdrpatchbeta"
};

PluginConfiguration ADDPR(config) {
	xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal | LiluAPI::AllowInstallerRecovery,
	bootargOff,
	arrsize(bootargOff),
	bootargDebug,
	arrsize(bootargDebug),
	bootargBeta,
	arrsize(bootargBeta),
	KernelVersion::Yosemite,
	KernelVersion::HighSierra,
	[]() {
		patchset.init();
	}
};





