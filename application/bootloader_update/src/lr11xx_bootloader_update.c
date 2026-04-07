/*!
 * @file      lr11xx_bootloader_update.c
 *
 * @brief     LR11XX bootloader update implementation
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

#include "lr11xx_bootloader_update.h"
#include "lr11xx_bootloader_updater.h"
#include "lr11xx_firmware_update.h"
#include "lr11xx_bootloader.h"
#include "lr11xx_update_utils.h"

#if defined BOOTLOADER_LOADER_IMAGE_HEADER_FILE
// With bootloader loader, there is a second firmware image to embed in the code (the first one being the actual
// transceiver or modem firmware).
// However both firmwares are referred with global variable lr11xx_firmware_image. Therefore the linker generates error
// for multiple definitions of lr11xx_firmware_image.
// This macro definitin before inclusion of the header containing bootloader loader firmware is used to rename the
// global lr11xx_firmware_image to lr11xx_firmware_loader_image.
#define lr11xx_firmware_image lr11xx_firmware_loader_image
#include BOOTLOADER_LOADER_IMAGE_HEADER_FILE
#else
#error BOOTLOADER_LOADER_IMAGE_HEADER_FILE is not defined, please define it or include bootloader loader firmware image instead of this message
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

// These constants values are provided by header BOOTLOADER_LOADER_IMAGE_HEADER_FILE
const lr11xx_fw_update_t bootloader_updater_firmware_direction  = LR11XX_FIRMWARE_UPDATE_TO;
const uint16_t           bootloader_updater_firmware_version    = LR11XX_FIRMWARE_VERSION;
const uint32_t*          bootloader_updater_firmware_image      = lr11xx_firmware_loader_image;
const uint32_t           bootloader_updater_firmware_image_size = LR11XX_FIRMWARE_IMAGE_SIZE;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static bool lr11xx_bootloader_update_verify_checks(
    const lr11xx_bootloader_updater_verification_report_t* verification_report );

static bool lr11xx_is_running_expected_bootloader( void* radio );

static uint16_t lr11xx_get_expected_bootloader_version_after_flash( uint16_t bootloader_updater_version );

static lr11xx_fw_update_status_t lr11xx_bootloader_update_post_flash_reboot_and_verification(
    void* radio, lr11xx_fw_update_t fw_update_direction, uint32_t fw_expected );

static bool lr11xx_is_bootloader_updater_firmware_compatible_with_chip( lr11xx_fw_update_t bootloader_updater_direction,
                                                                        uint16_t           bootloader_version,
                                                                        uint16_t           bootloader_updater_version );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr11xx_fw_update_status_t lr11xx_bootloader_update( void* radio )
{
    // Load bootloader updater firmware
    printf( "Loading bootloader updater firmware...\n" );
    const lr11xx_fw_update_status_t load_bootloader_fw_status =
        lr11xx_update_firmware( radio, bootloader_updater_firmware_direction, bootloader_updater_firmware_version,
                                bootloader_updater_firmware_image, bootloader_updater_firmware_image_size );

    if( load_bootloader_fw_status == LR11XX_FW_UPDATE_OK )
    {
        printf( "> done\n" );

        const lr11xx_fw_update_status_t load_bootloader_post_verification_status =
            lr11xx_bootloader_update_post_flash_reboot_and_verification( radio, bootloader_updater_firmware_direction,
                                                                         bootloader_updater_firmware_version );

        // Handle firmware update return code
        if( load_bootloader_post_verification_status == LR11XX_FW_UPDATE_OK )
        {
            // Request bootloader update
            lr11xx_bootloader_updater_update_bootloader( radio );

            // Get status to wait for bootloader update termination
            lr11xx_bootloader_updater_stat1_t    stat1      = { 0 };
            lr11xx_bootloader_updater_stat2_t    stat2      = { 0 };
            lr11xx_bootloader_updater_irq_mask_t irq_status = 0;
            lr11xx_bootloader_updater_get_status( radio, &stat1, &stat2, &irq_status );

            // Verify bootloader update
            lr11xx_bootloader_updater_verification_report_t verification_report = { 0 };
            lr11xx_bootloader_updater_verify_bootloader( radio, &verification_report );
            const bool bootloader_success = lr11xx_bootloader_update_verify_checks( &verification_report );
            if( bootloader_success == true )
            {
                // Reboot and check version
                // This reboot request to run to FW and not stay in bootloader. However, as the bootloader has jut been
                // updated, it is expected that it refuses to execute the FW, as the curent FW is made to run on
                // previous bootloader. Therefore the version is tested after reboot to confirm the chip stays in
                // bootloader, suggesting that the new bootloader is correctly running, and detected the current FW
                // should not be executed.
                lr11xx_bootloader_updater_reboot( radio, false );
                if( lr11xx_is_running_expected_bootloader( radio ) == true )
                {
                    // Running correct bootloader. Return with success
                    printf( "> Bootloader updated.\n" );
                    return LR11XX_FW_UPDATE_OK;
                }
                else
                {
                    // Unexpected bootloader running
                    printf( "> Error: Bootloader not behaving as expected.\n" );
                    return LR11XX_FW_UPDATE_ERROR;
                }
            }
            else
            {
                // Return bootloader update failure
                printf( "> Error: Bootloader install check fails.\n" );
                return LR11XX_FW_UPDATE_ERROR;
            }
        }
        else
        {
            // Failed to flash bootloader updater firmware: return failure
            printf( "> Error: incorrect firmware running after load of bootloader updater.\n" );
            return load_bootloader_post_verification_status;
        }
    }
    else
    {
        // Fail to flash bootloader updater firmware
        printf( "> Error: failed to load bootloader updater firmware.\n" );
        return load_bootloader_fw_status;
    }
}

bool lr11xx_bootloader_should_update( void* radio )
{
    lr11xx_bootloader_version_t bootloader_version = { 0 };
    lr11xx_bootloader_get_version( radio, &bootloader_version );
    return lr11xx_is_bootloader_updater_firmware_compatible_with_chip(
        bootloader_updater_firmware_direction, bootloader_version.fw, bootloader_updater_firmware_version );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

lr11xx_fw_update_status_t lr11xx_bootloader_update_post_flash_reboot_and_verification(
    void* radio, lr11xx_fw_update_t fw_update_direction, uint32_t fw_expected )
{
    printf( "Rebooting...\n" );
    lr11xx_bootloader_reboot( radio, false );
    printf( "> Reboot done!\n" );

    switch( fw_update_direction )
    {
    case LR1110_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1120_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1121_FIRMWARE_UPDATE_TO_BL_LOADER:
    {
        lr11xx_bootloader_updater_version_t version_bootloader = { 0x00 };

        lr11xx_bootloader_updater_get_version( radio, &version_bootloader );
        printf( "Chip in transceiver mode:\n" );
        printf( " - Chip type             = 0x%02X\n", version_bootloader.type );
        printf( " - Chip hardware version = 0x%02X\n", version_bootloader.hw );
        printf( " - Chip firmware version = 0x%04X\n", version_bootloader.fw );

        if( version_bootloader.fw == fw_expected )
        {
            return LR11XX_FW_UPDATE_OK;
        }
        else
        {
            return LR11XX_FW_UPDATE_ERROR;
        }
        break;
    }
    default:
    {
        return LR11XX_FW_UPDATE_ERROR;
    }
    }
}

bool lr11xx_bootloader_update_verify_checks(
    const lr11xx_bootloader_updater_verification_report_t* verification_report )
{
    return ( verification_report->checks.bootlader_use_case_verified == true ) &&
           ( verification_report->checks.bootloader_anti_rollback_verified == true ) &&
           ( verification_report->checks.bootloader_signature_verified == true ) &&
           ( verification_report->checks.bootloader_version_major_verified == true ) &&
           ( verification_report->checks.bootloader_version_minor_verified == true ) &&
           ( verification_report->checks.bootloader_version_verified == true );
}

bool lr11xx_is_running_expected_bootloader( void* radio )
{
    lr11xx_bootloader_version_t version_bootloader = { 0 };
    lr11xx_bootloader_get_version( radio, &version_bootloader );

    return version_bootloader.fw ==
           lr11xx_get_expected_bootloader_version_after_flash( bootloader_updater_firmware_version );
}

uint16_t lr11xx_get_expected_bootloader_version_after_flash( uint16_t bootloader_updater_version )
{
    switch( bootloader_updater_version )
    {
    case 0x6500:
    {
        return 0x1001;
    }
    case 0x2000:
    {
        return 0x2001;
    }
    case 0x2100:
    {
        return 0x2101;
    }
    default:
    {
        return 0;
    }
    }
}

bool lr11xx_is_bootloader_updater_firmware_compatible_with_chip( lr11xx_fw_update_t bootloader_updater_direction,
                                                                 uint16_t           bootloader_version,
                                                                 uint16_t           bootloader_updater_version )
{
    switch( bootloader_updater_direction )
    {
    case LR1110_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1120_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1121_FIRMWARE_UPDATE_TO_BL_LOADER:
    {
        // The version of the bootloader updater firmware must match the currently running bootloader version for it to
        // be compatible
        return bootloader_version == bootloader_updater_version;
    }
    default:
    {
        return false;
    }
    }
}

/* --- EOF ------------------------------------------------------------------ */
