/*!
 * @file      lr11xx_firmware_update.c
 *
 * @brief     LR11XX firmware update implementation
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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

#include "lr11xx_bootloader.h"
#include "lr11xx_system.h"
#include "lr11xx_firmware_update.h"
#include "lr1110_modem_lorawan.h"
#include "lr1121_modem_modem.h"
#include "system.h"
#include <stdint.h>

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

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static gpio_t lr11xx_busy = { LR11XX_BUSY_PORT, LR11XX_BUSY_PIN };

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

bool lr11xx_is_chip_in_production_mode( uint8_t type );

bool lr11xx_is_fw_compatible_with_chip( lr11xx_fw_update_t update, uint16_t bootloader_version );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr11xx_fw_update_status_t lr11xx_update_firmware( void* radio, lr11xx_fw_update_t fw_update_direction,
                                                  uint32_t fw_expected, const uint32_t* buffer, uint32_t length )
{
    lr11xx_bootloader_version_t version_bootloader = { 0 };

    printf( "Reset the chip...\n" );

    system_gpio_init_direction_state( lr11xx_busy, SYSTEM_GPIO_PIN_DIRECTION_OUTPUT, SYSTEM_GPIO_PIN_STATE_LOW );

    lr11xx_system_reset( radio );

    system_time_wait_ms( 500 );
    system_gpio_init_direction_state( lr11xx_busy, SYSTEM_GPIO_PIN_DIRECTION_INPUT, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 100 );

    printf( "> Reset done!\n" );

    lr11xx_bootloader_get_version( radio, &version_bootloader );
    printf( "Chip in bootloader mode:\n" );
    printf( " - Chip type               = 0x%02X (0xDF for production)\n", version_bootloader.type );
    printf( " - Chip hardware version   = 0x%02X (0x22 for V2C)\n", version_bootloader.hw );
    printf( " - Chip bootloader version = 0x%04X \n", version_bootloader.fw );

    if( lr11xx_is_chip_in_production_mode( version_bootloader.type ) == false )
    {
        return LR11XX_FW_UPDATE_WRONG_CHIP_TYPE;
    }

    if( lr11xx_is_fw_compatible_with_chip( fw_update_direction, version_bootloader.fw ) == false )
    {
        return LR11XX_FW_UPDATE_WRONG_CHIP_TYPE;
    };

    lr11xx_bootloader_pin_t      pin      = { 0x00 };
    lr11xx_bootloader_chip_eui_t chip_eui = { 0x00 };
    lr11xx_bootloader_join_eui_t join_eui = { 0x00 };

    lr11xx_bootloader_read_pin( radio, pin );
    lr11xx_bootloader_read_chip_eui( radio, chip_eui );
    lr11xx_bootloader_read_join_eui( radio, join_eui );

    printf( "PIN is     0x%02X%02X%02X%02X\n", pin[0], pin[1], pin[2], pin[3] );
    printf( "ChipEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n", chip_eui[0], chip_eui[1], chip_eui[2], chip_eui[3],
            chip_eui[4], chip_eui[5], chip_eui[6], chip_eui[7] );
    printf( "JoinEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n", join_eui[0], join_eui[1], join_eui[2], join_eui[3],
            join_eui[4], join_eui[5], join_eui[6], join_eui[7] );

    printf( "Start flash erase...\n" );
    lr11xx_bootloader_erase_flash( radio );
    printf( "> Flash erase done!\n" );

    printf( "Start flashing firmware...\n" );
    lr11xx_bootloader_write_flash_encrypted_full( radio, 0, buffer, length );
    printf( "> Flashing done!\n" );

    printf( "Rebooting...\n" );
    lr11xx_bootloader_reboot( radio, false );
    printf( "> Reboot done!\n" );

    switch( fw_update_direction )
    {
    case LR1110_FIRMWARE_UPDATE_TO_TRX:
    case LR1120_FIRMWARE_UPDATE_TO_TRX:
    case LR1121_FIRMWARE_UPDATE_TO_TRX:
    {
        lr11xx_system_version_t version_trx = { 0x00 };
        lr11xx_system_uid_t     uid         = { 0x00 };

        lr11xx_system_get_version( radio, &version_trx );
        printf( "Chip in transceiver mode:\n" );
        printf( " - Chip type             = 0x%02X\n", version_trx.type );
        printf( " - Chip hardware version = 0x%02X\n", version_trx.hw );
        printf( " - Chip firmware version = 0x%04X\n", version_trx.fw );

        lr11xx_system_read_uid( radio, uid );

        if( version_trx.fw == fw_expected )
        {
            return LR11XX_FW_UPDATE_OK;
        }
        else
        {
            return LR11XX_FW_UPDATE_ERROR;
        }
        break;
    }
    case LR1110_FIRMWARE_UPDATE_TO_MODEM_V1:
    {
        lr1110_modem_version_t version_modem = { 0 };

        system_time_wait_ms( 2000 );

        lr1110_modem_get_version( radio, &version_modem );
        printf( "Chip in LoRa Basics Modem-E mode:\n" );
        printf( " - Chip bootloader version = 0x%08x\n", version_modem.bootloader );
        printf( " - Chip firmware version   = 0x%08x\n", version_modem.firmware );
        printf( " - Chip LoRaWAN version    = 0x%04x\n", version_modem.lorawan );

        uint32_t fw_version = ( ( uint32_t )( version_modem.functionality ) << 24 ) + version_modem.firmware;

        if( fw_version == fw_expected )
        {
            return LR11XX_FW_UPDATE_OK;
        }
        else
        {
            return LR11XX_FW_UPDATE_ERROR;
        }
        break;
    }
    case LR1121_FIRMWARE_UPDATE_TO_MODEM_V2:
    {
        lr1121_modem_version_t version_modem = { 0 };

        system_time_wait_ms( 2000 );

        lr1121_modem_get_modem_version( radio, &version_modem );
        printf( "Chip in LoRa Basics Modem-E mode:\n" );
        printf( " - Chip use case version: 0x%02X\n", version_modem.use_case );
        printf( " - Chip modem major version: 0x%02X\n", version_modem.modem_major );
        printf( " - Chip modem minor version: 0x%02X\n", version_modem.modem_minor );
        printf( " - Chip modem patch version: 0x%02X\n", version_modem.modem_patch );
        printf( " - Chip lbm major version: 0x%02X\n", version_modem.lbm_major );
        printf( " - Chip lbm minor version: 0x%02X\n", version_modem.lbm_minor );
        printf( " - Chip lbm patch version: 0x%02X\n", version_modem.lbm_patch );

        uint32_t fw_version =
            ( ( uint32_t )( version_modem.use_case ) << 24 ) + ( ( uint32_t )( version_modem.modem_major ) << 16 ) +
            ( ( uint32_t )( version_modem.modem_minor << 8 ) ) + ( uint32_t )( version_modem.modem_patch );

        if( fw_version == fw_expected )
        {
            return LR11XX_FW_UPDATE_OK;
        }
        else
        {
            return LR11XX_FW_UPDATE_ERROR;
        }
        break;
    }
    }

    return LR11XX_FW_UPDATE_ERROR;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

bool lr11xx_is_chip_in_production_mode( uint8_t type )
{
    return ( type == LR11XX_TYPE_PRODUCTION_MODE ) ? true : false;
}

bool lr11xx_is_fw_compatible_with_chip( lr11xx_fw_update_t update, uint16_t bootloader_version )
{
    if( ( ( update == LR1110_FIRMWARE_UPDATE_TO_TRX ) || ( update == LR1110_FIRMWARE_UPDATE_TO_MODEM_V1 ) ) &&
        ( bootloader_version != 0x6500 ) )
    {
        return false;
    }
    else if( ( update == LR1120_FIRMWARE_UPDATE_TO_TRX ) && ( bootloader_version != 0x2000 ) )
    {
        return false;
    }
    else if( ( ( update == LR1121_FIRMWARE_UPDATE_TO_TRX ) || ( update == LR1121_FIRMWARE_UPDATE_TO_MODEM_V2 ) ) &&
             ( bootloader_version != 0x2100 ) )
    {
        return false;
    }

    return true;
}

/* --- EOF ------------------------------------------------------------------ */
