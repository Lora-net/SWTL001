/*!
 * @file      lr11xx_firmware_update.h
 *
 * @brief     LR11XX firmware update definition
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
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

#ifndef LR11XX_FIRMWARE_UPDATE_H
#define LR11XX_FIRMWARE_UPDATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "lr11xx_types.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

typedef enum
{
    LR1110_FIRMWARE_UPDATE_TO_TRX,
    LR1110_FIRMWARE_UPDATE_TO_MODEM_V2,
    LR1120_FIRMWARE_UPDATE_TO_TRX,
    LR1121_FIRMWARE_UPDATE_TO_TRX,
    LR1121_FIRMWARE_UPDATE_TO_MODEM_V2,
    LR1110_FIRMWARE_UPDATE_TO_BL_LOADER,
    LR1120_FIRMWARE_UPDATE_TO_BL_LOADER,
    LR1121_FIRMWARE_UPDATE_TO_BL_LOADER,
} lr11xx_fw_update_t;

typedef enum
{
    LR11XX_FW_UPDATE_OK                = 0,
    LR11XX_FW_UPDATE_WRONG_CHIP_TYPE   = 1,
    LR11XX_FW_UPDATE_ERROR             = 2,
    LR11XX_FW_UPDATE_BOOTLOADER_FAILED = 3,
} lr11xx_fw_update_status_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Reset LR11xx chip to bootloader mode
 *
 * @param radio Chip implementation context
 * @return Status of the operation
 */
lr11xx_status_t lr11xx_reset_to_bootloader( void* radio );

/**
 * @brief Update a firmware into LR11xx chip
 *
 * @note The LR11xx must have already been reset to bootloader with @ref lr11xx_reset_to_bootloader prior calling this
 * function.
 *
 * @param radio Chip implementation context
 * @param fw_update_direction Firmware direction of the firmware code
 * @param fw_expected Expected firmware version after update
 * @param buffer Firmware code to update
 * @param length Length of @p buffer in word
 * @return Status of the firmware update operation
 */
lr11xx_fw_update_status_t lr11xx_update_firmware( void* radio, lr11xx_fw_update_t fw_update_direction,
                                                  uint32_t fw_expected, const uint32_t* buffer, uint32_t length );

/**
 * @brief
 *
 * @param radio Chip implementation context
 * @param fw_update_direction Firmware direction of the firmware code
 * @param fw_expected Expected firmware version after update
 * @return Status of the firmware update operation
 */
lr11xx_fw_update_status_t lr11xx_update_post_flash_reboot_and_verification( void*              radio,
                                                                            lr11xx_fw_update_t fw_update_direction,
                                                                            uint32_t           fw_expected );

#ifdef __cplusplus
}
#endif

#endif  // LR11XX_FIRMWARE_UPDATE_H

/* --- EOF ------------------------------------------------------------------ */
