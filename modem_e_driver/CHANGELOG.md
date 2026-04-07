# LR1121 modem driver changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2026-03-27

### Added

- `modem_e_connect_with_abp`
- `modem_e_store_state_snapshot_to_nvm` and `modem_e_restore_state_snapshot_from_nvm`
- `modem_e_set_user_dr_backoff_sequence_threshold`/`modem_e_get_user_dr_backoff_sequence_threshold`
- `modem_e_connect_get_channel_mask` and `modem_e_connect_set_channel_mask`
- Events `MODEM_E_LORAWAN_EVENT_DR_BACKOFF_LIMIT` and `MODEM_E_LORAWAN_EVENT_RESET_REQUEST`

### Changed

- Renaming for Modem-E v2.1 compliance, with support of LR1110/LR1121 Modem-E
- Add one byte syncword argument to `modem_e_test_tx_lora`

### Removed

- `modem_e_regmem_write_mem8`/`modem_e_regmem_read_mem8` (formerly known as `lr1121_modem_regmem_write_mem8` and `lr1121_modem_regmem_read_mem8`)
- Field `reset` from `modem_e_event_t` structure (formerly known as `lr11xx_modem_event_t`)

## [1.1.0] - 2025-07-02

### Added

- `lr1121_modem_system_get_status_and_clear_reset_source` that replaces `lr1121_modem_system_clear_reset_status_info` and `lr1121_modem_system_get_status`
- Documentation of Tx Done status `lr1121_modem_tx_done_event_t`

### Fixed

- `lr1121_modem_system_set_dio_irq_params` was disabling an internal IRQ
- Implementation of `lr1121_modem_system_get_irq_status` to not rely on HAL function `lr1121_modem_hal_direct_read`
- Documentation of `lr1121_modem_get_next_tx_max_payload`
- Various conversion, sign-conversion, switch default, and unused macros compilation warnings

### Changed

- `lr1121_modem_system_set_sleep` can configure sleep mode without RTC running

### Removed

- `lr1121_modem_hal_direct_read` from Modem HAL API
- `lr1121_modem_system_clear_reset_status_info` and `lr1121_modem_system_get_status` which are replaced by `lr1121_modem_system_get_status_and_clear_reset_source`

## [v1.0.0] - 2024-09-19

- Initial release
