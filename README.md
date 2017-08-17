AzulPatcher4600
===================

Lilu plugin which fixes the ghost-port bootup lockups on haswell hd4600 mobile igpus  

This seems to be noticeably faster than clover kext patching even when injected with Clover.

#### Tested on:
- Lenovo Y50-70 FHD
- Lenovo Thinkpad T440p

#### Features
- Patches HDMI audio
- Applies the 9MB cursor patch with 2 ports only
- Disables the unused 0204 port

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu)
- [RehabMan](https://github.com/RehabMan) for the patch
