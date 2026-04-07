/**
 * @file      lr11xx_bootloader_updater.h
 *
 * @brief     Bootloader updater driver definition for LR11XX
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

#ifndef LR11XX_BOOTLOADER_UPDATER_H
#define LR11XX_BOOTLOADER_UPDATER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr11xx_bootloader_updater_types.h"
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

typedef uint32_t lr11xx_bootloader_updater_irq_mask_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Return the status registers and interrupt flags
 *
 * @remark To simplify system integration, this function does not actually execute the GetStatus command, which would
 * require bidirectional SPI communication. It obtains the stat1, stat2, and irq_status values by performing an ordinary
 * SPI read (which is required to send null/NOP bytes on the MOSI line). This is possible since the LR11XX returns these
 * values automatically whenever a read that does not directly follow a response-carrying command is performed. Unlike
 * with the GetStatus command, however, the reset status information is NOT cleared by this command.
 *
 * @param [in] context Chip implementation context
 * @param [out] stat1 Content of status register 1
 * @param [out] stat2 Content of status register 2
 * @param [out] irq_status Interrupt flags
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_bootloader_updater_get_status( const void* context, lr11xx_bootloader_updater_stat1_t* stat1,
                                                      lr11xx_bootloader_updater_stat2_t*    stat2,
                                                      lr11xx_bootloader_updater_irq_mask_t* irq_status );

/**
 * @brief Return the version of the system (hardware and software)
 *
 * @param [in] context Chip implementation context
 * @param [out] version Pointer to the structure holding the system version
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_bootloader_updater_get_version( const void*                          context,
                                                       lr11xx_bootloader_updater_version_t* version );

/**
 * @brief Update bootloader
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_bootloader_updater_update_bootloader( const void* context );

/**
 * @brief
 *
 * @param [in] context Chip implementation context
 * @param [out] verification_report
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_bootloader_updater_verify_bootloader(
    const void* context, lr11xx_bootloader_updater_verification_report_t* verification_report );

/**
 * @brief Software reboot of the chip.
 *
 * This method should be used to reboot the chip in a specified mode.
 *
 * @param [in] context Chip implementation context
 * @param [in] stay_in_bootloader Selector to stay in bootloader or execute flash code after reboot. If true, the
 * bootloader will not execute the flash code but activate SPI interface to allow firmware upgrade
 *
 * @returns Operation status
 */
lr11xx_status_t lr11xx_bootloader_updater_reboot( const void* context, const bool stay_in_bootloader );

#ifdef __cplusplus
}
#endif

#endif  // LR11XX_BOOTLOADER_UPDATER_H

/* --- EOF ------------------------------------------------------------------ */
