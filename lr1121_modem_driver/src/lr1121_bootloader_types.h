/*!
 * @file      lr1121_bootloader_types.h
 *
 * @brief     Bootloader driver types for LR1121
 *
 * @copyright Copyright Semtech Corporation 2024. All rights reserved.
 *
 * @license{The Clear BSD License
 * @par
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
 * @par
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
 * POSSIBILITY OF SUCH DAMAGE.}
 */

#ifndef LR1121_BOOTLOADER_TYPES_H
#define LR1121_BOOTLOADER_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*!
 * @brief Length in byte of the LR1121 version blob
 */
#define LR1121_BL_VERSION_LENGTH ( 4 )

/*!
 * @brief Length in bytes of a PIN
 */
#define LR1121_BL_PIN_LENGTH ( 4 )

/*!
 * @brief Length in bytes of a chip EUI
 */
#define LR1121_BL_CHIP_EUI_LENGTH ( 8 )

/*!
 * @brief Length in bytes of a join EUI
 */
#define LR1121_BL_JOIN_EUI_LENGTH ( 8 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief Fixed-length array to store a PIN
 */
typedef uint8_t lr1121_bootloader_pin_t[LR1121_BL_PIN_LENGTH];

/*!
 * @brief Fixed-length array to store a chipEUI
 */
typedef uint8_t lr1121_bootloader_chip_eui_t[LR1121_BL_CHIP_EUI_LENGTH];

/*!
 * @brief Fixed-length array to store a joinEUI
 */
typedef uint8_t lr1121_bootloader_join_eui_t[LR1121_BL_JOIN_EUI_LENGTH];

/*!
 * @brief Chip modes
 */
typedef enum lr1121_bootloader_chip_modes_e
{
    LR1121_BOOTLOADER_CHIP_MODE_SLEEP     = 0x00,
    LR1121_BOOTLOADER_CHIP_MODE_STBY_RC   = 0x01,
    LR1121_BOOTLOADER_CHIP_MODE_STBY_XOSC = 0x02,
    LR1121_BOOTLOADER_CHIP_MODE_FS        = 0x03,
    LR1121_BOOTLOADER_CHIP_MODE_RX        = 0x04,
    LR1121_BOOTLOADER_CHIP_MODE_TX        = 0x05,
    LR1121_BOOTLOADER_CHIP_MODE_LOC       = 0x06,
} lr1121_bootloader_chip_modes_t;

/*!
 * @brief Reset status
 */
typedef enum lr1121_bootloader_reset_status_e
{
    LR1121_BOOTLOADER_RESET_STATUS_CLEARED      = 0x00,
    LR1121_BOOTLOADER_RESET_STATUS_ANALOG       = 0x01,
    LR1121_BOOTLOADER_RESET_STATUS_EXTERNAL     = 0x02,
    LR1121_BOOTLOADER_RESET_STATUS_SYSTEM       = 0x03,
    LR1121_BOOTLOADER_RESET_STATUS_WATCHDOG     = 0x04,
    LR1121_BOOTLOADER_RESET_STATUS_IOCD_RESTART = 0x05,
    LR1121_BOOTLOADER_RESET_STATUS_RTC_RESTART  = 0x06,
} lr1121_bootloader_reset_status_t;

/*!
 * @brief Command status
 */
typedef enum lr1121_bootloader_command_status_e
{
    LR1121_BOOTLOADER_CMD_STATUS_FAIL = 0x00,
    LR1121_BOOTLOADER_CMD_STATUS_PERR = 0x01,
    LR1121_BOOTLOADER_CMD_STATUS_OK   = 0x02,
    LR1121_BOOTLOADER_CMD_STATUS_DATA = 0x03,
} lr1121_bootloader_command_status_t;

/*!
 * @brief Status register 1 structure definition
 */
typedef struct lr1121_bootloader_stat1_s
{
    lr1121_bootloader_command_status_t command_status;       //!< Status of the previous command
    bool                               is_interrupt_active;  //!< Indicator of currently active interrupt
} lr1121_bootloader_stat1_t;

/*!
 * @brief Status register 2 structure definition
 */
typedef struct lr1121_bootloader_stat2_s
{
    lr1121_bootloader_reset_status_t reset_status;  //!< Source of latest reset
    lr1121_bootloader_chip_modes_t   chip_mode;     //!< Current chip mode
    bool is_running_from_flash;                     //!< Indicator of firmware running from flash or from bootloader
} lr1121_bootloader_stat2_t;

/*!
 * @brief Bootloader version structure definition
 */
typedef struct lr1121_bootloader_version_s
{
    uint8_t  hw;    //!< Hardware bootloader version
    uint8_t  type;  //!< Type bootloader version
    uint16_t fw;    //!< Firmware bootloader version
} lr1121_bootloader_version_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // LR1121_BOOTLOADER_TYPES_H

/* --- EOF ------------------------------------------------------------------ */
