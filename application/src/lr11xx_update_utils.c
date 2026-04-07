/*!
 * @file      lr11xx_update_utils.c
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdio.h>
#include <stdbool.h>

#include "lr11xx_update_utils.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define LR11XX_TYPE_PRODUCTION_MODE 0xDF

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

typedef struct
{
    lr11xx_fw_update_t update;
    uint32_t           fw_version;
    uint16_t           bootloader_version;
} compatibility_t;

static const compatibility_t compatibility_matrix[] = {
    { .update = LR1110_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0307, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0308, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0401, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0402, .bootloader_version = 0x1001 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_MODEM_V1, .fw_version = 0x04010107, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_MODEM_V1, .fw_version = 0x04010108, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_MODEM_V1, .fw_version = 0x04010109, .bootloader_version = 0x6500 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_MODEM_V1, .fw_version = 0x04020100, .bootloader_version = 0x1001 },
    { .update = LR1110_FIRMWARE_UPDATE_TO_BL_LOADER, .fw_version = 0x6500, .bootloader_version = 0x6500 },
    { .update = LR1120_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0101, .bootloader_version = 0x2000 },
    { .update = LR1120_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0102, .bootloader_version = 0x2000 },
    { .update = LR1120_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0201, .bootloader_version = 0x2000 },
    { .update = LR1120_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0202, .bootloader_version = 0x2001 },
    { .update = LR1120_FIRMWARE_UPDATE_TO_BL_LOADER, .fw_version = 0x2000, .bootloader_version = 0x2000 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0101, .bootloader_version = 0x2100 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0102, .bootloader_version = 0x2100 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0103, .bootloader_version = 0x2100 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_TRX, .fw_version = 0x0104, .bootloader_version = 0x2101 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_MODEM_V2, .fw_version = 0x05020001, .bootloader_version = 0x2100 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_MODEM_V2, .fw_version = 0x05020002, .bootloader_version = 0x2100 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_MODEM_V2, .fw_version = 0x05020100, .bootloader_version = 0x2101 },
    { .update = LR1121_FIRMWARE_UPDATE_TO_BL_LOADER, .fw_version = 0x2100, .bootloader_version = 0x2100 },
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

bool lr11xx_is_chip_in_production_mode( uint8_t type )
{
    return ( type == LR11XX_TYPE_PRODUCTION_MODE ) ? true : false;
}

bool lr11xx_is_fw_direction_compatible_with_bootloader_version( lr11xx_fw_update_t update, uint32_t fw_version,
                                                                uint16_t bootloader_version )
{
    const unsigned int n_comp_elem = sizeof( compatibility_matrix ) / sizeof( *compatibility_matrix );

    // Parameters are valid if the combination is in compatibility_matrix
    for( unsigned int comp_index = 0; comp_index < n_comp_elem; comp_index++ )
    {
        const compatibility_t* elem = &compatibility_matrix[comp_index];
        if( elem->bootloader_version == bootloader_version && elem->fw_version == fw_version && elem->update == update )
        {
            return true;
        }
    }
    return false;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
