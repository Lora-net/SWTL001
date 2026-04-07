/*!
 * @file      conditional_bootloader_handling.h
 *
 * @brief     Helper header for compile time determination of bootloader handling.
 *
 * This file provides macro CONDITIONALLY_HANDLE_BOOTLOADER that can be used unconditionally.
 * If the macro HAS_BOOTLOADER is defined, this file provides also a static function to
 * handle the bootloader update, and the macro CONDITIONALLY_HANDLE_BOOTLOADER expands to its call.
 * If HAS_BOOTLOADER is not defined, macro CONDITIONALLY_HANDLE_BOOTLOADER expands to nothing.
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

#ifndef CONDITIONAL_BOOTLOADER_HANDLING_H
#define CONDITIONAL_BOOTLOADER_HANDLING_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#ifdef HAS_BOOTLOADER
#include <stdbool.h>
#include "lr11xx_bootloader_update.h"
#endif  // HAS_BOOTLOADER

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

#ifdef HAS_BOOTLOADER
#define CONDITIONALLY_HANDLE_BOOTLOADER( rr ) handle_bootloader( rr )

static bool handle_bootloader( void* radio )
{
    // Check if a bootloader update is possible
    const bool should_update_bootloader = lr11xx_bootloader_should_update( radio );

    if( should_update_bootloader == true )
    {
        printf( "Updating bootloader...\n" );
        // The bootloader should be updated
        const lr11xx_fw_update_status_t status = lr11xx_bootloader_update( radio );
        if( status == LR11XX_FW_UPDATE_OK )
        {
            printf( "> Bootloader update success.\n" );
            // Reset the radio to bootloader
            lr11xx_reset_to_bootloader( radio );
            return true;
        }
        else
        {
            // Update should be done but failed. Return error.
            printf( "> Error: bootloader update failed.\n" );
            return false;
        }
    }
    else
    {
        // There is no need to update
        printf( "> Bootloader already up-to-date: no update.\n" );
        return true;
    }
}
#else
#define CONDITIONALLY_HANDLE_BOOTLOADER( rr ) true
#endif  // HAS_BOOTLOADER

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

#ifdef __cplusplus
}
#endif

#endif  // CONDITIONAL_BOOTLOADER_HANDLING_H

/* --- EOF ------------------------------------------------------------------ */
