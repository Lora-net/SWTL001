/*!
 * @file      modem_e_lorawan.c
 *
 * @brief     LoRaWAN driver implementation for Modem-E
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
#include "modem_e_lorawan.h"
#include "modem_e_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define MODEM_E_GET_LORAWAN_VERSION_CMD_LENGTH ( 3 )
#define MODEM_E_GET_DEV_EUI_CMD_LENGTH ( 3 )
#define MODEM_E_SET_DEV_EUI_CMD_LENGTH ( 3 )
#define MODEM_E_GET_JOIN_EUI_CMD_LENGTH ( 3 )
#define MODEM_E_SET_JOIN_EUI_CMD_LENGTH ( 3 )
#define MODEM_E_SET_NWK_KEY_CMD_LENGTH ( 3 )
#define MODEM_E_SET_APP_KEY_CMD_LENGTH ( 3 )
#define MODEM_E_DERIVE_KEYS_CMD_LENGTH ( 3 )
#define MODEM_E_GET_CLASS_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CLASS_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_REGION_CMD_LENGTH ( 3 )
#define MODEM_E_SET_REGION_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_JOIN_CMD_LENGTH ( 3 )
#define MODEM_E_LEAVE_NETWORK_CMD_LENGTH ( 3 )
#define MODEM_E_GET_NEXT_TX_MAX_PAYLOAD_CMD_LENGTH ( 3 )
#define MODEM_E_REQUEST_TX_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_REQUEST_EMPTY_TX_CMD_LENGTH ( 3 + 3 )
#define MODEM_E_EMERGENCY_TX_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_GET_DOWNLINK_DATA_SIZE_CMD_LENGTH ( 3 )
#define MODEM_E_GET_DOWNLINK_DATA_CMD_LENGTH ( 3 )
#define MODEM_E_GET_DOWNLINK_METADATA_CMD_LENGTH ( 3 )
#define MODEM_E_GET_LOST_CONNECTION_COUNTER_CMD_LENGTH ( 3 )
#define MODEM_E_GET_NETWORK_TYPE_CMD_LENGTH ( 3 )
#define MODEM_E_SET_NETWORK_TYPE_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_CERTIFICATION_MODE_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CERTIFICATION_MODE_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_DUTY_CYCLE_STATUS_CMD_LENGTH ( 3 )
#define MODEM_E_GET_AVAILABLE_DATA_RATE_CMD_LENGTH ( 3 )
#define MODEM_E_GET_ADR_PROFILE_CMD_LENGTH ( 3 )
#define MODEM_E_SET_ADR_PROFILE_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_SET_JOIN_DATA_RATE_DISTRIBUTION_CMD_LENGTH ( 3 )
#define MODEM_E_GET_NB_TRANS_CMD_LENGTH ( 3 )
#define MODEM_E_SET_NB_TRANS_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH ( 3 )
#define MODEM_E_SET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_GET_LBT_STATE_CMD_LENGTH ( 3 )
#define MODEM_E_SET_LBT_STATE_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_LBT_PARAMS_CMD_LENGTH ( 3 )
#define MODEM_E_SET_LBT_PARAMS_CMD_LENGTH ( 3 + 10 )
#define MODEM_E_GET_CSMA_STATE_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CSMA_STATE_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_CSMA_PARAMS_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CSMA_PARAMS_CMD_LENGTH ( 3 + 3 )
#define MODEM_E_LORAWAN_MAC_REQUEST_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_LORAWAN_MAC_TIME_CMD_LENGTH ( 3 )
#define MODEM_E_GET_LINK_CHECK_DATA_CMD_LENGTH ( 3 )
#define MODEM_E_SET_BATTERY_LEVEL_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_GET_CLASS_B_PING_SLOT_PERIODICITY_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CLASS_B_PING_SLOT_PERIODICITY_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_MULTICAST_GROUP_CONFIG_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_SET_MULTICAST_GROUP_CONFIG_CMD_LENGTH ( 3 + 37 )
#define MODEM_E_START_SESSION_MULTICAST_CLASS_C_CMD_LENGTH ( 3 + 6 )
#define MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_STOP_SESSION_MULTICAST_CLASS_C_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_C_CMD_LENGTH ( 3 )
#define MODEM_E_START_SESSION_MULTICAST_CLASS_B_CMD_LENGTH ( 3 + 7 )
#define MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_STOP_SESSION_MULTICAST_CLASS_B_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_B_CMD_LENGTH ( 3 )
#define MODEM_E_START_ALC_SYNC_SERVICE_CMD_LENGTH ( 3 )
#define MODEM_E_STOP_ALC_SYNC_SERVICE_CMD_LENGTH ( 3 )
#define MODEM_E_ALC_SYNC_GET_TIME_CMD_LENGTH ( 3 )
#define MODEM_E_ALC_SYNC_TRIG_REQUEST_CMD_LENGTH ( 3 )
#define MODEM_E_FUOTA_GET_FILE_SIZE_CRC_CMD_LENGTH ( 3 )
#define MODEM_E_FUOTA_GET_FILE_FRAGMENT_CMD_LENGTH ( 3 + 8 )
#define MODEM_E_CONNECT_WITH_ABP_CMD_LENGTH ( 3 + 36 )
#define MODEM_E_SET_USER_ADR_ACK_LIMIT_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_GET_USER_ADR_ACK_LIMIT_CMD_LENGTH ( 3 )
#define MODEM_E_GET_CHANNEL_MASK_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CHANNEL_MASK_CMD_LENGTH ( 3 )

#define MODEM_E_SET_CHANNEL_MASK_SINGLE_CONFIGURATION_LENGTH ( 3 )
#define MODEM_E_SET_CHANNEL_MASK_MAX_N_CONFIGURATIONS ( 6 )
#define MODEM_E_SET_CHANNEL_MASK_DBUFFER_MAX_LENGTH \
    ( MODEM_E_SET_CHANNEL_MASK_SINGLE_CONFIGURATION_LENGTH * MODEM_E_SET_CHANNEL_MASK_MAX_N_CONFIGURATIONS )

#define MODEM_E_GET_LORAWAN_VERSION_RBUFFER_LENGTH ( 8 )
#define MODEM_E_DEV_EUI_BUFFER_LENGTH ( 8 )
#define MODEM_E_JOIN_EUI_BUFFER_LENGTH ( 8 )
#define MODEM_E_GET_DOWNLINK_DATA_SIZE_RBUFFER_LENGTH ( 2 )
#define MODEM_E_DOWNLINK_METADATA_RBUFFER_LENGTH ( 11 )
#define MODEM_E_LBT_PARAMS_RBUFFER_LENGTH ( 10 )
#define MODEM_E_CSMA_PARAMS_RBUFFER_LENGTH ( 3 )
#define MODEM_E_GET_LORAWAN_MAC_TIME_RBUFFER_LENGTH ( 8 )
#define MODEM_E_GET_LOST_CONNECTION_COUNTER_RBUFFER_LENGTH ( 6 )
#define MODEM_E_GET_MULTICAST_GROUP_CONFIG_RBUFFER_LENGTH ( 4 )
#define MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_RBUFFER_LENGTH ( 6 )
#define MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_RBUFFER_LENGTH ( 8 )
#define MODEM_E_ALC_SYNC_GET_TIME_RBUFFER_LENGTH ( 4 )
#define MODEM_E_FUOTA_GET_FILE_SIZE_CRC_RBUFFER_LENGTH ( 8 )

#define MODEM_E_NWKSKEY_LENGTH ( 16 )
#define MODEM_E_APPSKEY_LENGTH ( 16 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operation code command
 */
enum
{
    MODEM_E_GET_LORAWAN_VERSION_CMD                  = 0x00,
    MODEM_E_GET_DEV_EUI_CMD                          = 0x01,
    MODEM_E_SET_DEV_EUI_CMD                          = 0x02,
    MODEM_E_GET_JOIN_EUI_CMD                         = 0x03,
    MODEM_E_SET_JOIN_EUI_CMD                         = 0x04,
    MODEM_E_SET_NWK_KEY_CMD                          = 0x05,
    MODEM_E_SET_APP_KEY_CMD                          = 0x06,
    MODEM_E_DERIVE_KEYS_CMD                          = 0x07,
    MODEM_E_GET_CLASS_CMD                            = 0x08,
    MODEM_E_SET_CLASS_CMD                            = 0x09,
    MODEM_E_GET_REGION_CMD                           = 0x0B,
    MODEM_E_SET_REGION_CMD                           = 0x0C,
    MODEM_E_JOIN_CMD                                 = 0x0D,
    MODEM_E_LEAVE_NETWORK_CMD                        = 0x0E,
    MODEM_E_GET_NEXT_TX_MAX_PAYLOAD_CMD              = 0x11,
    MODEM_E_REQUEST_TX_CMD                           = 0x12,
    MODEM_E_REQUEST_EMPTY_TX_CMD                     = 0x13,
    MODEM_E_EMERGENCY_TX_CMD                         = 0x14,
    MODEM_E_GET_DOWNLINK_DATA_SIZE_CMD               = 0x15,
    MODEM_E_GET_DOWNLINK_DATA_CMD                    = 0x16,
    MODEM_E_GET_DOWNLINK_METADATA_CMD                = 0x17,
    MODEM_E_GET_LOST_CONNECTION_COUNTER_CMD          = 0x18,
    MODEM_E_GET_NETWORK_TYPE_CMD                     = 0x19,
    MODEM_E_SET_NETWORK_TYPE_CMD                     = 0x1A,
    MODEM_E_GET_CERTIFICATION_MODE_CMD               = 0x1B,
    MODEM_E_SET_CERTIFICATION_MODE_CMD               = 0x1C,
    MODEM_E_GET_DUTY_CYCLE_STATUS_CMD                = 0x1D,
    MODEM_E_GET_AVAILABLE_DATA_RATE_CMD              = 0x1F,
    MODEM_E_GET_ADR_PROFILE_CMD                      = 0x20,
    MODEM_E_SET_ADR_PROFILE_CMD                      = 0x21,
    MODEM_E_SET_JOIN_DATA_RATE_DISTRIBUTION_CMD      = 0x22,
    MODEM_E_GET_NB_TRANS_CMD                         = 0x23,
    MODEM_E_SET_NB_TRANS_CMD                         = 0x24,
    MODEM_E_GET_ADR_ACK_LIMIT_DELAY_CMD              = 0x25,
    MODEM_E_SET_ADR_ACK_LIMIT_DELAY_CMD              = 0x26,
    MODEM_E_GET_LBT_STATE_CMD                        = 0x27,
    MODEM_E_SET_LBT_STATE_CMD                        = 0x28,
    MODEM_E_GET_LBT_PARAMS_CMD                       = 0x29,
    MODEM_E_SET_LBT_PARAMS_CMD                       = 0x2A,
    MODEM_E_GET_CSMA_STATE_CMD                       = 0x2B,
    MODEM_E_SET_CSMA_STATE_CMD                       = 0x2C,
    MODEM_E_GET_CSMA_PARAMS_CMD                      = 0x2D,
    MODEM_E_SET_CSMA_PARAMS_CMD                      = 0x2E,
    MODEM_E_LORAWAN_MAC_REQUEST_CMD                  = 0x2F,
    MODEM_E_GET_LORAWAN_MAC_TIME_CMD                 = 0x30,
    MODEM_E_GET_LINK_CHECK_DATA_CMD                  = 0x31,
    MODEM_E_SET_BATTERY_LEVEL_CMD                    = 0x32,
    MODEM_E_GET_CLASS_B_PING_SLOT_PERIODICITY_CMD    = 0x34,
    MODEM_E_SET_CLASS_B_PING_SLOT_PERIODICITY_CMD    = 0x35,
    MODEM_E_GET_MULTICAST_GROUP_CONFIG_CMD           = 0x36,
    MODEM_E_SET_MULTICAST_GROUP_CONFIG_CMD           = 0x37,
    MODEM_E_START_SESSION_MULTICAST_CLASS_C_CMD      = 0x38,
    MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_CMD = 0x39,
    MODEM_E_STOP_SESSION_MULTICAST_CLASS_C_CMD       = 0x3A,
    MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_C_CMD   = 0x3B,
    MODEM_E_START_SESSION_MULTICAST_CLASS_B_CMD      = 0x3C,
    MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_CMD = 0x3D,
    MODEM_E_STOP_SESSION_MULTICAST_CLASS_B_CMD       = 0x3E,
    MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_B_CMD   = 0x3F,
    MODEM_E_START_ALC_SYNC_SERVICE_CMD               = 0x42,
    MODEM_E_STOP_ALC_SYNC_SERVICE_CMD                = 0x43,
    MODEM_E_ALC_SYNC_GET_TIME_CMD                    = 0x44,
    MODEM_E_ALC_SYNC_TRIG_REQUEST_CMD                = 0X45,
    MODEM_E_FUOTA_GET_FILE_SIZE_CRC_CMD              = 0x46,
    MODEM_E_FUOTA_GET_FILE_FRAGMENT_CMD              = 0x47,
    MODEM_E_GET_USER_ADR_ACK_LIMIT_CMD               = 0x48,
    MODEM_E_SET_USER_ADR_ACK_LIMIT_CMD               = 0x49,
    MODEM_E_CONNECT_WITH_ABP_CMD                     = 0x4A,
    MODEM_E_GET_CHANNEL_MASK_CMD                     = 0x4B,
    MODEM_E_SET_CHANNEL_MASK_CMD                     = 0x4C,
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/**
 * @brief Helper function that convert an array of uint8_t into a uint32_t single value
 *
 * @warning It is up to the caller to ensure that value points to an array of at least sizeof(uint32_t) elements.
 *
 * @param [in] value Array of uint8_t to be translated into a uint32_t
 *
 * @returns 32-bit value
 */
static uint32_t modem_e_uint8_to_uint32( const uint8_t value[4] );

/**
 * @brief Compute CRC32
 *
 * @param [in,out] pcrc Pointer to the CRC. Used as initial value and as output value
 * @param buf The buffer to compute the CRC on. It is up to the caller to ensure it is at least @ref len byte long
 * @param len Length of buffer to compute the CRC on
 */
static void modem_e_fuota_crc32( uint32_t* pcrc, const uint8_t* buf, uint32_t len );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

modem_e_response_code_t modem_e_get_lorawan_version( const void* context, modem_e_lorawan_version_t* lorawan_version )
{
    uint8_t rbuffer[MODEM_E_GET_LORAWAN_VERSION_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_LORAWAN_VERSION_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LORAWAN_VERSION_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_LORAWAN_VERSION_CMD_LENGTH, rbuffer, MODEM_E_GET_LORAWAN_VERSION_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        lorawan_version->lorawan_major    = rbuffer[0];
        lorawan_version->lorawan_minor    = rbuffer[1];
        lorawan_version->lorawan_patch    = rbuffer[2];
        lorawan_version->lorawan_revision = rbuffer[3];
        lorawan_version->rp_major         = rbuffer[4];
        lorawan_version->rp_minor         = rbuffer[5];
        lorawan_version->rp_patch         = rbuffer[6];
        lorawan_version->rp_revision      = rbuffer[7];
    }

    return rc;
}

modem_e_response_code_t modem_e_get_otaa_dev_eui( const void* context, modem_e_otaa_dev_eui_t dev_eui )
{
    uint8_t rbuffer[MODEM_E_DEV_EUI_BUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_DEV_EUI_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_DEV_EUI_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_DEV_EUI_CMD_LENGTH, rbuffer, MODEM_E_DEV_EUI_BUFFER_LENGTH );

    for( uint8_t i = 0; i < MODEM_E_DEV_EUI_BUFFER_LENGTH; i++ )
    {
        dev_eui[i] = rbuffer[i];
    }

    return rc;
}

modem_e_response_code_t modem_e_set_otaa_dev_eui( const void* context, const modem_e_otaa_dev_eui_t dev_eui )
{
    const uint8_t cbuffer[MODEM_E_SET_DEV_EUI_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_DEV_EUI_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_DEV_EUI_CMD_LENGTH,
                                                                dev_eui, MODEM_E_DEV_EUI_BUFFER_LENGTH );
}

modem_e_response_code_t modem_e_get_otaa_join_eui( const void* context, modem_e_otaa_join_eui_t join_eui )
{
    uint8_t rbuffer[MODEM_E_JOIN_EUI_BUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_JOIN_EUI_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_JOIN_EUI_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_JOIN_EUI_CMD_LENGTH, rbuffer, MODEM_E_JOIN_EUI_BUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        for( uint8_t i = 0; i < MODEM_E_JOIN_EUI_BUFFER_LENGTH; i++ )
        {
            join_eui[i] = rbuffer[i];
        }
    }

    return rc;
}

modem_e_response_code_t modem_e_set_otaa_join_eui( const void* context, const modem_e_otaa_join_eui_t join_eui )
{
    const uint8_t cbuffer[MODEM_E_SET_JOIN_EUI_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_JOIN_EUI_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_JOIN_EUI_CMD_LENGTH,
                                                                join_eui, MODEM_E_JOIN_EUI_BUFFER_LENGTH );
}

modem_e_response_code_t modem_e_set_otaa_nwk_key( const void* context, const modem_e_otaa_nwk_key_t nwk_key )
{
    const uint8_t cbuffer[MODEM_E_SET_NWK_KEY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_NWK_KEY_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_NWK_KEY_CMD_LENGTH,
                                                                nwk_key, MODEM_E_OTAA_NWK_KEY_LENGTH );
}

modem_e_response_code_t modem_e_set_otaa_app_key( const void* context, const modem_e_otaa_app_key_t app_key )
{
    const uint8_t cbuffer[MODEM_E_SET_APP_KEY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_APP_KEY_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_APP_KEY_CMD_LENGTH,
                                                                app_key, MODEM_E_OTAA_APP_KEY_LENGTH );
}

modem_e_response_code_t modem_e_derive_keys( const void* context )
{
    const uint8_t cbuffer[MODEM_E_DERIVE_KEYS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_DERIVE_KEYS_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_DERIVE_KEYS_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_class( const void* context, modem_e_classes_t* modem_class )
{
    const uint8_t cbuffer[MODEM_E_GET_CLASS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CLASS_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_CLASS_CMD_LENGTH,
                                                               ( uint8_t* ) modem_class, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_class( const void* context, const modem_e_classes_t modem_class )
{
    const uint8_t cbuffer[MODEM_E_SET_CLASS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_CLASS_CMD,
        ( uint8_t ) modem_class,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_CLASS_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_region( const void* context, modem_e_regions_t* region )
{
    const uint8_t cbuffer[MODEM_E_GET_REGION_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_REGION_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_REGION_CMD_LENGTH,
                                                               ( uint8_t* ) region, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_region( const void* context, const modem_e_regions_t region )
{
    const uint8_t cbuffer[MODEM_E_SET_REGION_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_REGION_CMD,
        ( uint8_t ) region,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_REGION_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_join( const void* context )
{
    const uint8_t cbuffer[MODEM_E_JOIN_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_JOIN_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_JOIN_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_leave_network( const void* context )
{
    const uint8_t cbuffer[MODEM_E_LEAVE_NETWORK_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_LEAVE_NETWORK_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_LEAVE_NETWORK_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_next_tx_max_payload( const void* context, uint8_t* tx_max_payload )
{
    const uint8_t cbuffer[MODEM_E_GET_NEXT_TX_MAX_PAYLOAD_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_NEXT_TX_MAX_PAYLOAD_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_NEXT_TX_MAX_PAYLOAD_CMD_LENGTH, tx_max_payload, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_request_tx( const void* context, const uint8_t port,
                                            const modem_e_uplink_type_t uplink_type, const uint8_t* data,
                                            const uint8_t length )
{
    const uint8_t cbuffer[MODEM_E_REQUEST_TX_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_REQUEST_TX_CMD,
        port,
        ( uint8_t ) uplink_type,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_REQUEST_TX_CMD_LENGTH, data,
                                                                length );
}

modem_e_response_code_t modem_e_request_empty_tx( const void* context, bool is_fport_populated, const uint8_t port,
                                                  const modem_e_uplink_type_t uplink_type )
{
    const uint8_t cbuffer[MODEM_E_REQUEST_EMPTY_TX_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_REQUEST_EMPTY_TX_CMD,
        ( is_fport_populated ? 0x01 : 0x00 ),
        port,
        ( uint8_t ) uplink_type,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_REQUEST_EMPTY_TX_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_emergency_tx( const void* context, const uint8_t port,
                                              const modem_e_uplink_type_t uplink_type, const uint8_t* data,
                                              const uint8_t length )
{
    const uint8_t cbuffer[MODEM_E_EMERGENCY_TX_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_EMERGENCY_TX_CMD,
        port,
        ( uint8_t ) uplink_type,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_EMERGENCY_TX_CMD_LENGTH, data,
                                                                length );
}

modem_e_response_code_t modem_e_get_downlink_data_size( const void* context, uint8_t* downlink_data_size,
                                                        uint8_t* remaining_downlinks )
{
    const uint8_t cbuffer[MODEM_E_GET_DOWNLINK_DATA_SIZE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_DOWNLINK_DATA_SIZE_CMD,
    };

    uint8_t rbuffer[MODEM_E_GET_DOWNLINK_DATA_SIZE_RBUFFER_LENGTH] = { 0x00 };

    const modem_e_response_code_t rc =
        ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_DOWNLINK_DATA_SIZE_CMD_LENGTH,
                                                            rbuffer, MODEM_E_GET_DOWNLINK_DATA_SIZE_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *downlink_data_size  = rbuffer[0];
        *remaining_downlinks = rbuffer[1];
    }
    return rc;
}

modem_e_response_code_t modem_e_get_downlink_data( const void* context, uint8_t* buffer, uint8_t downlink_data_size )
{
    const uint8_t cbuffer[MODEM_E_GET_DOWNLINK_DATA_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_DOWNLINK_DATA_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_DOWNLINK_DATA_CMD_LENGTH,
                                                               buffer, downlink_data_size );
}

modem_e_response_code_t modem_e_get_downlink_metadata( const void* context, modem_e_downlink_metadata_t* metadata )
{
    uint8_t rbuffer[MODEM_E_DOWNLINK_METADATA_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_DOWNLINK_METADATA_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_DOWNLINK_METADATA_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_DOWNLINK_METADATA_CMD_LENGTH, rbuffer, MODEM_E_DOWNLINK_METADATA_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        metadata->stack_id     = rbuffer[0];
        metadata->rssi         = ( int16_t ) ( ( int8_t ) rbuffer[1] - 64 );
        metadata->snr_integer  = ( ( int8_t ) rbuffer[2] ) >> 2;
        metadata->snr_quarter  = rbuffer[2] & 0x03;
        metadata->window       = ( modem_e_downlink_window_t ) rbuffer[3];
        metadata->fport        = rbuffer[4];
        metadata->fpending_bit = rbuffer[5];
        metadata->frequency_hz = modem_e_uint8_to_uint32( rbuffer + 6 );
        metadata->datarate     = rbuffer[10];
    }

    return rc;
}

modem_e_response_code_t modem_e_get_lost_connection_counter( const void* context, uint16_t* lost_connection_counter,
                                                             uint32_t* lost_connection_since_sec )
{
    uint8_t rbuffer[MODEM_E_GET_LOST_CONNECTION_COUNTER_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_LOST_CONNECTION_COUNTER_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LOST_CONNECTION_COUNTER_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_LOST_CONNECTION_COUNTER_CMD_LENGTH, rbuffer,
        MODEM_E_GET_LOST_CONNECTION_COUNTER_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *lost_connection_counter   = ( uint16_t ) ( ( rbuffer[0] << 8u ) + ( rbuffer[1] ) );
        *lost_connection_since_sec = modem_e_uint8_to_uint32( rbuffer + 2 );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_network_type( const void* context, modem_e_network_type_t* network_type )
{
    const uint8_t cbuffer[MODEM_E_GET_NETWORK_TYPE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_NETWORK_TYPE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_NETWORK_TYPE_CMD_LENGTH,
                                                               ( uint8_t* ) network_type, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_network_type( const void* context, const modem_e_network_type_t network_type )
{
    const uint8_t cbuffer[MODEM_E_SET_NETWORK_TYPE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_NETWORK_TYPE_CMD,
        ( uint8_t ) network_type,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_NETWORK_TYPE_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_get_certification_mode( const void* context, modem_e_certification_mode_t* enable )
{
    const uint8_t cbuffer[MODEM_E_GET_CERTIFICATION_MODE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CERTIFICATION_MODE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CERTIFICATION_MODE_CMD_LENGTH, ( uint8_t* ) enable, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_certification_mode( const void* context, const modem_e_certification_mode_t enable )
{
    const uint8_t cbuffer[MODEM_E_SET_CERTIFICATION_MODE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_CERTIFICATION_MODE_CMD,
        ( uint8_t ) enable,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_CERTIFICATION_MODE_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_duty_cycle_status( const void* context, int32_t* duty_cycle )
{
    uint8_t rbuffer[sizeof( int32_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_DUTY_CYCLE_STATUS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_DUTY_CYCLE_STATUS_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_DUTY_CYCLE_STATUS_CMD_LENGTH, rbuffer, sizeof( int32_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *duty_cycle = ( int32_t ) modem_e_uint8_to_uint32( rbuffer );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_available_data_rate( const void* context, uint16_t* available_data_rate )
{
    uint8_t rbuffer[sizeof( uint16_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_AVAILABLE_DATA_RATE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_AVAILABLE_DATA_RATE_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_AVAILABLE_DATA_RATE_CMD_LENGTH, rbuffer, sizeof( uint16_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *available_data_rate = ( uint16_t ) ( ( rbuffer[0] << 8u ) + rbuffer[1] );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_adr_profile( const void* context, modem_e_adr_profiles_t* adr_profile )
{
    const uint8_t cbuffer[MODEM_E_GET_ADR_PROFILE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_ADR_PROFILE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_ADR_PROFILE_CMD_LENGTH,
                                                               ( uint8_t* ) adr_profile, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_adr_profile( const void* context, const modem_e_adr_profiles_t adr_profile,
                                                 const uint8_t* adr_custom_list )
{
    const uint8_t cbuffer[MODEM_E_SET_ADR_PROFILE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_ADR_PROFILE_CMD,
        ( uint8_t ) adr_profile,
    };

    const uint8_t data_length = ( adr_profile == MODEM_E_ADR_PROFILE_CUSTOM ) ? 16 : 0;

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_ADR_PROFILE_CMD_LENGTH,
                                                                adr_custom_list, data_length );
}

modem_e_response_code_t modem_e_set_join_data_rate_distribution(
    const void* context, const uint8_t distribution[MODEM_E_DATARATE_DISTRIBUTION_LENGTH] )
{
    const uint8_t cbuffer[MODEM_E_SET_JOIN_DATA_RATE_DISTRIBUTION_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_JOIN_DATA_RATE_DISTRIBUTION_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_JOIN_DATA_RATE_DISTRIBUTION_CMD_LENGTH,
                                                                distribution, MODEM_E_DATARATE_DISTRIBUTION_LENGTH );
}

modem_e_response_code_t modem_e_get_nb_trans( const void* context, uint8_t* nb_trans )
{
    const uint8_t cbuffer[MODEM_E_GET_NB_TRANS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_NB_TRANS_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_NB_TRANS_CMD_LENGTH,
                                                               ( uint8_t* ) nb_trans, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_nb_trans( const void* context, const uint8_t nb_trans )
{
    const uint8_t cbuffer[MODEM_E_SET_NB_TRANS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_NB_TRANS_CMD,
        nb_trans,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_NB_TRANS_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_adr_ack_limit_delay( const void* context, uint8_t* limit, uint8_t* delay )
{
    uint8_t rbuffer[sizeof( uint16_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_ADR_ACK_LIMIT_DELAY_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH, rbuffer, sizeof( uint16_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *limit = rbuffer[0];
        *delay = rbuffer[1];
    }

    return rc;
}

modem_e_response_code_t modem_e_set_adr_ack_limit_delay( const void* context, const uint8_t limit, const uint8_t delay )
{
    const uint8_t cbuffer[MODEM_E_SET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_ADR_ACK_LIMIT_DELAY_CMD,
        limit,
        delay,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_ADR_ACK_LIMIT_DELAY_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_lbt_state( const void* context, modem_e_lbt_mode_t* enable )
{
    const uint8_t cbuffer[MODEM_E_GET_LBT_STATE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LBT_STATE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_LBT_STATE_CMD_LENGTH,
                                                               ( uint8_t* ) enable, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_lbt_state( const void* context, const modem_e_lbt_mode_t enable )
{
    const uint8_t cbuffer[MODEM_E_SET_LBT_STATE_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_SET_LBT_STATE_CMD,
        enable,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_LBT_STATE_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_set_lbt_params( const void* context, const uint32_t duration, const int16_t threshold,
                                                const uint32_t bandwidth )
{
    const uint8_t cbuffer[MODEM_E_SET_LBT_PARAMS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_LBT_PARAMS_CMD,
        ( uint8_t ) ( duration >> 24 ),
        ( uint8_t ) ( duration >> 16 ),
        ( uint8_t ) ( duration >> 8 ),
        ( uint8_t ) ( duration ),
        ( uint8_t ) ( threshold >> 8 ),
        ( uint8_t ) ( threshold ),
        ( uint8_t ) ( bandwidth >> 24 ),
        ( uint8_t ) ( bandwidth >> 16 ),
        ( uint8_t ) ( bandwidth >> 8 ),
        ( uint8_t ) ( bandwidth ),
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_LBT_PARAMS_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_lbt_params( const void* context, uint32_t* duration, int16_t* threshold,
                                                uint32_t* bandwidth )
{
    uint8_t rbuffer[MODEM_E_LBT_PARAMS_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_LBT_PARAMS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LBT_PARAMS_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_LBT_PARAMS_CMD_LENGTH, rbuffer, MODEM_E_LBT_PARAMS_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *duration = modem_e_uint8_to_uint32( rbuffer );

        *threshold = ( int16_t ) ( ( rbuffer[4] << 8u ) + ( rbuffer[5] ) );

        *bandwidth = modem_e_uint8_to_uint32( rbuffer + 6 );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_csma_state( const void* context, modem_e_csma_mode_t* enable )
{
    const uint8_t cbuffer[MODEM_E_GET_CSMA_STATE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CSMA_STATE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_CSMA_STATE_CMD_LENGTH,
                                                               ( uint8_t* ) enable, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_csma_state( const void* context, const modem_e_csma_mode_t enable )
{
    const uint8_t cbuffer[MODEM_E_SET_CSMA_STATE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_CSMA_STATE_CMD,
        enable,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_CSMA_STATE_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_set_csma_params( const void* context, const uint8_t max_channel_change,
                                                 const modem_e_csma_backoff_mode_t backoff_enable,
                                                 const uint8_t                     nb_backoff_max )
{
    const uint8_t cbuffer[MODEM_E_SET_CSMA_PARAMS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_CSMA_PARAMS_CMD,
        max_channel_change,
        ( uint8_t ) backoff_enable,
        nb_backoff_max,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_CSMA_PARAMS_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_csma_params( const void* context, uint8_t* max_channel_change,
                                                 uint8_t* backoff_enable, uint8_t* nb_backoff_max )
{
    uint8_t rbuffer[MODEM_E_CSMA_PARAMS_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_CSMA_PARAMS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CSMA_PARAMS_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CSMA_PARAMS_CMD_LENGTH, rbuffer, MODEM_E_CSMA_PARAMS_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *max_channel_change = rbuffer[0];
        *backoff_enable     = rbuffer[1];
        *nb_backoff_max     = rbuffer[2];
    }
    return rc;
}

modem_e_response_code_t modem_e_mac_request_tx( const void* context, const modem_e_mac_request_bitmask_t mac_request )
{
    const uint8_t cbuffer[MODEM_E_LORAWAN_MAC_REQUEST_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_LORAWAN_MAC_REQUEST_CMD,
        ( uint8_t ) mac_request,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_LORAWAN_MAC_REQUEST_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_lorawan_mac_time( const void* context, uint32_t* gps_time_sec,
                                                      uint32_t* gps_fractionnal_sec )
{
    uint8_t rbuffer[MODEM_E_GET_LORAWAN_MAC_TIME_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_LORAWAN_MAC_TIME_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LORAWAN_MAC_TIME_CMD,
    };

    const modem_e_response_code_t rc =
        ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_LORAWAN_MAC_TIME_CMD_LENGTH,
                                                            rbuffer, MODEM_E_GET_LORAWAN_MAC_TIME_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *gps_time_sec        = modem_e_uint8_to_uint32( rbuffer );
        *gps_fractionnal_sec = modem_e_uint8_to_uint32( rbuffer + 4 );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_link_check_data( const void* context, uint8_t* margin, uint8_t* gateway_count )
{
    uint8_t rbuffer[sizeof( uint16_t )] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_LINK_CHECK_DATA_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_LINK_CHECK_DATA_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_LINK_CHECK_DATA_CMD_LENGTH, rbuffer, sizeof( uint16_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *margin        = rbuffer[0];
        *gateway_count = rbuffer[1];
    }

    return rc;
}

modem_e_response_code_t modem_e_set_battery_level( const void*                                context,
                                                   const modem_e_battery_level_source_value_t modem_vs_user,
                                                   uint8_t                                    value )
{
    const uint8_t cbuffer[MODEM_E_SET_BATTERY_LEVEL_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_BATTERY_LEVEL_CMD,
        ( uint8_t ) modem_vs_user,
        value,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_BATTERY_LEVEL_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_get_class_b_ping_slot_periodicity( const void*                  context,
                                                                   modem_e_class_b_ping_slot_t* ping_slot_periodicity )

{
    const uint8_t cbuffer[MODEM_E_GET_CLASS_B_PING_SLOT_PERIODICITY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CLASS_B_PING_SLOT_PERIODICITY_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_LINK_CHECK_DATA_CMD_LENGTH,
                                                               ( uint8_t* ) ping_slot_periodicity, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_class_b_ping_slot_periodicity(
    const void* context, const modem_e_class_b_ping_slot_t ping_slot_periodicity )
{
    const uint8_t cbuffer[MODEM_E_SET_CLASS_B_PING_SLOT_PERIODICITY_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_CLASS_B_PING_SLOT_PERIODICITY_CMD,
        ( uint8_t ) ping_slot_periodicity,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_SET_CLASS_B_PING_SLOT_PERIODICITY_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_multicast_group_config( const void* context, const uint8_t mc_group_id,
                                                            uint32_t* mc_group_address )
{
    uint8_t rbuffer[MODEM_E_GET_MULTICAST_GROUP_CONFIG_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_MULTICAST_GROUP_CONFIG_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_MULTICAST_GROUP_CONFIG_CMD,
        mc_group_id,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_MULTICAST_GROUP_CONFIG_CMD_LENGTH, rbuffer,
        MODEM_E_GET_MULTICAST_GROUP_CONFIG_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *mc_group_address = modem_e_uint8_to_uint32( rbuffer );
    }

    return rc;
}

modem_e_response_code_t modem_e_set_multicast_group_config( const void* context, const uint8_t mc_group_id,
                                                            const uint32_t mc_group_address, const uint8_t* mc_nwkskey,
                                                            const uint8_t* mc_appskey )
{
    uint8_t cbuffer[MODEM_E_SET_MULTICAST_GROUP_CONFIG_CMD_LENGTH];

    cbuffer[0] = ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF;
    cbuffer[1] = MODEM_E_GROUP_ID_LORAWAN & 0xFF;
    cbuffer[2] = MODEM_E_SET_MULTICAST_GROUP_CONFIG_CMD;

    cbuffer[3] = mc_group_id;

    cbuffer[4] = ( uint8_t ) ( mc_group_address >> 24 );
    cbuffer[5] = ( uint8_t ) ( mc_group_address >> 16 );
    cbuffer[6] = ( uint8_t ) ( mc_group_address >> 8 );
    cbuffer[7] = ( uint8_t ) mc_group_address;

    for( uint8_t i = 0; i < MODEM_E_NWKSKEY_LENGTH; i++ )
    {
        cbuffer[8 + i] = mc_nwkskey[i];
    }

    for( uint8_t i = 0; i < MODEM_E_APPSKEY_LENGTH; i++ )
    {
        cbuffer[24 + i] = mc_appskey[i];
    }

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_MULTICAST_GROUP_CONFIG_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_start_session_multicast_class_c( const void* context, const uint8_t mc_group_id,
                                                                 const uint32_t downlink_frequency,
                                                                 const uint8_t  downlink_data_rate )
{
    const uint8_t cbuffer[MODEM_E_START_SESSION_MULTICAST_CLASS_C_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_START_SESSION_MULTICAST_CLASS_C_CMD,
        mc_group_id,
        ( uint8_t ) ( downlink_frequency >> 24 ),
        ( uint8_t ) ( downlink_frequency >> 16 ),
        ( uint8_t ) ( downlink_frequency >> 8 ),
        ( uint8_t ) downlink_frequency,
        downlink_data_rate,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_START_SESSION_MULTICAST_CLASS_C_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_multicast_class_c_session_status(
    const void* context, const uint8_t mc_group_id,
    modem_e_multicast_class_c_status_t* modem_e_multicast_class_c_status )
{
    uint8_t rbuffer[MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_CMD,
        mc_group_id,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_CMD_LENGTH, rbuffer,
        MODEM_E_GET_MULTICAST_CLASS_C_SESSION_STATUS_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        modem_e_multicast_class_c_status->is_session_started = rbuffer[0];
        modem_e_multicast_class_c_status->downlink_frequency = modem_e_uint8_to_uint32( rbuffer + 1 );
        modem_e_multicast_class_c_status->downlink_datarate  = rbuffer[5];
    }

    return rc;
}

modem_e_response_code_t modem_e_stop_session_multicast_class_c( const void* context, const uint8_t mc_group_id )
{
    const uint8_t cbuffer[MODEM_E_STOP_SESSION_MULTICAST_CLASS_C_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_STOP_SESSION_MULTICAST_CLASS_C_CMD,
        mc_group_id,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_STOP_SESSION_MULTICAST_CLASS_C_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_stop_all_session_multicast_class_c( const void* context )
{
    const uint8_t cbuffer[MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_C_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_C_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_C_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_start_session_multicast_class_b( const void* context, const uint8_t mc_group_id,
                                                                 const uint32_t                    downlink_frequency,
                                                                 const uint8_t                     downlink_data_rate,
                                                                 const modem_e_class_b_ping_slot_t ping_slot )
{
    const uint8_t cbuffer[MODEM_E_START_SESSION_MULTICAST_CLASS_B_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_START_SESSION_MULTICAST_CLASS_B_CMD,
        mc_group_id,
        ( uint8_t ) ( downlink_frequency >> 24 ),
        ( uint8_t ) ( downlink_frequency >> 16 ),
        ( uint8_t ) ( downlink_frequency >> 8 ),
        ( uint8_t ) downlink_frequency,
        downlink_data_rate,
        ping_slot,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_START_SESSION_MULTICAST_CLASS_B_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_multicast_class_b_session_status(
    const void* context, const uint8_t mc_group_id,
    modem_e_multicast_class_b_status_t* modem_e_multicast_class_b_status )
{
    uint8_t rbuffer[MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_CMD,
        mc_group_id,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_CMD_LENGTH, rbuffer,
        MODEM_E_GET_MULTICAST_CLASS_B_SESSION_STATUS_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        modem_e_multicast_class_b_status->is_session_started            = rbuffer[0];
        modem_e_multicast_class_b_status->downlink_frequency            = modem_e_uint8_to_uint32( rbuffer + 1 );
        modem_e_multicast_class_b_status->downlink_datarate             = rbuffer[5];
        modem_e_multicast_class_b_status->is_session_waiting_for_beacon = rbuffer[6];
        modem_e_multicast_class_b_status->ping_slot_periodicity         = rbuffer[7];
    }

    return rc;
}

modem_e_response_code_t modem_e_stop_session_multicast_class_b( const void* context, const uint8_t mc_group_id )
{
    const uint8_t cbuffer[MODEM_E_STOP_SESSION_MULTICAST_CLASS_B_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_STOP_SESSION_MULTICAST_CLASS_B_CMD,
        mc_group_id,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_STOP_SESSION_MULTICAST_CLASS_B_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_stop_all_session_multicast_class_b( const void* context )
{
    const uint8_t cbuffer[MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_B_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_B_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_STOP_ALL_SESSION_MULTICAST_CLASS_B_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_alc_sync_start_service( const void* context )
{
    const uint8_t cbuffer[MODEM_E_START_ALC_SYNC_SERVICE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_START_ALC_SYNC_SERVICE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_START_ALC_SYNC_SERVICE_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_alc_sync_stop_service( const void* context )
{
    const uint8_t cbuffer[MODEM_E_STOP_ALC_SYNC_SERVICE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_STOP_ALC_SYNC_SERVICE_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_STOP_ALC_SYNC_SERVICE_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_alc_sync_get_time( const void* context, uint32_t* alc_sync_epoch_time )
{
    uint8_t rbuffer[MODEM_E_ALC_SYNC_GET_TIME_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_ALC_SYNC_GET_TIME_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_ALC_SYNC_GET_TIME_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_ALC_SYNC_GET_TIME_CMD_LENGTH, rbuffer, MODEM_E_ALC_SYNC_GET_TIME_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *alc_sync_epoch_time = modem_e_uint8_to_uint32( rbuffer );
    }

    return rc;
}

modem_e_response_code_t modem_e_alc_sync_trig_request( const void* context )
{
    const uint8_t cbuffer[MODEM_E_ALC_SYNC_TRIG_REQUEST_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_ALC_SYNC_TRIG_REQUEST_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_ALC_SYNC_TRIG_REQUEST_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_fuota_get_file_size_crc( const void* context, uint32_t* file_size, uint32_t* file_crc )
{
    uint8_t rbuffer[MODEM_E_FUOTA_GET_FILE_SIZE_CRC_RBUFFER_LENGTH] = { 0 };

    const uint8_t cbuffer[MODEM_E_FUOTA_GET_FILE_SIZE_CRC_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_FUOTA_GET_FILE_SIZE_CRC_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_FUOTA_GET_FILE_SIZE_CRC_CMD_LENGTH, rbuffer,
        MODEM_E_FUOTA_GET_FILE_SIZE_CRC_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *file_size = modem_e_uint8_to_uint32( rbuffer );
        *file_crc  = modem_e_uint8_to_uint32( rbuffer + 4 );
    }

    return rc;
}

modem_e_response_code_t modem_e_fuota_read_file_fragment( const void* context, uint32_t base_address,
                                                          uint16_t fragment_size, uint8_t* fragment )
{
    const uint8_t cbuffer[MODEM_E_FUOTA_GET_FILE_FRAGMENT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_FUOTA_GET_FILE_FRAGMENT_CMD,
        ( uint8_t ) ( base_address >> 24 ),
        ( uint8_t ) ( base_address >> 16 ),
        ( uint8_t ) ( base_address >> 8 ),
        ( uint8_t ) base_address,
        0x00,
        0x00,
        ( uint8_t ) ( fragment_size >> 8 ),
        ( uint8_t ) fragment_size,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_FUOTA_GET_FILE_FRAGMENT_CMD_LENGTH, fragment, fragment_size );
}

bool modem_e_fuota_check_crc( const uint8_t* file, uint32_t file_size, uint32_t expected_crc )
{
    uint32_t crc = 0;  // This is used as both initial CRC value, and computed CRC value. It is important here to be
                       // init'd to 0 for the CRC computation
    modem_e_fuota_crc32( &crc, file, file_size );
    return ( crc == expected_crc );
}

modem_e_response_code_t modem_e_set_user_dr_backoff_sequence_threshold( const void* context,
                                                                        uint16_t    user_adr_ack_limit )
{
    const uint8_t cbuffer[MODEM_E_SET_USER_ADR_ACK_LIMIT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_SET_USER_ADR_ACK_LIMIT_CMD,
        ( uint8_t ) ( user_adr_ack_limit >> 8 ),
        ( uint8_t ) user_adr_ack_limit,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_USER_ADR_ACK_LIMIT_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_user_dr_backoff_sequence_threshold( const void* context,
                                                                        uint16_t*   user_adr_ack_limit )
{
    const uint8_t cbuffer[MODEM_E_GET_USER_ADR_ACK_LIMIT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_USER_ADR_ACK_LIMIT_CMD,
    };

    uint8_t                       rbuffer[sizeof( uint16_t )] = { 0x00 };
    const modem_e_response_code_t rc                          = ( modem_e_response_code_t ) modem_e_modem_hal_read(
                                 context, cbuffer, MODEM_E_GET_USER_ADR_ACK_LIMIT_CMD_LENGTH, rbuffer, sizeof( uint16_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *user_adr_ack_limit = ( ( rbuffer[0] << 8 ) | rbuffer[1] );
    }
    return rc;
}

modem_e_response_code_t modem_e_connect_with_abp( const void* context, const modem_e_abp_dev_addr_t dev_addr,
                                                  const modem_e_abp_nkw_skey_t nwk_skey,
                                                  const modem_e_abp_app_skey_t app_skey )
{
    const uint8_t cbuffer[MODEM_E_CONNECT_WITH_ABP_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_CONNECT_WITH_ABP_CMD,
        dev_addr[0],
        dev_addr[1],
        dev_addr[2],
        dev_addr[3],
        nwk_skey[0],
        nwk_skey[1],
        nwk_skey[2],
        nwk_skey[3],
        nwk_skey[4],
        nwk_skey[5],
        nwk_skey[6],
        nwk_skey[7],
        nwk_skey[8],
        nwk_skey[9],
        nwk_skey[10],
        nwk_skey[11],
        nwk_skey[12],
        nwk_skey[13],
        nwk_skey[14],
        nwk_skey[15],
        app_skey[0],
        app_skey[1],
        app_skey[2],
        app_skey[3],
        app_skey[4],
        app_skey[5],
        app_skey[6],
        app_skey[7],
        app_skey[8],
        app_skey[9],
        app_skey[10],
        app_skey[11],
        app_skey[12],
        app_skey[13],
        app_skey[14],
        app_skey[15],
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_CONNECT_WITH_ABP_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_connect_get_channel_mask( const void* context, modem_e_channel_mask_t channel_mask )
{
    const uint8_t cbuffer[MODEM_E_GET_CHANNEL_MASK_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_LORAWAN >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_LORAWAN,
        MODEM_E_GET_CHANNEL_MASK_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CHANNEL_MASK_CMD_LENGTH, channel_mask, MODEM_CHANNEL_MASK_LENGTH );

    return rc;
}

modem_e_response_code_t modem_e_connect_set_channel_mask(
    const void* context, const modem_e_channel_mask_configuration_t* channel_configuration,
    uint8_t n_channel_configurations )
{
    const uint8_t cbuffer[MODEM_E_SET_CHANNEL_MASK_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_LORAWAN >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_LORAWAN & 0xFF,
        MODEM_E_SET_CHANNEL_MASK_CMD,
    };

    uint8_t dbuffer[MODEM_E_SET_CHANNEL_MASK_DBUFFER_MAX_LENGTH] = { };
    for( uint8_t channel_configuration_index = 0; channel_configuration_index < n_channel_configurations;
         channel_configuration_index++ )
    {
        const modem_e_channel_mask_configuration_t* local_configuration =
            &channel_configuration[channel_configuration_index];
        const uint8_t local_dbuffer_index =
            channel_configuration_index * MODEM_E_SET_CHANNEL_MASK_SINGLE_CONFIGURATION_LENGTH;
        dbuffer[local_dbuffer_index + 0] = local_configuration->channel_mask_control;
        dbuffer[local_dbuffer_index + 1] = local_configuration->channel_mask[0];
        dbuffer[local_dbuffer_index + 2] = local_configuration->channel_mask[1];
    }

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_SET_CHANNEL_MASK_CMD_LENGTH, dbuffer,
        n_channel_configurations * MODEM_E_SET_CHANNEL_MASK_SINGLE_CONFIGURATION_LENGTH );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

uint32_t modem_e_uint8_to_uint32( const uint8_t value[4] )
{
    return ( ( ( uint32_t ) value[0] ) << 24 ) + ( ( ( uint32_t ) value[1] ) << 16 ) +
           ( ( ( uint32_t ) value[2] ) << 8 ) + ( ( ( uint32_t ) value[3] ) );
}

void modem_e_fuota_crc32( uint32_t* pcrc, const uint8_t* buf, uint32_t len )
{
    uint32_t crc = ~( *pcrc );
    while( len-- != 0 )
    {
        const uint32_t byte = *buf++;
        crc                 = crc ^ byte;
        for( uint8_t i = 0; i < 8; i++ )
        {
            const uint32_t mask = -( crc & 1 );
            crc                 = ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
        }
    }
    *pcrc = ~crc;
}

/* --- EOF ------------------------------------------------------------------ */
