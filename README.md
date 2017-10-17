AzulPatcher4600
===================

Lilu plugin for haswell hd4600 mobile igpus  

This seems to be noticeably faster than clover kext patching even when injected with Clover.

### OS Compatibility:
Yosemite to High Sierra

#### Tested on:
- Lenovo Y50-70 FHD
- Lenovo Thinkpad T440p
- hasee Z7M-i78172D1 (and other laptops based on the Clevo CP65S / Sager NP8650 / P650SA)

#### Features
- Patches HDMI audio
- Applies the 9MB cursor patch with 2 ports only
- Disables the unused 0204 port (Fixes graphic lockups on some laptops)

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu)
- [RehabMan](https://github.com/RehabMan) for the patch
