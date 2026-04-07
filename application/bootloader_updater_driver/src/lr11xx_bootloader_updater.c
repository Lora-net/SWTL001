/*!
 * @file      lr11xx_bootloader_updater.c
 *
 * @brief     Bootloader updater driver implementation for LR11XX
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

#include "lr11xx_bootloader_updater.h"
#include "lr11xx_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define LR11XX_BL_UPDATER_CMD_NO_PARAM_LENGTH ( 2 )
#define LR11XX_BL_UPDATER_GET_STATUS_CMD_LENGTH ( 2 + 4 )
#define LR11XX_BL_UPDATER_VERSION_CMD_LENGTH LR11XX_BL_UPDATER_CMD_NO_PARAM_LENGTH
#define LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_CMD_LENGTH LR11XX_BL_UPDATER_CMD_NO_PARAM_LENGTH
#define LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_CMD_LENGTH ( LR11XX_BL_UPDATER_CMD_NO_PARAM_LENGTH )
#define LR11XX_BL_UPDATER_REBOOT_CMD_LENGTH ( LR11XX_BL_UPDATER_CMD_NO_PARAM_LENGTH + 1 )

#define LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_RBUFFER_LENGTH ( 4 )
#define LR11XX_BL_UPDATER_GET_VERSION_RBUFFER_LENGTH ( 4 )
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operating codes for bootloader-related operations
 */
enum
{
    LR11XX_BL_UPDATER_GET_STATUS_OC        = 0x0100,
    LR11XX_BL_UPDATER_GET_VERSION_OC       = 0x0101,
    LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_OC = 0x8100,
    LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_OC = 0x8101,
    LR11XX_BL_UPDATER_REBOOT_OC            = 0x8102,
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

lr11xx_status_t lr11xx_bootloader_updater_get_status( const void* context, lr11xx_bootloader_updater_stat1_t* stat1,
                                                      lr11xx_bootloader_updater_stat2_t*    stat2,
                                                      lr11xx_bootloader_updater_irq_mask_t* irq_status )
{
    uint8_t data[LR11XX_BL_UPDATER_GET_STATUS_CMD_LENGTH] = { 0 };

    const lr11xx_status_t status =
        ( lr11xx_status_t ) lr11xx_hal_direct_read( context, data, LR11XX_BL_UPDATER_GET_STATUS_CMD_LENGTH );

    if( status == LR11XX_STATUS_OK )
    {
        stat1->is_interrupt_active = ( ( data[0] & 0x01 ) != 0 ) ? true : false;
        stat1->command_status      = ( lr11xx_bootloader_updater_command_status_t ) ( data[0] >> 1 );

        stat2->is_running_from_flash = ( ( data[1] & 0x01 ) != 0 ) ? true : false;
        stat2->chip_mode             = ( lr11xx_bootloader_updater_chip_modes_t ) ( ( data[1] & 0x0F ) >> 1 );
        stat2->reset_status          = ( lr11xx_bootloader_updater_reset_status_t ) ( ( data[1] & 0xF0 ) >> 4 );

        *irq_status = ( ( lr11xx_bootloader_updater_irq_mask_t ) data[2] << 24 ) +
                      ( ( lr11xx_bootloader_updater_irq_mask_t ) data[3] << 16 ) +
                      ( ( lr11xx_bootloader_updater_irq_mask_t ) data[4] << 8 ) +
                      ( ( lr11xx_bootloader_updater_irq_mask_t ) data[5] << 0 );
    }

    return status;
}

lr11xx_status_t lr11xx_bootloader_updater_get_version( const void*                          context,
                                                       lr11xx_bootloader_updater_version_t* version )
{
    const uint8_t cbuffer[LR11XX_BL_UPDATER_VERSION_CMD_LENGTH] = {
        ( uint8_t ) ( LR11XX_BL_UPDATER_GET_VERSION_OC >> 8 ),
        ( uint8_t ) ( LR11XX_BL_UPDATER_GET_VERSION_OC >> 0 ),
    };
    uint8_t rbuffer[LR11XX_BL_UPDATER_GET_VERSION_RBUFFER_LENGTH] = { 0x00 };

    const lr11xx_status_t status = ( lr11xx_status_t ) lr11xx_hal_read(
        context, cbuffer, LR11XX_BL_UPDATER_VERSION_CMD_LENGTH, rbuffer, LR11XX_BL_UPDATER_GET_VERSION_RBUFFER_LENGTH );

    if( status == LR11XX_STATUS_OK )
    {
        version->hw   = rbuffer[0];
        version->type = rbuffer[1];
        version->fw   = ( ( uint16_t ) rbuffer[2] << 8 ) + ( uint16_t ) rbuffer[3];
    }

    return status;
}

lr11xx_status_t lr11xx_bootloader_updater_update_bootloader( const void* context )
{
    const uint8_t cbuffer[LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_CMD_LENGTH] = {
        ( uint8_t ) ( LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_OC >> 8 ),
        ( uint8_t ) ( LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_OC >> 0 ),
    };

    return ( lr11xx_status_t ) lr11xx_hal_write( context, cbuffer, LR11XX_BL_UPDATER_UPDATE_BOOTLOADER_CMD_LENGTH, 0,
                                                 0 );
}

lr11xx_status_t lr11xx_bootloader_updater_verify_bootloader(
    const void* context, lr11xx_bootloader_updater_verification_report_t* verification_report )
{
    const uint8_t cbuffer[LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_CMD_LENGTH] = {
        ( uint8_t ) ( LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_OC >> 8 ),
        ( uint8_t ) ( LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_OC >> 0 ),
    };

    uint8_t rbuffer[LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_RBUFFER_LENGTH] = { 0 };

    const lr11xx_hal_status_t hal_status =
        lr11xx_hal_read( context, cbuffer, LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_CMD_LENGTH, rbuffer,
                         LR11XX_BL_UPDATER_VERIFY_BOOTLOADER_RBUFFER_LENGTH );

    if( hal_status == LR11XX_HAL_STATUS_OK )
    {
        const uint8_t check_byte                                      = rbuffer[0];
        verification_report->checks.bootloader_signature_verified     = ( check_byte & 0x01 ) != 0;
        verification_report->checks.bootloader_version_verified       = ( check_byte & 0x02 ) != 0;
        verification_report->checks.bootlader_use_case_verified       = ( check_byte & 0x04 ) != 0;
        verification_report->checks.bootloader_version_major_verified = ( check_byte & 0x08 ) != 0;
        verification_report->checks.bootloader_version_minor_verified = ( check_byte & 0x10 ) != 0;
        verification_report->checks.bootloader_anti_rollback_verified = ( check_byte & 0x20 ) != 0;
        verification_report->bootloader_use_case                      = rbuffer[1];
        verification_report->bootloader_version_major                 = rbuffer[2];
        verification_report->bootloader_version_minor                 = rbuffer[3];
    }
    return ( lr11xx_status_t ) hal_status;
}

lr11xx_status_t lr11xx_bootloader_updater_reboot( const void* context, const bool stay_in_bootloader )
{
    const uint8_t cbuffer[LR11XX_BL_UPDATER_REBOOT_CMD_LENGTH] = {
        ( uint8_t ) ( LR11XX_BL_UPDATER_REBOOT_OC >> 8 ),
        ( uint8_t ) ( LR11XX_BL_UPDATER_REBOOT_OC >> 0 ),
        ( stay_in_bootloader == true ) ? 0x03 : 0x00,
    };

    return ( lr11xx_status_t ) lr11xx_hal_write( context, cbuffer, LR11XX_BL_UPDATER_REBOOT_CMD_LENGTH, 0, 0 );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
