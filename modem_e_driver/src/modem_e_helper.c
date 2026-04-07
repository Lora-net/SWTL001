/*!
 * @file      modem_e_helper.c
 *
 * @brief     helper functions implementation for Modem-E
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

#include "modem_e_helper.h"
#include "modem_e_common.h"
#include "modem_e_modem.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

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

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

modem_e_helper_status_t modem_e_helper_get_event_data( const void* context, modem_e_event_t* modem_event )
{
    modem_e_helper_status_t status       = MODEM_E_HELPER_STATUS_ERROR;
    modem_e_event_fields_t  event_fields = { 0 };

    const modem_e_response_code_t modem_response_code = modem_e_get_event( context, &event_fields );

    if( modem_response_code == MODEM_E_RESPONSE_CODE_OK )
    {
        status                     = MODEM_E_HELPER_STATUS_OK;
        modem_event->event_type    = event_fields.event_type;
        modem_event->missed_events = event_fields.missed_events_count;

        switch( modem_event->event_type )
        {
            break;
        case MODEM_E_LORAWAN_EVENT_TX_DONE:
            modem_event->event_data.txdone.status = ( modem_e_tx_done_event_t ) ( event_fields.data >> 8 );
            break;
        case MODEM_E_LORAWAN_EVENT_LINK_CHECK:
            modem_event->event_data.link_check.status = ( modem_e_link_check_event_t ) ( event_fields.data >> 8 );
            break;
        case MODEM_E_LORAWAN_EVENT_LORAWAN_MAC_TIME:
            modem_event->event_data.mac_time.status = ( modem_e_mac_time_event_t ) ( event_fields.data >> 8 );
            break;
        case MODEM_E_LORAWAN_EVENT_CLASS_B_PING_SLOT_INFO:
            modem_event->event_data.ping_slot_info.status =
                ( modem_e_class_b_ping_slot_info_t ) ( event_fields.data >> 8 );
            break;
        case MODEM_E_LORAWAN_EVENT_CLASS_B_STATUS:
            modem_event->event_data.ping_slot_status.status =
                ( modem_e_class_b_ping_slot_status_t ) ( event_fields.data >> 8 );
            break;
        case MODEM_E_LORAWAN_EVENT_NEW_MULTICAST_SESSION_CLASS_C:
        {
            modem_event->event_data.new_multicast_class_c_groupid.mc_group_id = ( uint8_t ) ( event_fields.data >> 8 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_NEW_MULTICAST_SESSION_CLASS_B:
        {
            modem_event->event_data.new_multicast_class_b_groupid.mc_group_id = ( uint8_t ) ( event_fields.data >> 8 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_RELAY_TX_DYNAMIC:
        {
            modem_event->event_data.relay_tx_dynamic_status.status =
                ( modem_e_relay_tx_dynamic_status_t ) ( event_fields.data >> 8 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_RELAY_TX_MODE:
        {
            modem_event->event_data.relay_tx_mode_status.status =
                ( modem_e_relay_tx_mode_status_t ) ( event_fields.data >> 8 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_RELAY_TX_SYNC:
        {
            modem_event->event_data.relay_tx_sync_status.status =
                ( modem_e_relay_tx_sync_status_t ) ( event_fields.data >> 8 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_FUOTA_DONE:
        {
            modem_event->event_data.fuota_status.status =
                ( modem_e_fuota_status_t ) ( event_fields.data >> 8 ) & 0x00FF;
            break;
        }
        case MODEM_E_LORAWAN_EVENT_TEST_MODE:
        {
            modem_event->event_data.test_mode_status.status =
                ( modem_e_test_mode_status_t ) ( event_fields.data >> 8 ) & 0x00FF;
            break;
        }
        case MODEM_E_LORAWAN_EVENT_REGIONAL_DUTY_CYCLE:
        {
            modem_event->event_data.regional_duty_cycle_status.status =
                ( modem_e_regional_duty_cycle_status_t ) ( ( uint8_t ) ( event_fields.data >> 8 ) );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_DR_BACKOFF_LIMIT:
        {
            modem_event->event_data.adr_backoff_limit_status.stack_adr_limit =
                ( ( event_fields.data & 0x0100 ) == 0x0100 );
            modem_event->event_data.adr_backoff_limit_status.user_adr_limit =
                ( ( event_fields.data & 0x0200 ) == 0x0200 );
            break;
        }
        case MODEM_E_LORAWAN_EVENT_ALARM:
        case MODEM_E_LORAWAN_EVENT_JOINED:
        case MODEM_E_LORAWAN_EVENT_JOIN_FAIL:
        case MODEM_E_LORAWAN_EVENT_DOWN_DATA:
        case MODEM_E_LORAWAN_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_C:
        case MODEM_E_LORAWAN_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_B:
        case MODEM_E_LORAWAN_EVENT_ALC_SYNC_TIME:
        case MODEM_E_LORAWAN_EVENT_RESET_REQUEST:
        default:
            break;
        }
    }

    return status;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
