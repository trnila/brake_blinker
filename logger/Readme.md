# Capturing measurements on Xiaomi Redmi 3S
## Build usb serial driver cp210x.ko for Android
1. Get kernel branch for your running kernel on Android and checkout same commit
```sh
$ git clone -b lineage-15.1  https://github.com/LineageOS/android_kernel_xiaomi_msm8937.git --depth 1
$ cd android_kernel_xiaomi_msm8937
```

2. Download and prepare crosscompiler
```sh
$ git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 --depth 1
$ export PATH=$PATH:$PWD/aarch64-linux-android-4.9/bin/
$ export CROSS_COMPILE=aarch64-linux-android-
```

3. Copy current kernel configuration from mobile's */proc/config.gz* to *.config*
```sh
$ adb exec-out cat /proc/config.gz | gzip -d > .config
```

4. Enable compilation of driver *cp210x.ko* as loadable module
```sh
$ echo CONFIG_USB_SERIAL_CP210X=m >> .config
```

5. Build whole kernel
```sh
$ export ARCH=arm64
$ make
```

6. Copy driver to your phone
```sh
$ adb push drivers/usb/serial/cp210x.ko /sdcard/
```

7. Insert module
```sh
$ adb shell
land:/ $ su
land:/ # insmod /sdcard/cp210x.ko
```

8. Connect usb serial and test it
```sh
land:/ # stty -F /dev/ttyUSB0 115200 raw
land:/ # cat /dev/ttyUSB0
```

Precompiled driver for lineage-15.1-20190522-nightly-land/3.18.131-lineageos-g22175f5b is at this directory.
