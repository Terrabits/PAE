R&S PAE
=======

R&S PAE coordinates a measurement on a R&S Vector Network Analyzer with one or more external Digital Multimeters to calculate Power Added Efficiency or Drain Efficiency.

Requirements
------------

The application requires the following to build:

- Qt 5.2.1 msvc2010
- Microsoft Visual C++ 2010
- Windows Installer XML 3.10
- Compiled version of RsaToolbox (commit `6a6b21c77a7177af14d8b75f5e7d1d9261e6dfd8` from master)

This RsaToolbox commit can be found on GitHub:

[RsaToolbox commit 6a6b21c77a7177af14d8b75f5e7d1d9261e6dfd8](https://github.com/Terrabits/RsaToolbox/tree/6a6b21c77a7177af14d8b75f5e7d1d9261e6dfd8)

Instructions for compiling and using RsaToolbox are included in the readme.

Build
-----

To build the application, a compiled "install" version of RsaToolbox must be located at `./rsatoolbox`

Open the application in Qt Creator and build for release.

Create Installer
----------------

Creating the installer (`installer/R&S PAE X.Y.Z.msi`) requires the Windows Installer XML binaries to be in your path. Specifically, `candle.exe` and `light.exe` must be in the path.

The WiX installer script (`installer/Product.wxs`) also assumes that the build directory is located at `./build`

To create the installer, run the `installer/make.bat` file, with `installer/` as your working directory. The result should be an `.msi` file in the same directory.
