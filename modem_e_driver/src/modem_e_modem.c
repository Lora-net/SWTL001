/*!
 * @file      modem_e_modem.c
 *
 * @brief     modem driver implementation for Modem-E
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
#include "modem_e_modem.h"
#include "modem_e_common.h"
#include "modem_e_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define MODEM_E_FACTORY_RESET_CMD_LENGTH ( 3 )
#define MODEM_E_GET_VERSION_CMD_LENGTH ( 3 )
#define MODEM_E_GET_STATUS_CMD_LENGTH ( 3 )
#define MODEM_E_GET_CHARGE_CMD_LENGTH ( 3 )
#define MODEM_E_GET_EVENT_CMD_LENGTH ( 3 )
#define MODEM_E_SET_SUSPEND_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_GET_SUSPEND_CMD_LENGTH ( 3 )
#define MODEM_E_SET_ALARM_TIMER_CMD_LENGTH ( 3 + 4 )
#define MODEM_E_CLEAR_ALARM_TIMER_CMD_LENGTH ( 3 )
#define MODEM_E_GET_ALARM_REMAINING_TIME_CMD_LENGTH ( 3 )
#define MODEM_E_GET_CRASHLOG_CMD_LENGTH ( 3 )
#define MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_CMD_LENGTH ( 3 )
#define MODEM_E_RESTORE_STATE_SNAPSHOT_FROM_NVM_CMD_LENGTH ( 3 + 8 )

#define MODEM_E_TEST_MODE_TST_START_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_TEST_MODE_TST_NOP_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_TEST_MODE_TST_TX_LORA_CMD_LENGTH ( 3 + 26 )
#define MODEM_E_TEST_MODE_TST_TX_FSK_CMD_LENGTH ( 3 + 15 )
#define MODEM_E_TEST_MODE_TST_TX_LR_FHSS_CMD_LENGTH ( 3 + 19 )
#define MODEM_E_TEST_MODE_TST_CW_CMD_LENGTH ( 3 + 6 )
#define MODEM_E_TEST_MODE_TST_RX_LORA_CONT_CMD_LENGTH ( 3 + 8 )
#define MODEM_E_TEST_MODE_TST_RX_FSK_CONT_CMD_LENGTH ( 3 + 5 )
#define MODEM_E_TEST_MODE_TST_READ_PKT_COUNTER_RX_CONT_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_TEST_MODE_TST_RSSI_SUBGHZ_CMD_LENGTH ( 3 + 11 )
#define MODEM_E_TEST_MODE_TST_RADIO_RST_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_TEST_MODE_TST_EXIT_CMD_LENGTH ( 3 + 1 )
#define MODEM_E_TEST_MODE_READ_RSSI_CMD_LENGTH ( 3 + 1 )

#define MODEM_E_GET_CHARGE_RBUFFER_LENGTH ( 320 )
#define MODEM_E_GET_CRASHLOG_RBUFFER_LENGTH ( 243 )
#define MODEM_E_GET_VERSION_RBUFFER_LENGTH ( 9 )
#define MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_RBUFFER_LENGTH ( 4 )
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operation code command
 */
enum
{
    MODEM_E_FACTORY_RESET_CMD                   = 0x00,
    MODEM_E_GET_VERSION_CMD                     = 0x01,
    MODEM_E_GET_STATUS_CMD                      = 0x02,
    MODEM_E_GET_CHARGE_CMD                      = 0x03,
    MODEM_E_GET_EVENT_CMD                       = 0x04,
    MODEM_E_TEST_CMD                            = 0x05,
    MODEM_E_GET_SUSPEND_MODEM_COM_CMD           = 0x06,
    MODEM_E_SET_SUSPEND_MODEM_COM_CMD           = 0x07,
    MODEM_E_SET_ALARM_TIMER_CMD                 = 0x08,
    MODEM_E_CLEAR_ALARM_TIMER_CMD               = 0x09,
    MODEM_E_GET_ALARM_REMAINING_TIME_CMD        = 0x0A,
    MODEM_E_GET_CRASHLOG_CMD                    = 0x0B,
    MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_CMD     = 0x0C,
    MODEM_E_RESTORE_STATE_SNAPSHOT_FROM_NVM_CMD = 0x0D,
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Radio test mode type
 */
typedef enum
{
    MODEM_E_TEST_MODE_TST_START                       = 0x00,
    MODEM_E_TEST_MODE_TST_EXIT                        = 0x01,
    MODEM_E_TEST_MODE_TST_NOP                         = 0x02,
    MODEM_E_TEST_MODE_TST_TX_LORA                     = 0x03,
    MODEM_E_TEST_MODE_TST_TX_FSK                      = 0x04,
    MODEM_E_TEST_MODE_TST_TX_LR_FHSS                  = 0x05,
    MODEM_E_TEST_MODE_TST_TX_CW                       = 0x06,
    MODEM_E_TEST_MODE_TST_RX_LORA_CONT                = 0x07,
    MODEM_E_TEST_MODE_TST_RX_FSK_CONT                 = 0x08,
    MODEM_E_TEST_MODE_TST_READ_RX_PKT_COUNTER_RX_CONT = 0x09,
    MODEM_E_TEST_MODE_TST_RSSI_SUBGHZ                 = 0x0A,
    MODEM_E_TEST_MODE_READ_RSSI                       = 0x0B,
    MODEM_E_TEST_MODE_TST_RADIO_RST                   = 0x0E,
    MODEM_E_TEST_MODE_TST_READ_REGISTER               = 0x0F,
    MODEM_E_TEST_MODE_TST_WRITE_REGISTER              = 0x10,
} modem_e_test_mode_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Helper function that convert an array of uint8_t into a uint32_t single value
 *
 * @warning It is up to the caller to ensure that value points to an array of at least sizeof(uint32_t) elements.
 *
 * @param [in] value Array of uint8_t to be translated into a uint32_t
 *
 * @returns 32-bit value
 */
static uint32_t modem_e_uint8_to_uint32( const uint8_t value[4] );

static void modem_e_parse_charge_hook_id( const uint8_t* buffer, uint8_t hook_id, modem_e_consumption_details_t* hook );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

modem_e_response_code_t modem_e_factory_reset( const void* context )
{
    const uint8_t cbuffer[MODEM_E_FACTORY_RESET_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_FACTORY_RESET_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write_without_rc( context, cbuffer,
                                                                           MODEM_E_FACTORY_RESET_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_get_modem_version( const void* context, modem_e_version_t* modem_version )
{
    uint8_t rbuffer[MODEM_E_GET_VERSION_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_VERSION_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_VERSION_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_VERSION_CMD_LENGTH, rbuffer, MODEM_E_GET_VERSION_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        modem_version->use_case    = rbuffer[0];
        modem_version->modem_major = rbuffer[1];
        modem_version->modem_minor = rbuffer[2];
        modem_version->modem_patch = rbuffer[3];
        modem_version->lbm_major   = rbuffer[5];
        modem_version->lbm_minor   = rbuffer[6];
        modem_version->lbm_patch   = rbuffer[7];
    }
    return rc;
}

modem_e_response_code_t modem_e_get_status( const void* context, modem_e_lorawan_status_bitmask_t* status )
{
    const uint8_t cbuffer[MODEM_E_GET_STATUS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_STATUS_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_STATUS_CMD_LENGTH,
                                                               ( uint8_t* ) status, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_get_charge( const void* context, modem_e_charge_t* charge )
{
    uint8_t rbuffer[MODEM_E_GET_CHARGE_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_CHARGE_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_CHARGE_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CHARGE_CMD_LENGTH, rbuffer, MODEM_E_GET_CHARGE_RBUFFER_LENGTH );

    modem_e_parse_charge_hook_id( rbuffer, 0, &charge->suspend );
    modem_e_parse_charge_hook_id( rbuffer, 1, &charge->class_b_beacon );
    modem_e_parse_charge_hook_id( rbuffer, 2, &charge->lr1mac_stack );
    modem_e_parse_charge_hook_id( rbuffer, 3, &charge->lbt );
    modem_e_parse_charge_hook_id( rbuffer, 4, &charge->cad );
    modem_e_parse_charge_hook_id( rbuffer, 5, &charge->class_b_ping_slot );
    modem_e_parse_charge_hook_id( rbuffer, 6, &charge->test_mode );
    modem_e_parse_charge_hook_id( rbuffer, 7, &charge->direct_rp_access );
    modem_e_parse_charge_hook_id( rbuffer, 8, &charge->relay_tx );
    modem_e_parse_charge_hook_id( rbuffer, 9, &charge->class_c );

    return rc;
}

modem_e_response_code_t modem_e_get_event( const void* context, modem_e_event_fields_t* event_fields )
{
    uint8_t rbuffer[sizeof( uint32_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_EVENT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_EVENT_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_EVENT_CMD_LENGTH, rbuffer, sizeof( uint32_t ) );

    event_fields->event_type          = ( modem_e_lorawan_event_type_t ) rbuffer[0];
    event_fields->missed_events_count = rbuffer[1];
    event_fields->data                = ( uint16_t ) ( ( rbuffer[2] << 8u ) + rbuffer[3] );

    return rc;
}

modem_e_response_code_t modem_e_set_suspend( const void* context, const modem_e_suspend_t suspend )
{
    const uint8_t cbuffer[MODEM_E_SET_SUSPEND_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_SET_SUSPEND_MODEM_COM_CMD,
        suspend,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_SUSPEND_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_get_suspend( const void* context, modem_e_suspend_t* suspend )
{
    const uint8_t cbuffer[MODEM_E_GET_SUSPEND_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_SUSPEND_MODEM_COM_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_read( context, cbuffer, MODEM_E_GET_SUSPEND_CMD_LENGTH,
                                                               ( uint8_t* ) suspend, sizeof( uint8_t ) );
}

modem_e_response_code_t modem_e_set_alarm_timer( const void* context, uint32_t seconds )
{
    const uint8_t cbuffer[MODEM_E_SET_ALARM_TIMER_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_SET_ALARM_TIMER_CMD,
        ( uint8_t ) ( seconds >> 24 ),
        ( uint8_t ) ( seconds >> 16 ),
        ( uint8_t ) ( seconds >> 8 ),
        ( uint8_t ) seconds,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_SET_ALARM_TIMER_CMD_LENGTH, 0,
                                                                0 );
}

modem_e_response_code_t modem_e_clear_alarm_timer( const void* context )
{
    const uint8_t cbuffer[MODEM_E_CLEAR_ALARM_TIMER_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_CLEAR_ALARM_TIMER_CMD,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_CLEAR_ALARM_TIMER_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_get_alarm_remaining_time( const void* context, uint32_t* remaining_time )
{
    uint8_t rbuffer[sizeof( uint32_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_ALARM_REMAINING_TIME_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_ALARM_REMAINING_TIME_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_ALARM_REMAINING_TIME_CMD_LENGTH, rbuffer, sizeof( uint32_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *remaining_time = ( ( uint32_t ) rbuffer[0] << 24 ) + ( ( uint32_t ) rbuffer[1] << 16 ) +
                          ( ( uint32_t ) rbuffer[2] << 8 ) + ( ( uint32_t ) rbuffer[3] );
    }

    return rc;
}

modem_e_response_code_t modem_e_get_crashlog( const void* context, modem_e_crashlog_status_t* status,
                                              uint8_t* crashlog )
{
    uint8_t rbuffer[MODEM_E_GET_CRASHLOG_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_GET_CRASHLOG_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_GET_CRASHLOG_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_GET_CRASHLOG_CMD_LENGTH, rbuffer, MODEM_E_GET_CRASHLOG_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *status = ( modem_e_crashlog_status_t ) rbuffer[0];

        for( uint8_t i = 0; i < MODEM_E_GET_CRASHLOG_RBUFFER_LENGTH - 1; i++ )
        {
            crashlog[i] = rbuffer[1 + i];
        }
    }

    return rc;
}

modem_e_response_code_t modem_e_store_state_snapshot_to_nvm( const void* context, uint32_t* nvm_write_counter )
{
    uint8_t rbuffer[MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_RBUFFER_LENGTH] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_CMD,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_CMD_LENGTH, rbuffer,
        MODEM_E_STORE_STATE_SNAPSHOT_TO_NVM_RBUFFER_LENGTH );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *nvm_write_counter = ( ( uint32_t ) rbuffer[0] << 24 ) + ( ( uint32_t ) rbuffer[1] << 16 ) +
                             ( ( uint32_t ) rbuffer[2] << 8 ) + ( ( uint32_t ) rbuffer[3] );
    }

    return rc;
}

modem_e_response_code_t modem_e_restore_state_snapshot_from_nvm( const void* context, uint64_t elapsed_time_ms )
{
    const uint8_t cbuffer[MODEM_E_RESTORE_STATE_SNAPSHOT_FROM_NVM_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_RESTORE_STATE_SNAPSHOT_FROM_NVM_CMD,
        ( uint8_t ) ( elapsed_time_ms >> 56 ),
        ( uint8_t ) ( elapsed_time_ms >> 48 ),
        ( uint8_t ) ( elapsed_time_ms >> 40 ),
        ( uint8_t ) ( elapsed_time_ms >> 32 ),
        ( uint8_t ) ( elapsed_time_ms >> 24 ),
        ( uint8_t ) ( elapsed_time_ms >> 16 ),
        ( uint8_t ) ( elapsed_time_ms >> 8 ),
        ( uint8_t ) elapsed_time_ms,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write(
        context, cbuffer, MODEM_E_RESTORE_STATE_SNAPSHOT_FROM_NVM_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_mode_start( const void* context )
{
    const uint8_t test_msg[8] = { 'T', 'E', 'S', 'T', 'T', 'E', 'S', 'T' };

    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_START_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_START,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_START_CMD_LENGTH, test_msg, 8 );
}

modem_e_response_code_t modem_e_test_exit( const void* context )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_EXIT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_EXIT,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_TEST_MODE_TST_EXIT_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_test_nop( const void* context )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_NOP_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_NOP,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_TEST_MODE_TST_NOP_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_test_tx_lora( const void* context, uint32_t frequency, int8_t tx_power,
                                              uint8_t payload_length, modem_e_tst_mode_lora_sf_t sf,
                                              modem_e_tst_mode_lora_bw_t bw, modem_e_tst_mode_lora_cr_t cr,
                                              bool is_iq_inverted, bool is_crc_enabled,
                                              modem_e_tst_mode_lora_packet_header_mode_t header_mode,
                                              uint32_t preamble_length, uint32_t number_of_tx, uint32_t delay_ms,
                                              uint8_t syncword )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_TX_LORA_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_TX_LORA,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) tx_power,
        payload_length,
        ( uint8_t ) sf,
        ( uint8_t ) bw,
        ( uint8_t ) cr,
        ( uint8_t ) ( is_iq_inverted ? 0x01 : 0x00 ),
        ( uint8_t ) ( is_crc_enabled ? 0x01 : 0x00 ),
        ( uint8_t ) header_mode,
        ( uint8_t ) ( preamble_length >> 24 ),
        ( uint8_t ) ( preamble_length >> 16 ),
        ( uint8_t ) ( preamble_length >> 8 ),
        ( uint8_t ) preamble_length,
        ( uint8_t ) ( number_of_tx >> 24 ),
        ( uint8_t ) ( number_of_tx >> 16 ),
        ( uint8_t ) ( number_of_tx >> 8 ),
        ( uint8_t ) number_of_tx,
        ( uint8_t ) ( delay_ms >> 24 ),
        ( uint8_t ) ( delay_ms >> 16 ),
        ( uint8_t ) ( delay_ms >> 8 ),
        ( uint8_t ) delay_ms,
        syncword,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_TX_LORA_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_tx_fsk( const void* context, uint32_t frequency, int8_t tx_power,
                                             uint8_t payload_length, uint32_t number_of_tx, uint32_t delay_ms )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_TX_FSK_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_TX_FSK,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) tx_power,
        payload_length,
        ( uint8_t ) ( number_of_tx >> 24 ),
        ( uint8_t ) ( number_of_tx >> 16 ),
        ( uint8_t ) ( number_of_tx >> 8 ),
        ( uint8_t ) number_of_tx,
        ( uint8_t ) ( delay_ms >> 24 ),
        ( uint8_t ) ( delay_ms >> 16 ),
        ( uint8_t ) ( delay_ms >> 8 ),
        ( uint8_t ) delay_ms,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_TX_FSK_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_tx_lr_fhss( const void* context, uint32_t frequency, int8_t tx_power,
                                                 uint8_t payload_length, modem_e_tst_mode_lr_fhss_grid_t grid,
                                                 modem_e_tst_mode_lr_fhss_bw_t bw, modem_e_tst_mode_lr_fhss_cr_t cr,
                                                 uint32_t number_of_tx, uint32_t delay_ms, bool is_hopping_enabled )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_TX_LR_FHSS_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_TX_LR_FHSS,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) tx_power,
        payload_length,
        ( uint8_t ) grid,
        ( uint8_t ) bw,
        ( uint8_t ) cr,
        ( uint8_t ) ( number_of_tx >> 24 ),
        ( uint8_t ) ( number_of_tx >> 16 ),
        ( uint8_t ) ( number_of_tx >> 8 ),
        ( uint8_t ) number_of_tx,
        ( uint8_t ) ( delay_ms >> 24 ),
        ( uint8_t ) ( delay_ms >> 16 ),
        ( uint8_t ) ( delay_ms >> 8 ),
        ( uint8_t ) delay_ms,
        is_hopping_enabled ? 0x01 : 0x00,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_TX_LR_FHSS_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_tx_cw( const void* context, uint32_t frequency, int8_t tx_power )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_CW_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_TX_CW,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) tx_power,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_TEST_MODE_TST_CW_CMD_LENGTH,
                                                                0, 0 );
}

modem_e_response_code_t modem_e_test_rx_lora_cont( const void* context, uint32_t frequency,
                                                   modem_e_tst_mode_lora_sf_t sf, modem_e_tst_mode_lora_bw_t bw,
                                                   modem_e_tst_mode_lora_cr_t cr )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_RX_LORA_CONT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_RX_LORA_CONT,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) sf,
        ( uint8_t ) bw,
        ( uint8_t ) cr,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_RX_LORA_CONT_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_rx_fsk_cont( const void* context, uint32_t frequency )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_RX_FSK_CONT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_RX_FSK_CONT,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_RX_FSK_CONT_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_read_packet_counter_rx_cont( const void* context, uint32_t* rx_packet_counter )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_READ_PKT_COUNTER_RX_CONT_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_READ_RX_PKT_COUNTER_RX_CONT,
    };

    uint8_t rbuffer[sizeof( uint32_t )] = { 0 };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_TEST_MODE_TST_READ_PKT_COUNTER_RX_CONT_CMD_LENGTH, rbuffer, sizeof( uint32_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *rx_packet_counter = ( ( uint32_t ) rbuffer[0] << 24 ) + ( ( uint32_t ) rbuffer[1] << 16 ) +
                             ( ( uint32_t ) rbuffer[2] << 8 ) + ( ( uint32_t ) rbuffer[3] );
    }

    return rc;
}

modem_e_response_code_t modem_e_test_rssi_subghz( const void* context, uint32_t frequency, uint16_t time_ms,
                                                  uint32_t bw_hz )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_RSSI_SUBGHZ_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_RSSI_SUBGHZ,
        ( uint8_t ) ( ( frequency & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( frequency & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( frequency & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( frequency & 0x000000FF ),
        ( uint8_t ) ( time_ms >> 8 ),
        ( uint8_t ) ( time_ms ),
        ( uint8_t ) ( ( bw_hz & 0xFF000000 ) >> 24 ),
        ( uint8_t ) ( ( bw_hz & 0x00FF0000 ) >> 16 ),
        ( uint8_t ) ( ( bw_hz & 0x0000FF00 ) >> 8 ),
        ( uint8_t ) ( bw_hz & 0x000000FF ),
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_RSSI_SUBGHZ_CMD_LENGTH, 0, 0 );
}

modem_e_response_code_t modem_e_test_read_rssi( const void* context, int16_t* rssi )
{
    uint8_t rbuffer[sizeof( uint8_t )] = { 0x00 };

    const uint8_t cbuffer[MODEM_E_TEST_MODE_READ_RSSI_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_READ_RSSI,
    };

    const modem_e_response_code_t rc = ( modem_e_response_code_t ) modem_e_modem_hal_read(
        context, cbuffer, MODEM_E_TEST_MODE_READ_RSSI_CMD_LENGTH, rbuffer, sizeof( uint8_t ) );

    if( rc == MODEM_E_RESPONSE_CODE_OK )
    {
        *rssi = ( int16_t ) ( ( int8_t ) rbuffer[0] - 64 );
    }

    return rc;
}

modem_e_response_code_t modem_e_test_radio_rst( const void* context )
{
    const uint8_t cbuffer[MODEM_E_TEST_MODE_TST_RADIO_RST_CMD_LENGTH] = {
        ( uint8_t ) ( MODEM_E_GROUP_ID_MODEM >> 8 ),
        ( uint8_t ) MODEM_E_GROUP_ID_MODEM,
        MODEM_E_TEST_CMD,
        MODEM_E_TEST_MODE_TST_RADIO_RST,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer,
                                                                MODEM_E_TEST_MODE_TST_RADIO_RST_CMD_LENGTH, 0, 0 );
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

void modem_e_parse_charge_hook_id( const uint8_t* buffer, uint8_t hook_id, modem_e_consumption_details_t* hook )
{
    hook->tx_last_toa_ms      = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 0 );
    hook->rx_last_toa_ms      = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 4 );
    hook->tx_cumulated_toa_ms = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 8 );
    hook->rx_cumulated_toa_ms = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 12 );
    hook->none_consumption_ms = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 16 );
    hook->tx_consumption_ma   = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 20 );
    hook->rx_consumption_ma   = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 24 );
    hook->none_consumption_ma = modem_e_uint8_to_uint32( buffer + 32 * hook_id + 28 );
}

/* --- EOF ------------------------------------------------------------------ */
