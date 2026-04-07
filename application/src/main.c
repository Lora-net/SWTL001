/*!
 * @file      main.c
 *
 * @brief     LR11XX updater tool application entry point
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

#if defined IMAGE_HEADER_FILE
#include IMAGE_HEADER_FILE
#else
#error IMAGE_HEADER_FILE is not defined, please define it or include firmware image instead of this message
#endif

#include "configuration.h"
#include "system.h"
#include "stdio.h"
#include "string.h"
#include "lr11xx_firmware_update.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "gui.h"
#include "version.h"
#include "conditional_bootloader_handling.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#ifdef HAS_BOOTLOADER
static const bool has_bootloader = true;
#else
static const bool has_bootloader = false;
#endif  // HAS_BOOTLOADER

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

radio_t radio = {
    SPI1,
    { LR11XX_NSS_PORT, LR11XX_NSS_PIN },
    { LR11XX_RESET_PORT, LR11XX_RESET_PIN },
    { LR11XX_IRQ_PORT, LR11XX_IRQ_PIN },
    { LR11XX_BUSY_PORT, LR11XX_BUSY_PIN },
};

static gpio_t lr11xx_led_tx   = { LR11XX_LED_TX_PORT, LR11XX_LED_TX_PIN };
static gpio_t lr11xx_led_rx   = { LR11XX_LED_RX_PORT, LR11XX_LED_RX_PIN };
static gpio_t lr11xx_led_scan = { LR11XX_LED_SCAN_PORT, LR11XX_LED_SCAN_PIN };

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

int main( void )
{
    bool is_updated = false;

    system_init( );

    system_time_wait_ms( 2000 );

    lv_init( );
    lv_port_disp_init( );

    printf( "LR11XX updater tool %s%s\n", UPDATER_TOOL_VERSION, has_bootloader ? " with bootloader" : "" );

    gui_init( LR11XX_FIRMWARE_UPDATE_TO, LR11XX_FIRMWARE_VERSION, has_bootloader );

    switch( LR11XX_FIRMWARE_UPDATE_TO )
    {
    case LR1110_FIRMWARE_UPDATE_TO_TRX:
    {
        printf( "Update LR1110 to transceiver firmware 0x%04x\n", LR11XX_FIRMWARE_VERSION );
        break;
    }
    case LR1120_FIRMWARE_UPDATE_TO_TRX:
    {
        printf( "Update LR1120 to transceiver firmware 0x%04x\n", LR11XX_FIRMWARE_VERSION );
        break;
    }
    case LR1121_FIRMWARE_UPDATE_TO_TRX:
    {
        printf( "Update LR1121 to transceiver firmware 0x%04x\n", LR11XX_FIRMWARE_VERSION );
        break;
    }
    case LR1110_FIRMWARE_UPDATE_TO_MODEM_V1:
    {
        printf( "Update LR1110 to modem firmware 0x%06x\n", LR11XX_FIRMWARE_VERSION );
        break;
    }
    case LR1121_FIRMWARE_UPDATE_TO_MODEM_V2:
    {
        printf( "Update LR1121 to modem firmware 0x%06x\n", LR11XX_FIRMWARE_VERSION );
        break;
    }
    case LR1110_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1120_FIRMWARE_UPDATE_TO_BL_LOADER:
    case LR1121_FIRMWARE_UPDATE_TO_BL_LOADER:
    {
        printf( "Direct usage of bootloader loader should not be attempted. Aborting.\n" );
        gui_update( "ERROR\nDirect update to bootloader loader should not be attempted.\nAborting." );
        return 1;
    }
    }

    while( 1 )
    {
        lv_task_handler( );

        if( is_updated == false )
        {
            system_gpio_set_pin_state( lr11xx_led_scan, SYSTEM_GPIO_PIN_STATE_HIGH );

            // Reset the radio to bootloader
            lr11xx_reset_to_bootloader( &radio );

            const bool bootloader_updated = CONDITIONALLY_HANDLE_BOOTLOADER( &radio );

            lr11xx_fw_update_status_t status = LR11XX_FW_UPDATE_ERROR;
            if( bootloader_updated == true )
            {
                const lr11xx_fw_update_status_t update_status = lr11xx_update_firmware(
                    &radio, LR11XX_FIRMWARE_UPDATE_TO, LR11XX_FIRMWARE_VERSION, lr11xx_firmware_image,
                    sizeof( lr11xx_firmware_image ) / sizeof( lr11xx_firmware_image[0] ) );

                if( update_status == LR11XX_FW_UPDATE_OK )
                {
                    // Update went well: proceed with post flash verification and set status
                    status = lr11xx_update_post_flash_reboot_and_verification( &radio, LR11XX_FIRMWARE_UPDATE_TO,
                                                                               LR11XX_FIRMWARE_VERSION );
                }
                else
                {
                    // Update went incorrect: copy status to update status and propagate for status reporting
                    status = update_status;
                }
            }
            else
            {
                status = LR11XX_FW_UPDATE_BOOTLOADER_FAILED;
            }

            system_gpio_set_pin_state( lr11xx_led_scan, SYSTEM_GPIO_PIN_STATE_LOW );

            switch( status )
            {
            case LR11XX_FW_UPDATE_OK:
            {
                system_gpio_set_pin_state( lr11xx_led_rx, SYSTEM_GPIO_PIN_STATE_HIGH );
                gui_update( "UPDATE DONE!\nPlease flash another application\n(like EVK Demo App)" );
                printf( "Expected firmware running!\n" );
                printf( "Please flash another application (like EVK Demo App).\n" );
                break;
            }
            case LR11XX_FW_UPDATE_WRONG_CHIP_TYPE:
            {
                system_gpio_set_pin_state( lr11xx_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
                gui_update( "WRONG CHIP TYPE" );
                printf( "Wrong chip type!\n" );
                break;
            }
            case LR11XX_FW_UPDATE_ERROR:
            {
                system_gpio_set_pin_state( lr11xx_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
                gui_update( "ERROR\nWrong firmware version\nPlease retry" );
                printf( "Error! Wrong firmware version - please retry.\n" );
                break;
            }
            case LR11XX_FW_UPDATE_BOOTLOADER_FAILED:
            {
                system_gpio_set_pin_state( lr11xx_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
                gui_update( "ERROR\nBootloader update failed" );
                printf( "Error! Bootloader update failed.\n" );
                break;
            }
            }

            is_updated = true;
        }
    };
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
