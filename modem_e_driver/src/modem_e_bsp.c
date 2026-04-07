/*!
 * @file      modem_e_bsp.c
 *
 * @brief     BSP driver implementation for Modem-E
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
#include "modem_e_bsp.h"
#include "modem_e_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*!
 * @brief Power config block length
 */
#define MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH ( 5u )

#define MODEM_E_TX_POWER_CONSUMPTION_SIZE_BYTE ( 5u )

#define MODEM_E_GET_TX_POWER_OFFSET_CMD_LENGTH ( 3 )
#define MODEM_E_SET_TX_POWER_OFFSET_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_OUTPUT_POWER_CONFIG_CMD_LENGTH ( 3 )
#define MODEM_E_SET_OUTPUT_POWER_CONFIG_CMD_LENGTH ( 3 )
#define MODEM_E_GET_RF_OUTPUT_CMD_LENGTH ( 3 )
#define MODEM_E_SET_RF_OUTPUT_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_CRYSTAL_ERROR_CMD_LENGTH ( 3 )
#define MODEM_E_SET_CRYSTAL_ERROR_CMD_LENGTH ( 3 + 4 )
#define MODEM_E_GET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH ( 3 )
#define MODEM_E_SET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH ( 3 + 2 )
#define MODEM_E_GET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH ( 3 )
#define MODEM_E_GET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH ( 3 )
#define MODEM_E_SET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH ( 3 + 8 )
#define MODEM_E_SET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH ( 3 )

#define MODEM_E_MODEM_GET_OUTPUT_POWER_CONFIG_RBUFFER_LENGTH \
    ( MODEM_E_NB_OUTPUT_POWER_CONFIG_BLOCKS * MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH )
#define MODEM_E_GET_TX_POWER_CONSUMPTION_RBUFFER_LENGTH \
    ( MODEM_E_NB_OUTPUT_POWER_CONFIG_BLOCKS * MODEM_E_TX_POWER_CONSUMPTION_SIZE_BYTE )
#define MODEM_E_GET_RX_POWER_CONSUMPTION_RBUFFER_LENGTH ( 8 )

#define MODEM_E_SET_TX_POWER_CONSUMPTION_UA_DBUFFER_MAX_LENGTH \
    ( MODEM_E_NB_OUTPUT_POWER_CONFIG_BLOCKS * MODEM_E_TX_POWER_CONSUMPTION_SIZE_BYTE )
#define MODEM_E_SET_TX_OUTPUT_POWER_CONFIGURATION_DBUFFER_MAX_LENGTH \
    ( MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH * MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operation code command
 */
enum
{
    MODEM_E_GET_TX_POWER_OFFSET_CMD          = 0x00,
    MODEM_E_SET_TX_POWER_OFFSET_CMD          = 0x01,
    MODEM_E_GET_OUTPUT_POWER_CONFIG_CMD      = 0x02,
    MODEM_E_SET_OUTPUT_POWER_CONFIG_CMD      = 0x03,
    MODEM_E_GET_RF_OUTPUT_CMD                = 0x04,
    MODEM_E_SET_RF_OUTPUT_CMD                = 0x05,
    MODEM_E_GET_CRYSTAL_ERROR_CMD            = 0x06,
    MODEM_E_SET_CRYSTAL_ERROR_CMD            = 0x07,
    MODEM_E_GET_XOSC_CAPA_TRIM_A_B           = 0x08,
    MODEM_E_SET_XOSC_CAPA_TRIM_A_B           = 0x09,
    MODEM_E_GET_TX_POWER_CONSUMPTION_UA      = 0x0A,
    MODEM_E_SET_TX_POWER_CONSUMPTION_UA      = 0x0B,
    MODEM_E_GET_LORA_RX_POWER_CONSUMPTION_UA = 0x0C,
    MODEM_E_SET_LORA_RX_POWER_CONSUMPTION_UA = 0x0D,
    MODEM_E_GET_GFSK_RX_POWER_CONSUMPTION_UA = 0x0E,
    MODEM_E_SET_GFSK_RX_POWER_CONSUMPTION_UA = 0x0F,
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static uint32_t modem_e_uint8_to_uint32( const uint8_t value[4] );

static modem_e_response_code_t modem_e_get_rx_power_consumption_ua( const void*                     context,
                                                                    modem_e_rx_power_consumption_t* rx_consumption,
                                                                    uint8_t                         opcode );

static modem_e_response_code_t modem_e_set_rx_power_consumption_ua(
    const void* context, const modem_e_rx_power_consumption_t* rx_consumption, uint8_t opcode );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

modem_e_response_code_t modem_e_get_tx_power_offset( const void* context, int8_t* tx_power_offset )
{
    const uint8_t cbuffer[MODEM_E_GET_TX_POWER_OFFSET_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_GET_TX_POWER_OFFSET_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_TX_POWER_OFFSET_CMD_LENGTH,
                                                               ( uint8_t* ) tx_power_offset, sizeof( int8_t ) );
}

modem_e_response_code_t modem_e_set_tx_power_offset( const void* context, const int8_t tx_power_offset )
{
    const uint8_t cbuffer[MODEM_E_SET_TX_POWER_OFFSET_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_SET_TX_POWER_OFFSET_CMD,
        ( uint8_t ) tx_power_offset,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_TX_POWER_OFFSET_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_set_output_power_config( const void*                          context,
                                                         const modem_e_output_power_config_t* output_power_configs,
                                                         uint8_t                              n_output_power_configs )
{
    const uint8_t cbuffer[MODEM_E_SET_OUTPUT_POWER_CONFIG_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_SET_OUTPUT_POWER_CONFIG_CMD,
    };

    // Array of maximal size is allocated
    uint8_t dbuffer[MODEM_E_SET_TX_OUTPUT_POWER_CONFIGURATION_DBUFFER_MAX_LENGTH] = { 0 };

    for( uint8_t index_power_config = 0; index_power_config < n_output_power_configs; index_power_config++ )
    {
        const modem_e_output_power_config_t local_power_config = output_power_configs[index_power_config];
        const unsigned int local_dbuffer_index = index_power_config * MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH;

        dbuffer[local_dbuffer_index]     = local_power_config.expected_power;
        dbuffer[local_dbuffer_index + 1] = local_power_config.configured_power;
        dbuffer[local_dbuffer_index + 2] =
            ( uint8_t ) ( ( local_power_config.pa_supply & 0x0F ) | ( local_power_config.pa_sel << 4 ) );
        dbuffer[local_dbuffer_index + 3] =
            ( uint8_t ) ( ( uint8_t ) ( ( local_power_config.pa_duty_cycle & 0x0F ) << 4 ) |
                          ( uint8_t ) ( local_power_config.pa_hp_sel & 0x0F ) );
        dbuffer[local_dbuffer_index + 4] = ( uint8_t ) local_power_config.pa_ramp_time;
    }

    const uint16_t dbuffer_size = ( uint16_t ) ( n_output_power_configs * MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH );
    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_SET_OUTPUT_POWER_CONFIG_CMD_LENGTH, dbuffer, dbuffer_size );
}

modem_e_response_code_t modem_e_get_output_power_config( const void*                        context,
                                                         modem_e_output_power_config_list_t output_power_config )
{
    uint8_t rbuffer[MODEM_E_MODEM_GET_OUTPUT_POWER_CONFIG_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_OUTPUT_POWER_CONFIG_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_GET_OUTPUT_POWER_CONFIG_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_OUTPUT_POWER_CONFIG_CMD_LENGTH, rbuffer,
        MODEM_E_MODEM_GET_OUTPUT_POWER_CONFIG_RBUFFER_LENGTH );

    for( uint8_t i = 0; i < MODEM_E_NB_OUTPUT_POWER_CONFIG_BLOCKS; i++ )
    {
        const unsigned int local_rbuffer_index = i * MODEM_E_OUTPUT_POWER_CONFIG_BLOCK_LENGTH;

        output_power_config[i].expected_power   = rbuffer[local_rbuffer_index];
        output_power_config[i].configured_power = rbuffer[local_rbuffer_index + 1];
        output_power_config[i].pa_supply = ( modem_e_pa_reg_supply_t ) ( rbuffer[local_rbuffer_index + 2] & 0x0F );
        output_power_config[i].pa_sel =
            ( modem_e_output_power_configuration_pa_sel_t ) ( ( rbuffer[local_rbuffer_index + 2] >> 4 ) & 0x0F );
        output_power_config[i].pa_duty_cycle = ( rbuffer[local_rbuffer_index + 3] & 0xF0 ) >> 4;
        output_power_config[i].pa_hp_sel     = rbuffer[local_rbuffer_index + 3] & 0x0F;
        output_power_config[i].pa_ramp_time  = ( modem_e_ramp_time_t ) ( rbuffer[local_rbuffer_index + 4] );
    }

    return rc;
}

modem_e_response_code_t modem_e_set_rf_output( const void* context, const modem_e_bsp_radio_pa_selection_t output )
{
    const uint8_t cbuffer[MODEM_E_SET_RF_OUTPUT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_SET_RF_OUTPUT_CMD,
        ( uint8_t ) output,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_RF_OUTPUT_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_rf_output( const void* context, const modem_e_bsp_radio_pa_selection_t* output )
{
    const uint8_t cbuffer[MODEM_E_GET_RF_OUTPUT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_GET_RF_OUTPUT_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_RF_OUTPUT_CMD_LENGTH,
                                                               ( uint8_t* ) output, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_get_crystal_error( const void* context, uint32_t* crystal_error_ppm )
{
    uint8_t rbuffer[sizeof( uint32_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_CRYSTAL_ERROR_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_GET_CRYSTAL_ERROR_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CRYSTAL_ERROR_CMD_LENGTH, rbuffer, sizeof( uint32_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *crystal_error_ppm = ( ( uint32_t ) rbuffer[0] << 24 ) + ( ( uint32_t ) rbuffer[1] << 16 ) +
                             ( ( uint32_t ) rbuffer[2] << 8 ) + ( ( uint32_t ) rbuffer[3] );
    }

    return rc;
}

modem_e_response_code_t modem_e_set_crystal_error( const void* context, const uint32_t crystal_error_ppm )
{
    const uint8_t cbuffer[MODEM_E_SET_CRYSTAL_ERROR_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_SET_CRYSTAL_ERROR_CMD,
        ( uint8_t ) ( crystal_error_ppm >> 24 ),
        ( uint8_t ) ( crystal_error_ppm >> 16 ),
        ( uint8_t ) ( crystal_error_ppm >> 8 ),
        ( uint8_t ) crystal_error_ppm,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_CRYSTAL_ERROR_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_get_xosc_capa_trim_a_b( const void* context, uint8_t* capa_trim_a,
                                                        uint8_t* capa_trim_b )
{
    uint8_t rbuffer[sizeof( uint16_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_GET_XOSC_CAPA_TRIM_A_B,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH, rbuffer, sizeof( uint16_t ) );
    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *capa_trim_a = rbuffer[0];
        *capa_trim_b = rbuffer[1];
    }

    return rc;
}

modem_e_response_code_t modem_e_set_xosc_capa_trim_a_b( const void* context, const uint8_t capa_trim_a,
                                                        const uint8_t capa_trim_b )
{
    const uint8_t cbuffer[MODEM_E_SET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_BSP >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_BSP,
        MODEM_E_SET_XOSC_CAPA_TRIM_A_B,
        capa_trim_a,
        capa_trim_b,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_XOSC_CAPA_TRIM_A_B_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_tx_power_consumption_ua( const void*                         context,
                                                             modem_e_tx_power_consumption_list_t consumption_per_power )
{
    const uint8_t cbuffer[MODEM_E_GET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_BSP >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_BSP & 0xFF,
        MODEM_E_GET_TX_POWER_CONSUMPTION_UA,
    };

    uint8_t rbuffer[MODEM_E_GET_TX_POWER_CONSUMPTION_RBUFFER_LENGTH] = { 0 };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH, rbuffer,
        MODEM_E_GET_TX_POWER_CONSUMPTION_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        for( uint8_t index_power = 0; index_power < MODEM_E_NB_OUTPUT_POWER_CONFIG_BLOCKS; index_power++ )
        {
            modem_e_tx_power_consumption_value_t* local_value = &consumption_per_power[index_power];
            local_value->tx_power_dbm                         = ( int8_t ) rbuffer[index_power * 5];
            local_value->consumed_power_ua                    = ( uint32_t ) ( rbuffer[index_power * 5 + 1] << 24u ) +
                                             ( uint32_t ) ( rbuffer[index_power * 5 + 2] << 16u ) +
                                             ( uint32_t ) ( rbuffer[index_power * 5 + 3] << 8u ) +
                                             ( uint32_t ) ( rbuffer[index_power * 5 + 4] << 0u );
        }
    }
    return rc;
}

modem_e_response_code_t modem_e_set_tx_power_consumption_ua(
    const void* context, const modem_e_tx_power_consumption_value_t* consumption_per_power,
    uint8_t n_consumption_per_power )
{
    const uint8_t cbuffer[MODEM_E_SET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_BSP >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_BSP & 0xFF,
        MODEM_E_SET_TX_POWER_CONSUMPTION_UA,
    };

    // Array of maximal size is allocated
    uint8_t dbuffer[MODEM_E_SET_TX_POWER_CONSUMPTION_UA_DBUFFER_MAX_LENGTH] = { 0 };

    for( uint8_t power_table_index = 0; power_table_index < n_consumption_per_power; power_table_index++ )
    {
        const modem_e_tx_power_consumption_value_t local_consumption_value = consumption_per_power[power_table_index];
        const unsigned int local_dbuffer_index = power_table_index * MODEM_E_TX_POWER_CONSUMPTION_SIZE_BYTE;

        dbuffer[local_dbuffer_index]     = ( uint8_t ) local_consumption_value.tx_power_dbm;
        dbuffer[local_dbuffer_index + 1] = ( uint8_t ) ( local_consumption_value.consumed_power_ua >> 24 );
        dbuffer[local_dbuffer_index + 2] = ( uint8_t ) ( local_consumption_value.consumed_power_ua >> 16 );
        dbuffer[local_dbuffer_index + 3] = ( uint8_t ) ( local_consumption_value.consumed_power_ua >> 8 );
        dbuffer[local_dbuffer_index + 4] = ( uint8_t ) local_consumption_value.consumed_power_ua;
    }

    // Number of meaningful bytes from dbuffer to transmit to the chip
    const uint16_t dbuffer_size = ( uint16_t ) ( n_consumption_per_power * MODEM_E_TX_POWER_CONSUMPTION_SIZE_BYTE );

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_SET_TX_POWER_CONSUMPTION_UA_CMD_LENGTH, dbuffer, dbuffer_size );
}

modem_e_response_code_t modem_e_get_lora_rx_power_consumption_ua( const void*                     context,
                                                                  modem_e_rx_power_consumption_t* rx_consumption )
{
    return modem_e_get_rx_power_consumption_ua( context, rx_consumption, MODEM_E_GET_LORA_RX_POWER_CONSUMPTION_UA );
}

modem_e_response_code_t modem_e_set_lora_rx_power_consumption_ua( const void*                           context,
                                                                  const modem_e_rx_power_consumption_t* rx_consumption )
{
    return modem_e_set_rx_power_consumption_ua( context, rx_consumption, MODEM_E_SET_LORA_RX_POWER_CONSUMPTION_UA );
}

modem_e_response_code_t modem_e_get_gfsk_rx_power_consumption_ua( const void*                     context,
                                                                  modem_e_rx_power_consumption_t* rx_consumption )
{
    return modem_e_get_rx_power_consumption_ua( context, rx_consumption, MODEM_E_GET_GFSK_RX_POWER_CONSUMPTION_UA );
}

modem_e_response_code_t modem_e_set_gfsk_rx_power_consumption_ua( const void*                           context,
                                                                  const modem_e_rx_power_consumption_t* rx_consumption )
{
    return modem_e_set_rx_power_consumption_ua( context, rx_consumption, MODEM_E_SET_GFSK_RX_POWER_CONSUMPTION_UA );
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

modem_e_response_code_t modem_e_get_rx_power_consumption_ua( const void*                     context,
                                                             modem_e_rx_power_consumption_t* rx_consumption,
                                                             uint8_t                         opcode )
{
    const uint8_t cbuffer[MODEM_E_GET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_BSP >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_BSP & 0xFF,
        opcode,
    };

    uint8_t rbuffer[MODEM_E_GET_RX_POWER_CONSUMPTION_RBUFFER_LENGTH] = { 0 };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH, rbuffer,
        MODEM_E_GET_RX_POWER_CONSUMPTION_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        rx_consumption->consumption_rx_boosted_off_ua = modem_e_uint8_to_uint32( rbuffer );
        rx_consumption->consumption_rx_boosted_on_ua  = modem_e_uint8_to_uint32( rbuffer + 4 );
    }
    return rc;
}

modem_e_response_code_t modem_e_set_rx_power_consumption_ua( const void*                           context,
                                                             const modem_e_rx_power_consumption_t* rx_consumption,
                                                             uint8_t                               opcode )
{
    const uint8_t cbuffer[MODEM_E_SET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH] = {
        ( MODEM_E_GROUP_ID_BSP >> 8 ) & 0xFF,
        MODEM_E_GROUP_ID_BSP & 0xFF,
        opcode,
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_off_ua >> 24 ),
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_off_ua >> 16 ),
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_off_ua >> 8 ),
        ( uint8_t ) rx_consumption->consumption_rx_boosted_off_ua,
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_on_ua >> 24 ),
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_on_ua >> 16 ),
        ( uint8_t ) ( rx_consumption->consumption_rx_boosted_on_ua >> 8 ),
        ( uint8_t ) rx_consumption->consumption_rx_boosted_on_ua,
    };
    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_SET_RX_POWER_CONSUMPTION_UA_CMD_LENGTH, 0, 0 );
}

/* --- EOF ------------------------------------------------------------------ */
