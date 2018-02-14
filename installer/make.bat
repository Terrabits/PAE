candle Product.wxs -ext WixUtilExtension -ext WixUIExtension -out Product.wixobj

light.exe Product.wixobj -spdb -ext WixUtilExtension -ext WixUIExtension -out "R&S PAE 0.6.msi"
