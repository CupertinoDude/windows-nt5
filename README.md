# Windows NT 5 (Windows XP / Windows Server 2003)
Windows NT is a proprietary graphical operating system produced by Microsoft and its first version was released on July 27, 1993. It is a processor-independent, multi-processor and multi-user operating system.

This edition of Windows NT 5 is a member of the Microsoft Windows NT family, beginning with Windows 2000 and ending with Windows Server 2003.

## Pre-Installation Preparations
- Extract source tree, in this guide we will assume `C:\NT`. 
- Remove the Read Only setting, including subfolders and files. 
- Create desktop shortcut for `%windir%\system32\cmd.exe /k C:\NT\tools\razzle.cmd free offline`
- Open razzle window using shortcut you created

## Building
- Performs clean rebuild of all components: `build /cZP`
- Builds only components that have changed since last clean build: `build /ZP`
- Mount RTM CD and execute `tools\missing.cmd` (optionally, perform this step for every SKU)
- Execute `tools\postbuild.cmd` (use `-sku:{sku}` if you want to process only specific one, expect `filechk` errors if you ignore this and have skipped on `missing.cmd` optional step)

## Generating ISO Files
Execute `tools\oscdimg.cmd {sku}` where `{sku}` is one of: 
- `srv` - Windows Server 2003 Standard Edition
- `sbs` - Windows Server 2003 Small Business Edition
- `ads` - Windows Server 2003 Enterprise Edition
- `dtc` - Windows Server 2003 Datacenter Edition
- `bla` - Windows Server 2003 Web Edition
- `per` - Windows XP Home Edition
- `pro` - Windows XP Professional

## Disclaimer
I do not own this source code. The leaks are not from me. I uploaded the extracted source code to a new repository because I thought it would benefit developers.

## Issues, Feature Requests or Support
Please use the [New Issue](https://github.com/akinbicer/windows-nt5/issues/new) button to submit issues, feature requests or support issues directly to me. You can also send an e-mail to akin.bicer@outlook.com.tr.