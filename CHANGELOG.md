# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v2.5.1] - 2024-09-23

### Changed

- LR1121 Modem-E dependency updated to release version

### Fixed

- Fix Keil compiling toolchain following LR1121 Modem-E driver addition

## [v2.5.0] - 2024-09-20

### Added

- Support to flash Modem-E v2 on LR1121 chips
- It is no longer useful to set LR11XX_FIRMWARE_IMAGE_SIZE in transceiver.h or modem.h file
- Add programatic include to choose image to load
- Rename the destination update names `LR1110_FIRMWARE_UPDATE_TO_MODEM_V1` and `LR1121_FIRMWARE_UPDATE_TO_MODEM_V2`

### Removed

- Remove the destination update `LR1120_FIRMWARE_UPDATE_TO_MODEM`

## [2.4.0] - 2024-02-07

### Added

- Fimrware images
  - LR1110 LoRa Basics Modem-E `1.1.8`
  - LR1110 LoRa Basics Modem-E `1.1.9`

## [2.3.0] - 2023-12-12

### Changed

- [driver] Update driver to v2.4.1
- [driver LR11xx modem] Update driver to v3.1.0

### Added

- Fimrware images
  - LR1110 `0x0401`
  - LR1120 `0x0201`
  - LR1121 `0x0103`

## [2.2.0] - 2023-06-05

### Changed

- [driver] Updated LR11xx driver to v2.3.0

## [2.1.0] - 2022-04-08

### Added

- [app] Support of LR1121 transceiver

## [2.0.0] - 2022-04-08

### Added

- [app] Revamped the way core implementation and helper functions are interleaved for better clarity
- [system] `system_spi_read_with_dummy_byte()` function

### Changed

- [driver] Updated LoRa Basics Modem-E driver to v3.0.1
- [driver] Transitionned from LR1110 driver to LR11xx driver v2.1.1
- [driver] LR11xx HAL implementations
- [debug] Improved debug messages on both UART and display interfaces

### Removed

- [system] `system_spi_read()` and `system_spi_write_read()` functions

## [1.2.0] - 2020-06-02

### Changed

- [driver] Updated LoRa Basics Modem-E driver to v2.0.1
- [debug] Improved debug messages on both UART and display interfaces

## [1.1.0] - 2020-10-13

### Added

- Initial version (with LR1110 driver v3.0.0 and LoRa Basics Modem-E driver v1.0.0)
