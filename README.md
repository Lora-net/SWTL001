# LR11xx Updater tool

This tool is a reference implementation of the mechanism to update a firmware in a LR1110 / LR1120 / LR1121 chip. It is based on the [Application Note AN1200.57](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R000000Hlit/77J00f9xeOhqu8XDfHrB0G54bEJGikO58GGYT__hyis) - "LR1110: Upgrade of the Program Memory".

## Requirements

### Supported boards

This tool is developed on the ST Microeletronic [NUCLEO-L476RG development board](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html). It is a reference implementation that can be ported to another MCU.

### Supported shields

The list of compatible Semtech LR1110 shields is:

| Shield       | PCB                                                   | Frequency matching |
| ------------ | ----------------------------------------------------- | ------------------ |
| LR1110MB1DIS | PCB_E655V01A - GNSS with LNA for Passive GNSS Antenna | 868/915MHz         |
| LR1110MB1DJS | PCB_E656V01A - GNSS without LNA                       | 868/915MHz         |
| LR1110MB1GIS | PCB_E655V01A - GNSS with LNA for Passive GNSS Antenna | 490MHz             |
| LR1110MB1GJS | PCB_E656V01A - GNSS without LNA                       | 490MHz             |

The list of compatible Semtech LR1120 shields is:

| Shield       | PCB                                                   | Frequency matching |
| ------------ | ----------------------------------------------------- | ------------------ |
| LR1120MB1DIS | PCB_E655V01A - GNSS with LNA for Passive GNSS Antenna | 868/915MHz         |
| LR1120MB1DJS | PCB_E656V01A - GNSS without LNA                       | 868/915MHz         |
| LR1120MB1GIS | PCB_E655V01A - GNSS with LNA for Passive GNSS Antenna | 490MHz             |
| LR1120MB1GJS | PCB_E656V01A - GNSS without LNA                       | 490MHz             |

The list of compatible Semtech LR1121 shields is:

| Shield       | PCB          | Frequency matching |
| ------------ | ------------ | ------------------ |
| LR1121MB1DIS | PCB_E655V01A | 868/915MHz         |
| LR1121MB1GIS | PCB_E655V01A | 490MHz             |

### Touchscreen (optional)

This tool is compatible with a touchscreen (DM-TFT28-116) that can be optionnaly connected on top of the shield to get information directly - without the need to open a terminal on the computer connected to the board.

### Toolchain

This tool can be compiled with the following toolchains:

* [Keil MDK ARM](https://www2.keil.com/mdk5) - Keil project file available in `apps/<example>/MDK-ARM/`
* [GNU Arm Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) - makefile available in `apps/<example>/makefile/`

## Getting started

### Configure

Before starting to build an example, check the firmware header file being included in the [main file](application/src/main.c), ``application/src/main.c``.

### Build

#### Pre-compiled binaries

Pre-compiled binaries are available [the Wiki](https://github.com/Lora-net/SWTL001/wiki/home).

#### Keil MDK ARM

This tool is delivered with a Keil project file - see `keil/lr11xx-updater-tool.uvprojx`.

To build a project:

1. Launch Keil IDE
2. Open the project file
3. Compile

#### GNU Arm embedded toolchain

This tool is built from a Makefile available in the root folder of the project.

The output files of the build process are stored in the `build` folder with firmware binary file having the same name as the project with a .bin extension.

To build a project, simply run make:

```shell
$ cd $LR11XX_UPDATER_TOOL_FOLDER
$ make
```

### Load

After a project is built, it can be loaded onto a device.

There are multiple ways to do it, among which:

* Drag and drop the binary file to the USB drive listed by our OS - usually shows up as `NODE_L476RG`.
* Load it through the Keil IDE
* A tool like [STM32 Cube Programmer](https://www.st.com/en/development-tools/stm32cubeprog.html)

:warning: drag & drop binary file to NUCLEO USB storage device in windows is currently not functioning as of November 2023.  The microsoft patch is not generally available as of this date.  If copying binary file to NUCLEO USB storage does not function in windows, use the STM32CubeProgrammer.\
USB mass storage works OK in other operating systems. :warning:
### Operations

As soon as the binary is downloaded, the update process begins.

The user can get information about the update status through the following interfaces:

* LEDs: an orange LED is on during the update and a green LED indicates that the update is successful (or a red LED if something went wrong)
* COM port: if there is a terminal connected to the COM port exposed by the NUCLEO board, information can be read (bitrate set to 921600 bps)
* Touchscreen (if connected): the status is displayed on the screen
