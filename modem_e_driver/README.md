# LR1110/LR1121 Modem-E v2.1 driver

This project proposes an implementation in C of the drivers for LR1110/LR1121 Modem-E v2.1.

It does not involve any state machine or high level API.

## Firmware compatibility

This driver is known to be compatible with firmware LR1110 Modem-E v2.1.0 and LR1121 Modem-E v2.1.0.

## Components

The driver is split in several components:

- Transceiver components
  - Bootloader
  - System configuration
  - Register / memory access
  - Direct radio access
  - LR-FHSS
- Modem components
  - Board specific package (BSP)
  - LoRaWAN modem
  - Relay

The transceiver commands are available only if the modem is set to *suspended* state (by calling `modem_e_set_suspend`), except for:

- all register and memory access commands
- the following system commands:
  - `modem_e_system_get_version`
  - `modem_e_system_set_reg_mode`
  - `modem_e_system_set_dio_as_rf_switch`
  - `modem_e_system_set_dio_irq_params`
  - `modem_e_system_cfg_lfclk`
  - `modem_e_system_set_tcxo_mode`
  - `modem_e_system_reboot`
  - `modem_e_system_get_vbat`
  - `modem_e_system_get_temp`
  - `modem_e_system_get_random_number`
  - `modem_e_system_read_uid`
  - `modem_e_system_read_join_eui`
  - `modem_e_system_read_pin`

## HAL integration

The SPI commands this driver requires are documented in the file [modem_e_hal.h](src/modem_e_hal.h) and [modem_e_modem_hal.h](src/modem_e_modem_hal.h).

All driver commands are using the HAL from `modem_e_modem_hal.h` except the bootloader ones that are using the ones from `modem_e_hal.h`.
