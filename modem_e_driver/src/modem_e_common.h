/*!
 * @file      modem_e_common.h
 *
 * @brief     modem driver common definition
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

#ifndef MODEM_E_COMMON_H
#define MODEM_E_COMMON_H

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

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief Command group identifier
 */
typedef enum
{
    MODEM_E_GROUP_ID_BSP     = 0x0600,  //!< Group ID for BSP commands
    MODEM_E_GROUP_ID_MODEM   = 0x0601,  //!< Group ID for modem commands
    MODEM_E_GROUP_ID_LORAWAN = 0x0602,  //!< Group ID for LoRaWAN commands
    MODEM_E_GROUP_ID_RELAY   = 0x0603,  //!< Group ID for relay commands
} modem_e_api_group_id_t;

/*!
 * @brief Command return code (RC)
 */
typedef enum
{
    MODEM_E_RESPONSE_CODE_OK              = 0x00,  //!< Driver command executed successfully
    MODEM_E_RESPONSE_CODE_UNKOWN          = 0x01,  //!< Command code unknown
    MODEM_E_RESPONSE_CODE_NOT_IMPLEMENTED = 0x02,  //!< Command not implemented
    MODEM_E_RESPONSE_CODE_NOT_INITIALIZED = 0x03,  //!< Command not initialized
    MODEM_E_RESPONSE_CODE_INVALID         = 0x04,  //!< Invalid command parameters
    MODEM_E_RESPONSE_CODE_BUSY            = 0x05,  //!< Command cannot be executed now
    MODEM_E_RESPONSE_CODE_FAIL            = 0x06,  //!< Command execution failed
    MODEM_E_RESPONSE_CODE_BAD_CRC         = 0x08,  //!< CRC check failed
    MODEM_E_RESPONSE_CODE_BAD_SIZE        = 0x0A,  //!< Size check failed
    MODEM_E_RESPONSE_CODE_FRAME_ERROR     = 0x0F,  //!< SPI command checksum failed or CRC failed
    MODEM_E_RESPONSE_CODE_NO_TIME         = 0x10,  //!< Modem time is not synchronized
    MODEM_E_RESPONSE_CODE_NO_EVENT        = 0x12,  //!< No Event
} modem_e_response_code_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_COMMON_H

/* --- EOF ------------------------------------------------------------------ */
