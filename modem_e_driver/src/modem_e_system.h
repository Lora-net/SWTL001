/*!
 * @file      modem_e_system.h
 *
 * @brief     System driver definition
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2026. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MODEM_E_SYSTEM_H
#define MODEM_E_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdint.h>
#include "modem_e_common.h"
#include "modem_e_system_types.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief Frequency step in MHz used to compute the image calibration parameter
 *
 * @see modem_e_system_calibrate_image_in_mhz
 */
#define MODEM_E_SYSTEM_IMAGE_CALIBRATION_STEP_IN_MHZ 4

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Reset the radio
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_reset( const void* context );

/**
 * @brief Wake the radio up from sleep mode.
 *
 * @param [in]  context Chip implementation context.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_wakeup( const void* context );

/*!
 * @brief Return stat1, stat2, and irq_status and clear reset status stored in stat2
 *
 * @param [in] context Chip implementation context
 * @param [out] stat1      Pointer to a variable for holding stat1
 * @param [out] stat2      Pointer to a variable for holding stat2
 * @param [out] irq_status Pointer to a variable for holding irq_status
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_status_and_clear_reset_source( const void*                context,
                                                                          modem_e_system_stat1_t*    stat1,
                                                                          modem_e_system_stat2_t*    stat2,
                                                                          modem_e_system_irq_mask_t* irq_status );

/*!
 * @brief Return irq_status
 *
 * @param [in] context Chip implementation context
 * @param [out] irq_status irq_status status variable
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_irq_status( const void* context, modem_e_system_irq_mask_t* irq_status );

/*!
 * @brief Return the version of the system (hardware and software)
 *
 * @param [in] context Chip implementation context
 * @param [out] version Pointer to the structure holding the system version
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_version( const void* context, modem_e_system_version_t* version );

/*!
 * @brief Return the system errors
 *
 * Errors may be fixed following:
 * - calibration error can be fixed by attempting another RC calibration;
 * - XOsc related errors may be due to hardware problems, can be fixed by reset;
 * - PLL lock related errors can be due to not-locked PLL, or by attempting to use an out-of-band frequency, can be
 * fixed by executing a PLL calibration, or by using other frequencies.
 *
 * @param [in] context Chip implementation context
 * @param [out] errors Pointer to a value holding error flags
 *
 * @returns Operation status
 *
 * @see modem_e_system_calibrate, modem_e_system_calibrate_image, modem_e_system_clear_errors
 */
modem_e_response_code_t modem_e_system_get_errors( const void* context, uint16_t* errors );

/*!
 * @brief Clear all error flags pending.
 *
 * This function cannot be used to clear flags individually.
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 *
 * @see modem_e_system_get_errors
 */
modem_e_response_code_t modem_e_system_clear_errors( const void* context );

/*!
 * @brief modem_e_system_calibrate the requested blocks
 *
 * This function can be called in any mode of the chip.
 *
 * The chip will return to standby RC mode on exit. Potential calibration issues can be read out with
 * modem_e_system_get_errors command.
 *
 * @param [in] context Chip implementation context
 * @param [in] calib_param Structure holding the reference to blocks to be calibrated
 *
 * @returns Operation status
 *
 * @see modem_e_system_get_errors
 */
modem_e_response_code_t modem_e_system_calibrate( const void* context, const uint8_t calib_param );

/*!
 * @brief Configure the regulator mode to be used in specific modes
 *
 * This function shall only be called in standby RC mode.
 *
 * The reg_mode parameter defines if the DC-DC converter is switched on in the following modes: STANDBY XOSC, FS, RX, TX
 * and RX_CAPTURE.
 *
 * @param [in] context Chip implementation context
 * @param [in] reg_mode Regulator mode configuration
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_set_reg_mode( const void* context, const modem_e_system_reg_mode_t reg_mode );

/*!
 * @brief Launch an image calibration valid for all frequencies inside an interval, in steps
 *
 * This function can be called in any mode of the chip.
 *
 * The chip will return to standby RC mode on exit. Potential calibration issues can be read out with
 * modem_e_system_get_errors command.
 *
 * The frequencies given in parameters are defined in 4MHz step (Eg. 900MHz corresponds to 0xE1). If freq1 = freq2, only
 * one calibration is performed.
 *
 * @param [in] context Chip implementation context
 * @param [in] freq1 Image calibration interval lower bound, in steps
 * @param [in] freq2 Image calibration interval upper bound, in steps
 *
 * @remark freq1 must be less than or equal to freq2
 *
 * @returns Operation status
 *
 * @see modem_e_system_get_errors
 */
modem_e_response_code_t modem_e_system_calibrate_image( const void* context, const uint8_t freq1, const uint8_t freq2 );

/*!
 * @brief Launch an image calibration valid for all frequencies inside an interval, in MHz
 *
 * @remark This function relies on @ref modem_e_system_calibrate_image
 *
 * @param [in] context Chip implementation context
 * @param [in] freq1_in_mhz Image calibration interval lower bound, in MHz
 * @param [in] freq2_in_mhz Image calibration interval upper bound, in MHz
 *
 * @remark freq1 must be less than or equal to freq2
 *
 * @returns Operation status
 *
 * @see modem_e_system_calibrate_image
 */
modem_e_response_code_t modem_e_system_calibrate_image_in_mhz( const void* context, const uint16_t freq1_in_mhz,
                                                               const uint16_t freq2_in_mhz );

/*!
 * @brief Set the RF switch configurations for each RF setup
 *
 * This function shall only be called in standby RC mode.
 *
 * By default, no DIO is used to control a RF switch. All DIOs are set in High-Z mode.
 *
 * @param [in] context Chip implementation context
 * @param [in] rf_switch_cfg Pointer to a structure that holds the switches configuration
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_set_dio_as_rf_switch( const void*                          context,
                                                             const modem_e_system_rfswitch_cfg_t* rf_switch_cfg );

/*!
 * @brief Set which interrupt signals are redirected to the dedicated DIO pin
 *
 * By default, no interrupt signal is redirected.
 *
 * The dedicated DIO pin will remain asserted until all redirected interrupt signals are cleared with a call to
 * modem_e_system_clear_irq_status.
 *
 * @param [in] context Chip implementation context
 * @param [in] irqs_to_enable_dio1 Variable that holds the interrupt mask for dio1
 * @param [in] irqs_to_enable_dio2 Variable that holds the interrupt mask for dio2
 *
 * @returns Operation status
 *
 * @see modem_e_system_clear_irq_status
 */
modem_e_response_code_t modem_e_system_set_dio_irq_params( const void*               context,
                                                           modem_e_system_irq_mask_t irqs_to_enable_dio1,
                                                           modem_e_system_irq_mask_t irqs_to_enable_dio2 );

/*!
 * @brief Clear requested bits in the internal pending interrupt register
 *
 * @param [in] context Chip implementation context
 * @param [in] irqs_to_clear Variable that holds the interrupts to be cleared
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_clear_irq_status( const void*                     context,
                                                         const modem_e_system_irq_mask_t irqs_to_clear );

/**
 * @brief This helper function clears any radio irq status flags that are set and returns the flags that were cleared.
 *
 * @param [in] context Chip implementation context.
 * @param [out] irq Pointer to a variable for holding the system interrupt status. Can be NULL.
 *
 * @returns Operation status
 *
 * @see modem_e_system_get_irq_status, modem_e_system_clear_irq_status
 */
modem_e_response_code_t modem_e_system_get_and_clear_irq_status( const void* context, modem_e_system_irq_mask_t* irq );

/*!
 * @brief Defines which clock is used as Low Frequency (LF) clock
 *
 * @param [in] context Chip implementation context
 * @param [in] lfclock_cfg Low frequency clock configuration
 * @param [in] wait_for_32k_ready Tells the radio if it has to check if 32k source is ready before driving busy low
 *
 * @returns Operation status
 *
 * @see modem_e_system_calibrate, modem_e_system_calibrate_image
 */
modem_e_response_code_t modem_e_system_cfg_lfclk( const void* context, const modem_e_system_lfclk_cfg_t lfclock_cfg,
                                                  const bool wait_for_32k_ready );

/*!
 * @brief Enable and configure TCXO supply voltage and detection timeout
 *
 * This function shall only be called in standby RC mode.
 *
 * The timeout parameter is the maximum time the firmware waits for the TCXO to be ready. The timeout duration is given
 * by: \f$ timeout\_duration\_us = timeout \times 30.52 \f$
 *
 * The TCXO mode can be disabled by setting timeout parameter to 0.
 *
 * @param [in] context Chip implementation context
 * @param [in] tune Supply voltage value
 * @param [in] timeout Gating time before which the radio starts its Rx / Tx operation
 *
 * @returns Operation status
 *
 * @see modem_e_system_calibrate, modem_e_system_calibrate_image
 */
modem_e_response_code_t modem_e_system_set_tcxo_mode( const void*                                context,
                                                      const modem_e_system_tcxo_supply_voltage_t tune,
                                                      const uint32_t                             timeout );

/*!
 * @brief Software reset of the chip.
 *
 * This function should be used to reboot the chip in a specified mode. Rebooting in flash mode presumes that the
 * content in flash memory is not corrupted (i.e. the integrity check performed by the bootloader before executing the
 * first instruction in flash is OK).
 *
 * @param [in] context Chip implementation context
 * @param [in] stay_in_bootloader Selector to stay in bootloader or execute flash code after reboot. If true, the
 * bootloader will not execute the flash code but activate SPI interface to allow firmware upgrade
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_reboot( const void* context, const bool stay_in_bootloader );

/*!
 * @brief Returns the value of Vbat
 *
 * Vbat value (in V) is a function of Vana (typ. 1.35V) using the following
 * formula: \f$ Vbat_{V} = (5 \times \frac{Vbat}{255} - 1) \times Vana \f$
 *
 * @param [in] context Chip implementation context
 * @param [out] vbat A pointer to the Vbat value
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_vbat( const void* context, uint8_t* vbat );

/*!
 * @brief Returns the value of Temp
 *
 * The temperature (in °C) is a function of Vana (typ. 1.35V), Vbe25 (Vbe voltage @ 25°C, typ. 0.7295V) and VbeSlope
 * (typ. -1.7mV/°C) using the following formula:
 * \f$ Temperature_{°C} = (\frac{Temp(10:0)}{2047} \times Vana - Vbe25) \times \frac{1000}{VbeSlope} + 25 \f$
 *
 * @remark If a TCXO is used, make sure to configure it with @ref modem_e_system_set_tcxo_mode before calling this
 * function
 *
 * @param [in] context Chip implementation context
 * @param [out] temp A pointer to the Temp value
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_temp( const void* context, uint16_t* temp );

/*!
 * @brief Set the device into Sleep or Deep Sleep Mode
 *
 * The sleep_cfg parameter defines in which sleep mode the device is put and if it wakes up after a given time on the
 * RTC event.
 *
 * The @p sleep_time parameter is taken into account only when \ref modem_e_system_sleep_cfg_s::is_rtc_timeout is
 * @p true. It sets the sleep time in number of clock cycles: \f$ sleep\_time\_ms = sleep\_time \times \frac{1}{32.768}
 * \f$
 *
 * @warning If the configuration modem_e_system_sleep_cfg_s::is_rtc_timeout is set to @p false then the low
 * frequency clock must be configured to @ref modem_e_system_lfclk_cfg_e::MODEM_E_SYSTEM_LFCLK_RC with @ref
 * modem_e_system_cfg_lfclk.
 *
 * @param [in] context Chip implementation context
 * @param [in] sleep_cfg Sleep mode configuration
 * @param [in] sleep_time Value of the RTC timeout (if \ref modem_e_system_sleep_cfg_s::is_rtc_timeout is @p true)
 *
 * @returns Operation status
 *
 * @see modem_e_system_set_standby, modem_e_system_set_fs, modem_e_system_cfg_lfclk
 */
modem_e_response_code_t modem_e_system_set_sleep( const void* context, const modem_e_system_sleep_cfg_t sleep_cfg,
                                                  const uint32_t sleep_time );

/*!
 * @brief Set the device into the requested Standby mode
 *
 * @param [in] context Chip implementation context
 * @param [in] standby_cfg Stand by mode configuration (RC or XOSC)
 *
 * @returns Operation status
 *
 * @see modem_e_system_set_sleep, modem_e_system_set_fs
 */
modem_e_response_code_t modem_e_system_set_standby( const void*                        context,
                                                    const modem_e_system_standby_cfg_t standby_cfg );

/*!
 * @brief Set the device into Frequency Synthesis (FS) mode
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 *
 * @see modem_e_system_set_standby, modem_e_system_set_sleep
 */
modem_e_response_code_t modem_e_system_set_fs( const void* context );

/*!
 * @brief Erase an info page
 *
 * @param [in] context Chip implementation context
 * @param [in] info_page_id Info page to be erased. Only MODEM_E_SYSTEM_INFOPAGE_1 is allowed.
 *
 * @returns Operation status
 *
 * @see modem_e_system_write_infopage, modem_e_system_read_infopage
 */
modem_e_response_code_t modem_e_system_erase_infopage( const void*                        context,
                                                       const modem_e_system_infopage_id_t info_page_id );

/*!
 * @brief Write data in an info page
 *
 * @param [in] context Chip implementation context
 * @param [in] info_page_id Info page where data are written. Only MODEM_E_SYSTEM_INFOPAGE_1 is allowed.
 * @param [in] address Address within the info page (aligned on 32-bit data)
 * @param [in] data Pointer to the data to write (data buffer shall be - at least - length words long)
 * @param [in] length Number of 32-bit data to write (maximum value is 64)
 *
 * @returns Operation status
 *
 * @see modem_e_system_erase_infopage, modem_e_system_read_infopage
 */
modem_e_response_code_t modem_e_system_write_infopage( const void*                        context,
                                                       const modem_e_system_infopage_id_t info_page_id,
                                                       const uint16_t address, const uint32_t* data,
                                                       const uint8_t length );

/*!
 * @brief Read data from an info page
 *
 * It is possible to cross from page 0 to 1 if (address + length >= 512)
 *
 * @param [in] context Chip implementation context
 * @param [in] info_page_id Info page where data are read
 * @param [in] address Address within the info page (aligned on 32-bit data)
 * @param [out] data Pointer to the data to read (data buffer shall be - at least - length words long)
 * @param [in] length Number of 32-bit data to read (maximum value is 64)
 *
 * @returns Operation status
 *
 * @see modem_e_system_erase_infopage, modem_e_system_write_infopage
 */
modem_e_response_code_t modem_e_system_read_infopage( const void*                        context,
                                                      const modem_e_system_infopage_id_t info_page_id,
                                                      const uint16_t address, uint32_t* data, const uint8_t length );

/*!
 * @brief Read and return the Unique Identifier of the Modem-E
 *
 * @param [in] context Chip implementation context
 * @param [out] unique_identifier The buffer to be filled with the Unique Identifier of the Modem-E. It is up to the
 * application to ensure unique_identifier is long enough to hold the unique identifier
 *
 * @returns Operation status
 *
 * @see MODEM_E_SYSTEM_UID_LENGTH
 */
modem_e_response_code_t modem_e_system_read_uid( const void* context, modem_e_system_uid_t unique_identifier );

/*!
 * @brief Read and return the Join EUI of the Modem-E
 *
 * @param [in] context Chip implementation context
 * @param [out] join_eui The buffer to be filled with Join EUI of the Modem-E. It is up to the application to ensure
 * join_eui is long enough to hold the join EUI
 *
 * @returns Operation status
 *
 * @see MODEM_E_SYSTEM_JOIN_EUI_LENGTH
 */
modem_e_response_code_t modem_e_system_read_join_eui( const void* context, modem_e_system_join_eui_t join_eui );

/*!
 * @brief Compute and return the PIN of the Modem-E based on factory default EUIs
 *
 * @remark Calling this command also triggers a derivation of network and application keys based on factory default EUIs
 *
 * @param [in] context Chip implementation context
 * @param [out] pin The buffer to be filled with PIN of the Modem-E. It is up to the application to ensure pin is long
 * enough to hold the PIN
 *
 * @returns Operation status
 *
 * @see MODEM_E_SYSTEM_PIN_LENGTH
 */
modem_e_response_code_t modem_e_system_read_pin( const void* context, modem_e_system_pin_t pin );

/*!
 * @brief Compute and return the PIN of the Modem-E based on EUIs provided as parameters
 *
 * @remark Calling this command also triggers a derivation of network and application keys based on EUIs provided as
 * parameters
 *
 * @param [in] context Chip implementation context
 * @param [in] device_eui Custom Device EUI
 * @param [in] join_eui Custom Join EUI
 * @param [in] rfu Parameter RFU - shall be set to 0x00
 * @param [out] pin The buffer to be filled with PIN of the Modem-E. It is up to the application to ensure pin is long
 * enough to hold the PIN
 *
 * @returns Operation status
 *
 * @see MODEM_E_SYSTEM_PIN_LENGTH
 */
modem_e_response_code_t modem_e_system_read_pin_custom_eui( const void* context, modem_e_system_uid_t device_eui,
                                                            modem_e_system_join_eui_t join_eui, uint8_t rfu,
                                                            modem_e_system_pin_t pin );

/*!
 * @brief Read and return a 32-bit random number
 *
 * @remark Radio operating mode must be set into standby.
 *
 * @param [in] context Chip implementation context
 * @param [out] random_number 32-bit random number
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_get_random_number( const void* context, uint32_t* random_number );

/*!
 * @brief Enable the CRC on SPI transactions
 *
 * @remark This command shall always be sent with a CRC (to both enable and disable the feature). The function does not
 * take care of this additional byte - which is under the responsibility of the underlying HAL functions
 *
 * @param [in] context Chip implementation context
 * @param [in] enable_crc CRC
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_enable_spi_crc( const void* context, bool enable_crc );

/*!
 * @brief Configure the GPIO drive in sleep mode
 *
 * @remark GPIO stands for RF switch and IRQ line DIOs
 *
 * @note This command is available from firmware version 0x0306
 *
 * @param [in] context       Chip implementation context
 * @param [in] enable_drive  GPIO drive configuration (true: enabled / false: disabled)
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_system_drive_dio_in_sleep_mode( const void* context, bool enable_drive );

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_SYSTEM_H

/* --- EOF ------------------------------------------------------------------ */
