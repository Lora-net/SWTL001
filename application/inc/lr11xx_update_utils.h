/*!
 * @file      lr11xx_update_utils.h
 *
 * @brief     LR11XX update utilities
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

#ifndef LR11XX_UPDATE_UTILS_H
#define LR11XX_UPDATE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include "lr11xx_firmware_update.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/**
 * @brief Helper macro that execute return call if status is not @ref LR11XX_STATUS_OK
 *
 */
#define RETURN_STATUS_ON_NOT_OK( call )      \
    do                                       \
    {                                        \
        const lr11xx_status_t status = call; \
        if( status != LR11XX_STATUS_OK )     \
        {                                    \
            return status;                   \
        }                                    \
    } while( 0 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Determine if the chip's type indicates production mode
 *
 * @param type The chip type obtained from @ref lr11xx_bootloader_get_version
 * @retval true The chip is in production mode
 * @return false The chip is not in production mode
 */
bool lr11xx_is_chip_in_production_mode( uint8_t type );

/**
 * @brief Determine if the firmware update direction, firmware version, and bootloader version are compatible
 *
 * @param update The firmware direction of the update
 * @param fw_version The firmware version of the update
 * @param bootloader_version The running bootloader version
 * @return true Provided versions and direction are compatible together
 * @return false Provided versions and direction are not compatible together
 */
bool lr11xx_is_fw_direction_compatible_with_bootloader_version( lr11xx_fw_update_t update, uint32_t fw_version,
                                                                uint16_t bootloader_version );

#ifdef __cplusplus
}
#endif

#endif  // LR11XX_UPDATE_UTILS_H

/* --- EOF ------------------------------------------------------------------ */
