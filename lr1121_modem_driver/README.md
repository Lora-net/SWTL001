# LR1121 Modem-E driver

This project proposes an implementation in C of the drivers for LR1121 Modem-E.

It does not involve any state machine or high level API.

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

The transceiver commands are available only if the modem is set to *suspended* state (by calling `lr1121_modem_set_suspend`), except for:

- all register and memory access commands
- the following system commands:
  - `lr1121_modem_system_get_version`
  - `lr1121_modem_system_set_reg_mode`
  - `lr1121_modem_system_set_dio_as_rf_switch`
  - `lr1121_modem_system_set_dio_irq_params`
  - `lr1121_modem_system_cfg_lfclk`
  - `lr1121_modem_system_set_tcxo_mode`
  - `lr1121_modem_system_reboot`
  - `lr1121_modem_system_get_vbat`
  - `lr1121_modem_system_get_temp`
  - `lr1121_modem_system_get_random_number`
  - `lr1121_modem_system_read_uid`
  - `lr1121_modem_system_read_join_eui`
  - `lr1121_modem_system_read_pin`

## HAL integration

The SPI commands this driver requires are documented in the file [lr1121_modem_hal.h](src/lr1121_modem_hal.h) and [lr1121_hal.h](src/lr1121_hal.h).

All driver commands are using the HAL from `lr1121_modem_hal.h` except the bootloader ones that are using the ones from `lr1121_hal.h`.
