/*!
 * @file      modem_e_helper.h
 *
 * @brief     helper functions definition for Modem-E
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

#ifndef MODEM_E_HELPER_H
#define MODEM_E_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "modem_e_modem_types.h"

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

/**
 * @brief Modem-E helper status
 */
typedef enum
{
    MODEM_E_HELPER_STATUS_OK    = 0,
    MODEM_E_HELPER_STATUS_ERROR = 3,
} modem_e_helper_status_t;

/*!
 * @brief TX status values
 *
 * These Tx statuses refer to the status of the complete LoRaWAN exchange, including uplink and downlink if any (for
 * instance in a Tx/Rx1/Rx2 LoRaWAN class A sequence).
 *
 * @see MODEM_E_LORAWAN_EVENT_TX_DONE
 */
typedef enum
{
    MODEM_E_TX_NOT_SENT =
        0x00,  //!< The Tx failed, possible reasons are:
               //!<  - payload is higher than maximal possible length (see @ref modem_e_get_next_tx_max_payload)
               //!<  - a network leave has been initiated before the uplink termination (see @ref
               //!<  modem_e_leave_network)
               //!<  - duty-cycle (if applicable) forbids next Tx
               //!<  - LBT (if applicable) detected activity and blocked the Tx
               //!<  - the relay WOR ACK has not been received (if applicable)
    MODEM_E_UNCONFIRMED_TX = 0x01,  //!< The meaning of the status depends on the requested uplink:
                                    //!<   - for unconfirmed uplink it means the exchange sequence has been executed
                                    //!<   - for confirmed uplink, it means the exchange sequence has terminated without
                                    //!<   receiving confirmation from network
    MODEM_E_CONFIRMED_TX = 0x02,    //!< The uplink has been confirmed by network
} modem_e_tx_done_event_t;

/*!
 * @brief Link check request values
 */
typedef enum
{
    MODEM_E_LINK_CHECK_NOT_RECEIVED = 0x00,  //!< Link check response has not been received
    MODEM_E_LINK_CHECK_RECEIVED     = 0x01,  //!< Link check response has been received
} modem_e_link_check_event_t;

/*!
 * @brief Time request values
 *
 * @see MODEM_E_LORAWAN_EVENT_LORAWAN_MAC_TIME
 */
typedef enum
{
    MODEM_E_TIME_NOT_VALID          = 0x00,  //!< Time is not valid
    MODEM_E_TIME_VALID              = 0x01,  //!< Time is valid and has been synchronized
    MODEM_E_TIME_VALID_BUT_NOT_SYNC = 0x02,  //!< Time is still valid but has not been synchronized
} modem_e_mac_time_event_t;

/*!
 * @brief class b ping slot info
 *
 * @see MODEM_E_LORAWAN_EVENT_CLASS_B_PING_SLOT_INFO
 */
typedef enum
{
    MODEM_E_CLASS_B_PING_SLOT_INFO_NOT_ANSWERED = 0x00,
    MODEM_E_CLASS_B_PING_SLOT_INFO_ANSWERED     = 0x01,
} modem_e_class_b_ping_slot_info_t;

/*!
 * @brief class b ping slot status
 *
 * @see MODEM_E_LORAWAN_EVENT_CLASS_B_STATUS
 */
typedef enum
{
    MODEM_E_CLASS_B_PING_SLOT_STATUS_NOT_READY = 0x00,
    MODEM_E_CLASS_B_PING_SLOT_STATUS_READY     = 0x01,
} modem_e_class_b_ping_slot_status_t;

/*!
 * @brief Event status for Wake On Radio protocol status change
 *
 * @see MODEM_E_LORAWAN_EVENT_RELAY_TX_DYNAMIC
 */
typedef enum
{
    MODEM_E_RELAY_TX_DYNAMIC_WOR_DISABLED = 0x00,
    MODEM_E_RELAY_TX_DYNAMIC_WOR_ENABLED  = 0x01,
} modem_e_relay_tx_dynamic_status_t;

/*!
 * @brief Event status for relay Tx activation change
 *
 * @see MODEM_E_LORAWAN_EVENT_RELAY_TX_MODE
 */
typedef enum
{
    MODEM_E_RELAY_TX_MODE_DISABLED          = 0x00,
    MODEM_E_RELAY_TX_MODE_ENABLED           = 0x01,
    MODEM_E_RELAY_TX_MODE_DYNAMIC           = 0x02,
    MODEM_E_RELAY_TX_MODE_DEVICE_CONTROLLED = 0x03,
} modem_e_relay_tx_mode_status_t;

/*!
 * @brief Event status for relay synchronization change
 *
 * @see MODEM_E_LORAWAN_EVENT_RELAY_TX_SYNC
 */
typedef enum
{
    MODEM_E_RELAY_TX_SYNCHRONIZATION_INIT = 0x00,
    MODEM_E_RELAY_TX_UNSYNCHRONIZED       = 0x01,
    MODEM_E_RELAY_TX_SYNCHRONIZED         = 0x02,
} modem_e_relay_tx_sync_status_t;

/**
 * @brief Event status on FUOTA done event
 *
 * @see MODEM_E_LORAWAN_EVENT_FUOTA_DONE
 */
typedef enum
{
    MODEM_E_FUOTA_STATUS_TERMINATED_SUCCESSFULLY = 0x00,  //!< FUOTA terminated successfully
    MODEM_E_FUOTA_STATUS_FAILED                  = 0x01,  //!< FUOTA failed
    MODEM_E_FUOTA_STATUS_ONGOING                 = 0xFF,  //!< FUOTA is ongoing
    MODEM_E_FUOTA_STATUS_NOT_STARTED             = 0xFE,  //!< FUOTA is not started
} modem_e_fuota_status_t;

/**
 * @brief Event status on test mode event
 *
 * @see MODEM_E_LORAWAN_EVENT_TEST_MODE
 */
typedef enum
{
    MODEM_E_TEST_MODE_STATUS_TX_NOT_SENT       = 0x00,
    MODEM_E_TEST_MODE_STATUS_TX_SENT           = 0x01,
    MODEM_E_TEST_MODE_STATUS_TERMINATED_ACTION = 0x02,
} modem_e_test_mode_status_t;

/**
 * @brief Event status on regional duty cycle event
 *
 * The duty cycle status can be obtained by calling @ref modem_e_get_duty_cycle_status.
 *
 * @see modem_e_get_duty_cycle_status
 */
typedef enum
{
    MODEM_E_REGINAL_DUTY_CYCLE_TX_ALLOWED = 0x00,  //!< Previously duty cycle constrained transmissions are now allowed
    MODEM_E_REGINAL_DUTY_CYCLE_TX_CONSTRAINED = 0x01,  //!< Transmissions are constrained by regional duty cycle
} modem_e_regional_duty_cycle_status_t;

/**
 * @brief Structure holding event-related data
 */
typedef struct
{
    modem_e_lorawan_event_type_t event_type;     //!< Type of the event
    uint8_t                      missed_events;  //!< Number of @p event_type events missed before the current one
    union
    {
        struct
        {
            modem_e_tx_done_event_t status;
        } txdone;
        struct
        {
            modem_e_link_check_event_t status;
        } link_check;
        struct
        {
            modem_e_mac_time_event_t status;
        } mac_time;
        struct
        {
            modem_e_class_b_ping_slot_info_t status;
        } ping_slot_info;
        struct
        {
            modem_e_class_b_ping_slot_status_t status;
        } ping_slot_status;
        struct
        {
            uint8_t mc_group_id;
        } new_multicast_class_c_groupid;
        struct
        {
            uint8_t mc_group_id;
        } new_multicast_class_b_groupid;
        struct
        {
            modem_e_relay_tx_dynamic_status_t status;
        } relay_tx_dynamic_status;
        struct
        {
            modem_e_relay_tx_mode_status_t status;
        } relay_tx_mode_status;
        struct
        {
            modem_e_relay_tx_sync_status_t status;
        } relay_tx_sync_status;
        struct
        {
            modem_e_fuota_status_t status;
        } fuota_status;
        struct
        {
            modem_e_test_mode_status_t status;
        } test_mode_status;
        struct
        {
            modem_e_regional_duty_cycle_status_t status;
        } regional_duty_cycle_status;
        struct
        {
            bool stack_adr_limit;  //!< The threshold that generated this event comes from the stack datarate backoff
                                   //!< limit
            bool user_adr_limit;   //!< The threshold that generated this event comes from the user provided limit
        } adr_backoff_limit_status;
    } event_data;  //!< Status data associated to the event
} modem_e_event_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Extract the event data contained in the event field buffer
 *
 * @param [in] context Chip implementation context
 * @param [out] modem_event Struct containing the event data \see modem_e_event_t
 *
 * @returns  Operation status
 */
modem_e_helper_status_t modem_e_helper_get_event_data( const void* context, modem_e_event_t* modem_event );

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_HELPER_H

/* --- EOF ------------------------------------------------------------------ */
