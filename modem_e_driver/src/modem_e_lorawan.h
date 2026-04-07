/*!
 * @file      modem_e_lorawan.h
 *
 * @brief     LoRaWAN driver for Modem-E
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

#ifndef MODEM_E_LORAWAN_H
#define MODEM_E_LORAWAN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdint.h>
#include "modem_e_common.h"
#include "modem_e_lorawan_types.h"

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

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Return the version of the LoRaWAN standard and of the regional parameters implemented
 *
 * @param [in] context Chip implementation context
 * @param [out] lorawan_version LoRaWAN and regional parameters version implemented
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_lorawan_version( const void* context, modem_e_lorawan_version_t* lorawan_version );

/*!
 * @brief Return the Over-the-air Activation (OTAA) Device EUI
 *
 * @param [in] context Chip implementation context
 * @param [out] dev_eui Device EUI buffer on 8 bytes
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_otaa_dev_eui( const void* context, modem_e_otaa_dev_eui_t dev_eui );

/*!
 * @brief Set the Over-the-air Activation (OTAA) Device EUI
 *
 * The device EUI is stored in non-volatile upon this call but the keys are not derived.
 * Refer to @ref modem_e_derive_keys for keys derivation.
 *
 * @param [in] context Chip implementation context
 * @param [in] dev_eui Device EUI buffer on 8 bytes
 *
 * @returns Operation status
 *
 * @see modem_e_derive_keys
 */
modem_e_response_code_t modem_e_set_otaa_dev_eui( const void* context, const modem_e_otaa_dev_eui_t dev_eui );

/*!
 * @brief Return the Over-the-air Activation (OTAA) join EUI
 *
 * @remark Join EUI is also known as Application Identifier in LoRaWan v1.0.4.
 *
 * @param [in] context Chip implementation context
 * @param [out] join_eui Join EUI buffer on 8 bytes
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_otaa_join_eui( const void* context, modem_e_otaa_join_eui_t join_eui );

/*!
 * @brief Set the Over-the-air Activation (OTAA) Join EUI
 *
 * The device EUI is stored in non-volatile upon this call but the keys are not derived.
 * Refer to @ref modem_e_derive_keys for keys derivation.
 *
 * @remark Join EUI is also known as Application Identifier in LoRaWan v1.0.4.
 *
 * @param [in] context Chip implementation context
 * @param [in] join_eui Join EUI buffer on 8 bytes
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_otaa_join_eui( const void* context, const modem_e_otaa_join_eui_t join_eui );

/*!
 * @brief Set the Over-the-air Activation (OTAA) network key
 *
 * The network key is named:
 *   - Application Key (AppKey) in LoRaWAN L2 1.0.4 Specification
 *   - NwkKey in LoRaWAN 1.1 Specification and after
 *
 * This key is reset by a factory reset @ref modem_e_factory_reset.
 *
 * @param [in] context Chip implementation context
 * @param [in] nwk_key Network Key buffer on 16 bytes
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_otaa_nwk_key( const void* context, const modem_e_otaa_nwk_key_t nwk_key );

/*!
 * @brief Set the Over-the-air Activation (OTAA) application key
 *
 * The application key is named GenAppKey in LoRaWAN Remote Multicast Setup v1.0.0 Specification.
 *
 * This key is reset by a factory reset @ref modem_e_factory_reset.
 *
 * @note This does not set the key named AppKey in LoRaWAN L2 1.0.4 Specification. To set the AppKey corresponding to
 * LoRaWAN L2 1.0.4 Specification, use @ref modem_e_set_otaa_nwk_key.
 *
 * @param [in] context Chip implementation context
 * @param [in] app_key Application Key buffer on 16 bytes
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_otaa_app_key( const void* context, const modem_e_otaa_app_key_t app_key );

/*!
 * @brief Use the previously set of JoinEUI/DevEUI to derive the app keys used in the Semtech join server
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_derive_keys( const void* context );

/*!
 * @brief Get the LoRaWAN device class
 *
 * @param [in] context Chip implementation context
 * @param [out] modem_class LoRaWAN device class @ref modem_e_classes_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_class( const void* context, modem_e_classes_t* modem_class );

/*!
 * @brief Set the LoRaWAN device class
 *
 * @param [in] context Chip implementation context
 * @param [in] modem_class LoRaWAN device class @ref modem_e_classes_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_class( const void* context, const modem_e_classes_t modem_class );

/*!
 * @brief Return the regulatory region
 *
 * @param [in] context Chip implementation context
 * @param [out] region LoRaWAN regulatory region @ref modem_e_regions_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_region( const void* context, modem_e_regions_t* region );

/*!
 * @brief Set the regulatory region
 *
 * Additionally this command resets the ADR profile to Network Server Controlled. If different ADR profile is desired,
 * the profile needs to be set again.
 *
 * @param [in] context Chip implementation context
 * @param [in] region LoRaWAN regulatory region @ref modem_e_regions_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_region( const void* context, const modem_e_regions_t region );

/*!
 * @brief Connect LoRaWAN network through Over-The-Air-Activation (OTAA)
 *
 * The join procedure started by this command stops either if the join succeed or if @ref modem_e_leave_network is
 * called to cancel the join procedure. In case of join failure, the procedure automatically starts again.
 *
 * During the join procedure no further transmissions can occur. When the network has been successfully joined, a Joined
 * event is generated. If the device is already joined to a network, or is in the process of joining, this command has
 * no effect.
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 *
 * @see modem_e_connect_with_abp
 */
modem_e_response_code_t modem_e_join( const void* context );

/*!
 * @brief Leave joined network or cancel ongoing join process
 *
 * After leaving the network, no further transmissions can occur.
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_leave_network( const void* context );

/*!
 * @brief Get maximal length of next uplink payload
 *
 * This command returns the maximum application payload size possible according to the LoRaWAN regional
 * parameters for the next transmission using the current data rate.
 *
 * It takes into account the presence of FOpts in the next payload.
 * However, given that FOpts can be added after downlink reception, the returned value takes it into account if this
 * function is called after reception of downlink.
 *
 * @note If this device is connected behind a relay, then 19 bytes are added to the payload.
 * These 19 bytes are not taken into account by this function.
 * Therefore it must be subtracted from the returned value.
 *
 * @param [in] context Chip implementation context
 * @param [out] tx_max_payload Maximum application payload size possible
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_next_tx_max_payload( const void* context, uint8_t* tx_max_payload );

/*!
 * @brief Request uplink
 *
 * This command requests sending the given data on the specified port as an unconfirmed (0x00) or confirmed
 * (0x01) frame. The request will be queued and the frame will be sent as soon as the current bandwidth usage of the
 * regulatory region permits. A TxDone event is generated when the frame either has been sent, or if it couldn’t be sent
 * because the specified data exceeded the maximum possible payload size.
 *
 * @param [in] context Chip implementation context
 * @param [in] port LoRaWAN port
 * @param [in] uplink_type Uplink type unconfirmed (0x00) or confirmed (0x01) @ref modem_e_uplink_type_t
 * @param [in] data Data buffer. It is up to the caller that @p data contains at least @p length elements
 * @param [in] length Data length. Valid value in [0,242]
 *
 * @remark The application shall not use port 0 or the LoRaWAN test port 224 as well as the ports from 225 to 255 since
 * they are reserved for future standardized application extensions.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_request_tx( const void* context, const uint8_t port,
                                            const modem_e_uplink_type_t uplink_type, const uint8_t* data,
                                            const uint8_t length );

/*!
 * @brief Request empty uplink
 *
 * This command requests sending an empty payload on the specified port as an unconfirmed (0x00) or confirmed
 * (0x01) frame. The request will be queued and the frame will be sent as soon as the current bandwidth usage of the
 * regulatory region permits. A TxDone event is generated when the frame has been sent.
 *
 * @param [in] context Chip implementation context
 * @param [in] is_fport_populated Indicate whether the fport should be included in the empty frame or not
 * @param [in] port LoRaWAN port. Only meaningful if @p is_fport_populated is true
 * @param [in] uplink_type Uplink type unconfirmed (0x00) or confirmed (0x01) @ref modem_e_uplink_type_t
 *
 * @remark The application shall not use port 0 or the LoRaWAN test port 224 as well as the ports from 225 to 255 since
 * they are reserved for future standardized application extensions.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_request_empty_tx( const void* context, bool is_fport_populated, const uint8_t port,
                                                  const modem_e_uplink_type_t uplink_type );

/*!
 * @brief Request emergency uplink
 *
 * An emergency uplink request has highest precedence than any other actions the chip may be doing, and the duty-cycle
 * limitation is not honored. LoRaWAN payload size limitations still applies.
 *
 * It is intended to be used for alarm-like event that requires real time signaling. However, due to the relaxed
 * limitations, it must be used with care.
 *
 * This command sends the given data on the specified port as an unconfirmed (0x00) or confirmed (0x01) frame
 * immediately. It has higher priority than all other services and does not take duty cycle or payload size restrictions
 * into account
 *
 * @param [in] context Chip implementation context
 * @param [in] port LoRaWAN port
 * @param [in] uplink_type Uplink type unconfirmed (0x00) or confirmed (0x01) @ref modem_e_uplink_type_t
 * @param [in] data Data buffer. It is up to the caller that @p data contains at least @p length elements
 * @param [in] length Data length. Valid value in [0,242]
 *
 * @remark The application shall not use port 0 or the LoRaWAN test port 224 as well as the ports from 225 to 255 since
 * they are reserved for future standardized application extensions.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_emergency_tx( const void* context, const uint8_t port,
                                              const modem_e_uplink_type_t uplink_type, const uint8_t* data,
                                              const uint8_t length );

/*!
 * @brief Get the size of latest downlink
 *
 * This command must be called in the following sequence, without any other commands in between:
 * 1. @ref modem_e_get_downlink_data_size
 * 2. @ref modem_e_get_downlink_data
 * 3. @ref modem_e_get_downlink_metadata
 *
 * @param [in] context Chip implementation context
 * @param [out] downlink_data_size Size of the received downlink
 * @param [out] remaining_downlinks Number of remaining downlinks in the internal FiFo
 *
 * @see modem_e_get_downlink_data, modem_e_get_downlink_metadata
 */
modem_e_response_code_t modem_e_get_downlink_data_size( const void* context, uint8_t* downlink_data_size,
                                                        uint8_t* remaining_downlinks );

/*!
 * @brief Read the last downlink data
 *
 * This command must be called in the following sequence, without any other commands in between:
 * 1. @ref modem_e_get_downlink_data_size
 * 2. @ref modem_e_get_downlink_data
 * 3. @ref modem_e_get_downlink_metadata
 *
 * @param [in] context Chip implementation context
 * @param [in] buffer buffer containing the downlink data
 * @param [in] downlink_data_size Size of the payload to read
 *
 * @see modem_e_get_downlink_data_size, modem_e_get_downlink_metadata
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_downlink_data( const void* context, uint8_t* buffer, uint8_t downlink_data_size );

/*!
 * @brief Get the metadata of the last downlink
 *
 * This command must be called in the following sequence, without any other commands in between:
 * 1. @ref modem_e_get_downlink_data_size
 * 2. @ref modem_e_get_downlink_data
 * 3. @ref modem_e_get_downlink_metadata
 *
 * @param [in] context Chip implementation context
 * @param [out] metadata Downlink metada, @ref modem_e_downlink_metadata_t
 *
 * @see modem_e_get_downlink_data_size, modem_e_get_downlink_data
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_downlink_metadata( const void* context, modem_e_downlink_metadata_t* metadata );

/*!
 * @brief Get count of uplinks and duration since last received downlink
 *
 * @p lost_connection_counter is increased even in case of uplink retransmission (nb_trans), and is reset when a valid
 * downlink is received.
 * @p lost_connection_since_sec is reset when a downlink is received.
 *
 * @param [in] context Chip implementation context
 * @param [out] lost_connection_counter Number of uplinks sent without downlink since last received downlink
 * @param [out] lost_connection_since_sec Duration since last downlink received
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_lost_connection_counter( const void* context, uint16_t* lost_connection_counter,
                                                             uint32_t* lost_connection_since_sec );

/*!
 * @brief Get the LoRaWAN network type
 *
 * @param [in] context Chip implementation context
 * @param [out] network_type Type of LoRaWAN network (see @ref modem_e_network_type_t)
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_network_type( const void* context, modem_e_network_type_t* network_type );

/*!
 * @brief Set the LoRaWAN network type
 *
 * By default the @ref MODEM_E_LORAWAN_PUBLIC_NETWORK is set.
 *
 * @param [in] context Chip implementation context
 * @param [in] network_type @ref modem_e_network_type_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_network_type( const void* context, const modem_e_network_type_t network_type );

/*!
 * @brief Get the LoRaWAN certification mode enable status
 *
 * @param [in] context Chip implementation context
 * @param [out] enable @ref modem_e_certification_mode_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_certification_mode( const void* context, modem_e_certification_mode_t* enable );

/*!
 * @brief Activate/deactivate the LoRaWAN certification mode
 *
 * Upon certification activation, the Modem-E starts a join sequence. The duty-cycle is disabled after the join is
 * accepted. Then the Modem-E automatically replies to certification test commands.
 *
 * The certification mode activation status is stored in non-volatile memory so that the Modem-E can re-join with
 * certification mode enable after a reset during certification test sequence.
 *
 * When certification tests are terminated, this command must be called to explicitly disable the certification mode.
 * The duty-cycle is re-enabled when the certification mode is disabled.
 *
 * @param [in] context Chip implementation context
 * @param [in] enable @ref modem_e_certification_mode_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_certification_mode( const void*                        context,
                                                        const modem_e_certification_mode_t enable );

/*!
 * @brief Get Duty cycle status info
 *
 * The sign determines the meaning of the value:
 * - duty_cycle >= 0: duty_cycle is the time budget in millisecond still available for transmission
 * - duty_cycle < 0: Abs(duty_cycle) is the time in millisecond before it can start transmitting again
 *
 * @param [in] context Chip implementation context
 * @param [out] duty_cycle Time in milliseconds (see notes for explanations)
 *
 * @note When duty cycle is deactivated, the returned value is 0.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_duty_cycle_status( const void* context, int32_t* duty_cycle );

/*!
 * @brief Get the available data rate mask
 *
 * One bit indicates one data rate. Bit n = 1 mean Data Rate n is available.
 *
 * @param [in] context Chip implementation context
 * @param [out] available_data_rate Available data rate bit mask
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_available_data_rate( const void* context, uint16_t* available_data_rate );

/*!
 * @brief Return the ADR profile type
 *
 * @param [in] context Chip implementation context
 * @param [out] adr_profile ADR profile type @ref modem_e_adr_profiles_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_adr_profile( const void* context, modem_e_adr_profiles_t* adr_profile );

/*!
 * @brief Set the ADR profile and parameters
 *
 * @param [in] context Chip implementation context
 * @param [in] adr_profile ADR profile type @ref modem_e_adr_profiles_t
 * @param [in] adr_custom_list custom ADR profile consisting of a list of 16 preferred data rates
 *
 * @remark each call to the function reinitialize the data rate distribution.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_adr_profile( const void* context, const modem_e_adr_profiles_t adr_profile,
                                                 const uint8_t* adr_custom_list );

/*!
 * @brief Set the join data rate distribution
 *
 * @param [in] context Chip implementation context
 * @param [in] distribution 16-byte-array describing the distribution. It is up to the user to ensure it contains at
 * least 16 bytes
 *
 * @remark each call to the function reinitialize the data rate distribution.
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_join_data_rate_distribution(
    const void* context, const uint8_t distribution[MODEM_E_DATARATE_DISTRIBUTION_LENGTH] );

/*!
 * @brief Get the LoRaWAN number of retransmission
 *
 * @param [in] context Chip implementation context
 * @param [out] nb_trans Number of retransmission
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_nb_trans( const void* context, uint8_t* nb_trans );

/*!
 * @brief Set the LoRaWAN number of retransmission
 *
 * @param [in] context Chip implementation context
 * @param [in] nb_trans Number of retransmission. Valid values in [1:15]
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_nb_trans( const void* context, const uint8_t nb_trans );

/*!
 * @brief Get the ADR backoff mechanism parameters
 *
 * @param [in] context Chip implementation context
 * @param [out] limit The ADR ACK limit
 * @param [out] delay The ADR ACK delay
 *
 * @returns Operation status
 *
 * @see modem_e_set_adr_ack_limit_delay
 */
modem_e_response_code_t modem_e_get_adr_ack_limit_delay( const void* context, uint8_t* limit, uint8_t* delay );

/*!
 * @brief Set the ADR backoff mechanism parameters
 *
 * The ADR backoff mechanism is described in LoRaWAN specification ADR backoff sequence.
 * Once the end of the data rate backoff mechanism is reached, the event @ref MODEM_E_LORAWAN_EVENT_DR_BACKOFF_LIMIT is
 * raised.
 * Note that it is possible to provide a user threshold to raise @ref MODEM_E_LORAWAN_EVENT_DR_BACKOFF_LIMIT after a
 * certain number of uplink without downlink with @ref modem_e_set_user_dr_backoff_sequence_threshold.
 *
 * @param [in] context Chip implementation context
 * @param [in] limit The ADR ACK limit. Valid values in [2:127]
 * @param [in] delay The ADR ACK delay. Valid values in [2:127]
 *
 * @returns Operation status
 *
 * @see modem_e_get_adr_ack_limit_delay, modem_e_set_user_dr_backoff_sequence_threshold
 */
modem_e_response_code_t modem_e_set_adr_ack_limit_delay( const void* context, const uint8_t limit,
                                                         const uint8_t delay );

/*!
 * @brief Get the listen before talk state
 *
 * @param [in] context Chip implementation context
 * @param [out] enable @ref modem_e_lbt_mode_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_lbt_state( const void* context, modem_e_lbt_mode_t* enable );

/*!
 * @brief Activate the listen before talk
 *
 * @param [in] context Chip implementation context
 * @param [in] enable @ref modem_e_lbt_mode_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_lbt_state( const void* context, const modem_e_lbt_mode_t enable );

/*!
 * @brief Configure the listen before talk
 *
 * @param [in] context Chip implementation context
 * @param [in] duration LBT duration in ms, default value is 5 ms
 * @param [in] threshold LBT threshold in dBm, default value is -80 dBm
 * @param [in] bandwidth LBT bandwidth in Hz, default value is 200000 Hz
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_lbt_params( const void* context, const uint32_t duration, const int16_t threshold,
                                                const uint32_t bandwidth );

/*!
 * @brief Get the listen before talk configuration
 *
 * @param [in] context Chip implementation context
 * @param [out] duration LBT duration in ms, default value is 5 ms
 * @param [out] threshold LBT treshold in dBm, default value is -80 dBm
 * @param [out] bandwidth LBT bandwidth in Hz, default value is 200000 Hz
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_lbt_params( const void* context, uint32_t* duration, int16_t* threshold,
                                                uint32_t* bandwidth );

/*!
 * @brief Get the Carrier Sense Multiple Access (CSMA) state
 *
 * @param [in] context Chip implementation context
 * @param [out] enable @ref modem_e_csma_mode_t
 *
 * @returns Operation status
 *
 * @see modem_e_set_csma_state, modem_e_set_csma_params
 */
modem_e_response_code_t modem_e_get_csma_state( const void* context, modem_e_csma_mode_t* enable );

/*!
 * @brief Enable or disable Carrier Sense Multiple Access (CSMA)
 *
 * The CSMA mechanism is the one described in LoRa-Alliance document TR013-1.0.0 "Enabling CSMA for LoRaWAN".
 * It is enabled by default.
 *
 * @param [in] context Chip implementation context
 * @param [in] enable @ref modem_e_csma_mode_t
 *
 * @returns Operation status
 *
 * @see modem_e_get_csma_state, modem_e_set_csma_params
 */
modem_e_response_code_t modem_e_set_csma_state( const void* context, const modem_e_csma_mode_t enable );

/*!
 * @brief Configure the Carrier Sense Multiple Access (CSMA)
 *
 * Refer to LoRa-Alliance document TR013-1.0.0 "Enabling CSMA for LoRaWAN" for details concerning the configuration.
 *
 * @param [in] context Chip implementation context
 * @param [in] max_channel_change Maximum number of sensing to execute on a channel before selecting another one
 * @param [in] backoff_enable Indicate if backoff is enabled or disabled
 * @param [in] nb_backoff_max Maximal number of sensing per channel to conclude it is free
 *
 * @returns Operation status
 *
 * @see modem_e_get_csma_state, modem_e_set_csma_state
 */
modem_e_response_code_t modem_e_set_csma_params( const void* context, const uint8_t max_channel_change,
                                                 const modem_e_csma_backoff_mode_t backoff_enable,
                                                 const uint8_t                     nb_backoff_max );

/*!
 * @brief Get the Carrier Sense Multiple Access (CSMA) configuration
 *
 * @param [in] context Chip implementation context
 * @param [out] max_channel_change Maximum number of sensing to execute on a channel before selecting another one
 * @param [out] backoff_enable Indicate if backoff is enabled or disabled
 * @param [out] nb_backoff_max Maximal number of sensing per channel to conclude it is free
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_csma_params( const void* context, uint8_t* max_channel_change,
                                                 uint8_t* backoff_enable, uint8_t* nb_backoff_max );

/*!
 * @brief Request a MAC command uplink
 *
 * @param [in] context Chip implementation context
 * @param [in] mac_request Mask of modem_e_mac_request_t
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_mac_request_tx( const void* context, const modem_e_mac_request_bitmask_t mac_request );

/*!
 * @brief Get time from LoRaWAN network
 *
 * Time expressed as number of seconds since GPS epoch (January 6th 1980, 00:00:00).
 * The MAC time is available after a successfully responded call @ref modem_e_mac_request_tx with @ref
 * MODEM_E_MAC_REQUEST_TIME bit set.
 *
 * @param [in] context Chip implementation context
 * @param [out] gps_time_sec
 * @param [out] gps_fractionnal_sec
 *
 * @returns Operation status
 *
 * @see modem_e_mac_request_tx, modem_e_mac_request_bitmask_t
 */
modem_e_response_code_t modem_e_get_lorawan_mac_time( const void* context, uint32_t* gps_time_sec,
                                                      uint32_t* gps_fractionnal_sec );

/*!
 * @brief Get the link check result
 *
 * The Link Check Request is initiated by calling @ref modem_e_mac_request_tx with @ref
 * MODEM_E_MAC_REQUEST_LINK_CHECK bit set.
 *
 * @param [in] context Chip implementation context
 * @param [out] margin The link demodulation margin obtained from last network received Link Check Request in dB
 * @param [out] gateway_count Number of gateways that received the last  Link Check Request
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_link_check_data( const void* context, uint8_t* margin, uint8_t* gateway_count );

/*!
 * @brief Set the battery level
 *
 * The battery level is used by the Modem-E to respond to Device Status Request from LoRaWAN Network Server.
 *
 * @param [in] context Chip implementation context
 * @param [in] modem_vs_user Indicate if the battery level should be obtained from internal value or not
 * @param [in] value Battery level value to use. Only effective if @p modem_vs_user is @ref
 * MODEM_E_BATTERY_LEVEL_FROM_USER_VALUE. Possible values: 0 means the end device is connected to an external power
 * source, 1-254 is the battery level (1is the minimum, 254 the maximum), 255 means the end device was not able to
 * measure the battery level
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_battery_level( const void*                                context,
                                                   const modem_e_battery_level_source_value_t modem_vs_user,
                                                   uint8_t                                    value );

/*!
 * @brief Get the current class B ping slot periodicity
 *
 * @param [in] context Chip implementation context
 * @param [in] ping_slot_periodicity Current class B ping slot periodicity
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_class_b_ping_slot_periodicity( const void*                  context,
                                                                   modem_e_class_b_ping_slot_t* ping_slot_periodicity );

/*!
 * @brief Set the class B ping slot periodicity
 *
 * @param [in] context Chip implementation context
 * @param [out] ping_slot_periodicity Class B ping slot periodicity to set
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_class_b_ping_slot_periodicity(
    const void* context, const modem_e_class_b_ping_slot_t ping_slot_periodicity );

/*!
 * @brief Get the current multicast group configuration
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier
 * @param [in] mc_group_address Multicast group address
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_multicast_group_config( const void* context, const uint8_t mc_group_id,
                                                            uint32_t* mc_group_address );

/*!
 * @brief Set the multicast group configuration
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier
 * @param [in] mc_group_address Multicast group address
 * @param [in] mc_nwkskey Multicast network session key for this group
 * @param [in] mc_appskey Multicast application session key for this group
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_set_multicast_group_config( const void* context, const uint8_t mc_group_id,
                                                            const uint32_t mc_group_address, const uint8_t* mc_nwkskey,
                                                            const uint8_t* mc_appskey );

/*!
 * @brief Start a class C multicast session
 *
 * Several class C multicast sessions can run in parallel if configured with the same downlink_frequency and
 * downlink_data_rate.
 *
 * To have the class C multicast session started immediately upon call to @ref
 * modem_e_start_session_multicast_class_c, it is recommended to start class C before calling it.
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier
 * @param [in] downlink_frequency Downlink frequency for this session
 * @param [in] downlink_data_rate Datarate for this session
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_start_session_multicast_class_c( const void* context, const uint8_t mc_group_id,
                                                                 const uint32_t downlink_frequency,
                                                                 const uint8_t  downlink_data_rate );

/*!
 * @brief Get the class C multicast session status
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier to get the status
 * @param [out] modem_e_multicast_class_c_status Status of the multicast class C session corresponding to @p
 * mc_group_id
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_multicast_class_c_session_status(
    const void* context, const uint8_t mc_group_id,
    modem_e_multicast_class_c_status_t* modem_e_multicast_class_c_status );

/*!
 * @brief Stop given class C multicast session
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier to stop session
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_stop_session_multicast_class_c( const void* context, const uint8_t mc_group_id );

/*!
 * @brief Stop all class C multicast sessions
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_stop_all_session_multicast_class_c( const void* context );

/*!
 * @brief Start class B multicast session
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier
 * @param [in] downlink_frequency Downlink frequency for this session
 * @param [in] downlink_data_rate Datarate for this session
 * @param [in] ping_slot Ping slot periodicity for this session
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_start_session_multicast_class_b( const void* context, const uint8_t mc_group_id,
                                                                 const uint32_t                    downlink_frequency,
                                                                 const uint8_t                     downlink_data_rate,
                                                                 const modem_e_class_b_ping_slot_t ping_slot );

/*!
 * @brief Get current status of given class B multicast session
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id Multicast group identifier
 * @param [out] modem_e_multicast_class_b_status Status of the multicast class B session corresponding to @p
 * mc_group_id
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_get_multicast_class_b_session_status(
    const void* context, const uint8_t mc_group_id,
    modem_e_multicast_class_b_status_t* modem_e_multicast_class_b_status );

/*!
 * @brief Stop given class B multicast session
 *
 * @param [in] context Chip implementation context
 * @param [in] mc_group_id multicast group identifier to stop session
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_stop_session_multicast_class_b( const void* context, const uint8_t mc_group_id );
/*!
 * @brief Stop all class B multicast sessions
 *
 * @param [in] context Chip implementation context
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_stop_all_session_multicast_class_b( const void* context );

/**
 * @brief Start the Applicative Layer Clock synchronization service
 *
 * This corresponds to the LoRaWAN Application Layer Clock Synchronization v2.0.0 available in LoRa Alliance document
 * TS003-2.0.0.
 *
 * @param context Chip implementation context
 *
 * @return Operation status
 *
 * @see modem_e_alc_sync_stop_service, modem_e_alc_sync_get_time, modem_e_alc_sync_trig_request
 */
modem_e_response_code_t modem_e_alc_sync_start_service( const void* context );

/**
 * @brief Stop the Applicative Layer Clock synchronization service
 *
 * @param context Chip implementation context
 *
 * @return Operation status
 *
 * @see modem_e_alc_sync_start_service, modem_e_alc_sync_get_time, modem_e_alc_sync_trig_request
 */
modem_e_response_code_t modem_e_alc_sync_stop_service( const void* context );

/**
 * @brief Get the current time of the chip since GPS epoch
 *
 * This time is maintained by the chip after successful reception of an ALC sync downlink.
 * It is expressed as number of seconds elapsed since January 6th 1980 modulo 2^32.
 *
 * @param context Chip implementation context
 * @param alc_sync_epoch_time The current GPS time
 *
 * @return Operation status
 *
 * @see modem_e_alc_sync_start_service, modem_e_alc_sync_stop_service, modem_e_alc_sync_trig_request
 */
modem_e_response_code_t modem_e_alc_sync_get_time( const void* context, uint32_t* alc_sync_epoch_time );

/**
 * @brief Trig an Applicative Layer Clock synchronization time request
 *
 * @param context Chip implementation context
 *
 * @return Operation status
 *
 * @see modem_e_alc_sync_start_service, modem_e_alc_sync_stop_service, modem_e_alc_sync_get_time
 */
modem_e_response_code_t modem_e_alc_sync_trig_request( const void* context );

/**
 * @brief Get the FUOTA received file size and CRC
 *
 * This command must be called after @ref MODEM_E_LORAWAN_EVENT_FUOTA_DONE event.
 *
 * @param context Chip implementation context
 * @param file_size Size of the received file in bytes
 * @param file_crc CRC of the received file
 *
 * @returns Operation status
 */
modem_e_response_code_t modem_e_fuota_get_file_size_crc( const void* context, uint32_t* file_size, uint32_t* file_crc );

/**
 * @brief Read a FUOTA file fragment
 *
 * The maximal FUOTA file size the chip can store is 32768 bytes. And the maximal number of data that can be read in a
 * single SPI transaction is 350 bytes.
 * Therefore this function may be called several times to fetch a complete FUOTA file received.
 *
 * The complete size of the FUOTA file to read is obtained by calling @ref modem_e_fuota_get_file_size_crc.
 *
 * @param [in] context Chip implementation context
 * @param [in] base_address The base address to read fragment from. Valid values in [0:32768]
 * @param [in] fragment_size The size of the fragment to read. Valid values in [0:350]
 * @param [out] fragment A memory buffer to receive the fragment to read. It is up to the caller that it can store at
 * least @p fragment_size bytes
 *
 * @return Operation status
 *
 * @see modem_e_fuota_get_file_size_crc
 */
modem_e_response_code_t modem_e_fuota_read_file_fragment( const void* context, uint32_t base_address,
                                                          uint16_t fragment_size, uint8_t* fragment );

/**
 * @brief Helper function to check the CRC of a file
 *
 * This function does not send any command to the chip.
 * It is a helper function that check the CRC of a complete received file matches the expected value.
 * The complete received file is obtained through successive @ref modem_e_fuota_read_file_fragment calls, and the
 * expected CRC is obtained by calling @ref modem_e_fuota_get_file_size_crc.
 *
 * @param file Pointer to the buffer of the complete FUOTA file. It is up to the caller to ensure it is @p file_size
 * byte long
 * @param file_size The number of bytes of the complete FUOTA file
 * @param expected_crc The expected CRC of the file
 * @return true The CRC computed on the file content matches the expected CRC
 * @return false The CRC computed on the file content does not match the expected CRC
 */
bool modem_e_fuota_check_crc( const uint8_t* file, uint32_t file_size, uint32_t expected_crc );

/**
 * @brief Set the user configurable data rate backoff threshold
 *
 * The user data rate backoff threshold corresponds to the number of consecutive uplinks transmitted without any
 * downlink from the network. If this threshold is reached, the event @ref MODEM_E_LORAWAN_EVENT_DR_BACKOFF_LIMIT is
 * raised.
 * It different from the stack data rate backoff threshold configured through @ref modem_e_set_adr_ack_limit_delay.
 * The default value is 2400.
 *
 * @param[in] context Chip implementation context
 * @param[in] user_adr_ack_limit The user limit to configure
 *
 * @return Operation status
 *
 * @see modem_e_get_user_dr_backoff_sequence_threshold, modem_e_set_adr_ack_limit_delay
 */
modem_e_response_code_t modem_e_set_user_dr_backoff_sequence_threshold( const void* context,
                                                                        uint16_t    user_adr_ack_limit );

/**
 * @brief Get the user configurable data rate backoff threshold
 *
 * It returns either the default value, or the latest configured value of user data rate backoff threshold with @ref
 * modem_e_set_user_dr_backoff_sequence_threshold.
 * This is different from the stack data rate backoff threshold.
 *
 * @param[in] context Chip implementation context
 * @param [out] user_adr_ack_limit The user configurable data rate backoff threshold
 *
 * @return Operation status
 *
 * @see modem_e_set_user_dr_backoff_sequence_threshold
 */
modem_e_response_code_t modem_e_get_user_dr_backoff_sequence_threshold( const void* context,
                                                                        uint16_t*   user_adr_ack_limit );

/**
 * @brief Connect LoRaWAN network through Activation By Personalization (ABP)
 *
 * @note This ABP implementation does not store automatically the frame counters as requested by the LoRaWAN
 * specification L2 1.0.4. Therefore it is up to the caller to store/restore context with functions @ref
 * modem_e_store_state_snapshot_to_nvm and @ref modem_e_restore_state_snapshot_from_nvm when appropriate to ensure
 * compliance.
 *
 * @param [in] context Chip implementation context
 * @param [in] dev_addr ABP device address
 * @param [in] nwk_skey ABP network session key
 * @param [in] app_skey ABP application session key
 *
 * @return Operation status
 *
 * @see modem_e_join, modem_e_store_state_snapshot_to_nvm, modem_e_restore_state_snapshot_from_nvm
 */
modem_e_response_code_t modem_e_connect_with_abp( const void* context, const modem_e_abp_dev_addr_t dev_addr,
                                                  const modem_e_abp_nkw_skey_t nwk_skey,
                                                  const modem_e_abp_app_skey_t app_skey );

/**
 * @brief Get channel mask
 *
 * The channel mask returned here is a 12 bytes long bit mask.
 * Refer to LoRaWAN Regional Parameters document for the description of the channel mask
 *
 * @param context Chip implementation context
 * @param channel_mask 12 bytes long channel mask
 * @return Operation status
 */
modem_e_response_code_t modem_e_connect_get_channel_mask( const void* context, modem_e_channel_mask_t channel_mask );

/**
 * @brief Configure the channel mask
 *
 * Configure the channel mask.
 * Refer to the LoRaWAN Regional Parameters for the description of channel mask configuration.
 *
 * @param context Chip implementation context
 * @param channel_configuration Array of channel mask configurations. It is up to the caller to ensure it contains at
 * least @p n_channel_configurations elements.
 * @param n_channel_configurations Number of channel mask configuration to set. Valid values are in [1,6] included.
 * @return Operation status
 */
modem_e_response_code_t modem_e_connect_set_channel_mask(
    const void* context, const modem_e_channel_mask_configuration_t* channel_configuration,
    uint8_t n_channel_configurations );

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_LORAWAN_H

/* --- EOF ------------------------------------------------------------------ */
