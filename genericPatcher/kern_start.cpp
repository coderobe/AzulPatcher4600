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
	bootargOff,
	sizeof(bootargOff)/sizeof(bootargOff[0]),
	bootargDebug,
	sizeof(bootargDebug)/sizeof(bootargDebug[0]),
	bootargBeta,
	sizeof(bootargBeta)/sizeof(bootargBeta[0]),
	KernelVersion::Yosemite,
	KernelVersion::Sierra,
	[]() {
		patchset.init();
	}
};





