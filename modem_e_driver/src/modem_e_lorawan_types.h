/*!
 * @file      modem_e_lorawan_types.h
 *
 * @brief     LoRaWAN driver types for Modem-E
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

#ifndef MODEM_E_LORAWAN_TYPES_H
#define MODEM_E_LORAWAN_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*!
 * @brief Length in bytes of a LoRaWAN device eui
 */
#define MODEM_E_OTAA_DEV_EUI_LENGTH ( 8 )

/*!
 * @brief Length in bytes of a LoRaWAN join eui
 */
#define MODEM_E_OTAA_JOIN_EUI_LENGTH ( 8 )

/*!
 * @brief Length in bytes of a LoRaWAN application key
 */
#define MODEM_E_OTAA_APP_KEY_LENGTH ( 16 )

/*!
 * @brief Length in bytes of a LoRaWAN network key
 */
#define MODEM_E_OTAA_NWK_KEY_LENGTH ( 16 )

/*!
 * @brief Length of datarate distribution array
 */
#define MODEM_E_DATARATE_DISTRIBUTION_LENGTH ( 16 )

/**
 * @brief Length in byte of ABP device address
 */
#define MODEM_E_ABP_DEV_ADDR_LEGNTH ( 4 )

/**
 * @brief Length in byte of ABP session keys
 */
#define MODEM_E_ABP_SKEYS_LEGNTH ( 16 )

/**
 * @brief Length in byte of channel mask
 */
#define MODEM_CHANNEL_MASK_LENGTH ( 12 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief LoRaWAN class type
 */
typedef enum
{
    MODEM_E_LORAWAN_CLASS_A = 0x00,
    MODEM_E_LORAWAN_CLASS_B = 0x01,
    MODEM_E_LORAWAN_CLASS_C = 0x02,
} modem_e_classes_t;

/**
 * @brief LoRaWAN network type
 */
typedef enum
{
    MODEM_E_LORAWAN_PRIVATE_NETWORK = 0x00,  //!< LoRaWAN private network
    MODEM_E_LORAWAN_PUBLIC_NETWORK  = 0x01   //!< LoRaWAN public network
} modem_e_network_type_t;

/*!
 * @brief LoRaWAN region type
 */
typedef enum
{
    MODEM_E_LORAWAN_REGION_EU868        = 0x01,
    MODEM_E_LORAWAN_REGION_AS923_GRP1   = 0x02,
    MODEM_E_LORAWAN_REGION_US915        = 0x03,
    MODEM_E_LORAWAN_REGION_AU915        = 0x04,
    MODEM_E_LORAWAN_REGION_CN470        = 0x05,
    MODEM_E_LORAWAN_REGION_WW2G4        = 0x06,
    MODEM_E_LORAWAN_REGION_AS923_GRP2   = 0x07,
    MODEM_E_LORAWAN_REGION_AS923_GRP3   = 0x08,
    MODEM_E_LORAWAN_REGION_IN865        = 0x09,
    MODEM_E_LORAWAN_REGION_KR920        = 0x0A,
    MODEM_E_LORAWAN_REGION_RU864        = 0x0B,
    MODEM_E_LORAWAN_REGION_CN470_RP_1_0 = 0x0C,
    MODEM_E_LORAWAN_REGION_AS923_GRP4   = 0x0D,
} modem_e_regions_t;

/*!
 * @brief Adaptative Data Rate profiles type
 */
typedef enum
{
    MODEM_E_ADR_PROFILE_NETWORK_SERVER_CONTROLLED = 0x00,  //!< Network Server Controlled
    MODEM_E_ADR_PROFILE_MOBILE_LONG_RANGE = 0x01,  //!< Mobile Long Range : 50% MinDr, 25% MinDr + 1, 25% MinDr + 2
    MODEM_E_ADR_PROFILE_MOBILE_LOW_POWER =
        0x02,                           //!< Mobile Low Power : 25% MaxDr, 25% MaxDr - 1, 25% MaxDr - 2, 25% MaxDr - 3
    MODEM_E_ADR_PROFILE_CUSTOM = 0x03,  //!< Custom List A custom ADR profile consists of a list of 16 preferred data
                                        //!< rates. For every transmission, a random entry in that list is selected.
} modem_e_adr_profiles_t;

/*!
 * @brief LoRaWAN uplink type
 */
typedef enum
{
    MODEM_E_UPLINK_UNCONFIRMED = 0x00,
    MODEM_E_UPLINK_CONFIRMED   = 0x01,
} modem_e_uplink_type_t;

/*!
 * @brief LoRaWAN Duty Cycle activation type
 */
typedef enum
{
    MODEM_E_CERTIFICATION_MODE_DISABLE = 0x00,
    MODEM_E_CERTIFICATION_MODE_ENABLE  = 0x01,
} modem_e_certification_mode_t;

/*!
 * @brief Listen Before Talk (LBT) activation type
 */
typedef enum
{
    MODEM_E_LBT_MODE_DISABLE = 0x00,
    MODEM_E_LBT_MODE_ENABLE  = 0x01,
} modem_e_lbt_mode_t;

/*!
 * @brief Carrier Sense Multiple Access (CSMA) activation type
 */
typedef enum
{
    MODEM_E_CSMA_MODE_DISABLE = 0x00,
    MODEM_E_CSMA_MODE_ENABLE  = 0x01,
} modem_e_csma_mode_t;

/**
 * @brief Carrier Sense Multiple Access (CSMA) backoff activation type
 */
typedef enum
{
    MODEM_E_CSMA_BACKOFF_DISABLE = 0x00,  //!< Disable CMSA backoff
    MODEM_E_CSMA_BACKOFF_ENABLE  = 0x01,  //!< Enable CMSA backoff
} modem_e_csma_backoff_mode_t;

/*!
 * @brief LoRaWAN mac request field
 */
typedef enum
{
    MODEM_E_MAC_REQUEST_LINK_CHECK     = 0x01,  //!< Enable the MAC Link Check request
    MODEM_E_MAC_REQUEST_TIME           = 0x02,  //!< Enable the MAC Time request
    MODEM_E_MAC_REQUEST_PING_SLOT_INFO = 0x04,  //!< Enable the MAC Ping Slot Info request
} modem_e_mac_request_t;

/**
 * @brief Bit mask for modem_e_mac_request_t
 *
 * @see modem_e_mac_request_t
 */
typedef uint8_t modem_e_mac_request_bitmask_t;

/*!
 * @brief RX flags encoding
 */
typedef enum
{
    MODEM_E_DOWNLINK_WINDOW_RX1                  = 0x01,  //!< received on RX1 unicast
    MODEM_E_DOWNLINK_WINDOW_RX2                  = 0x02,  //!< received on RX2 unicast
    MODEM_E_DOWNLINK_WINDOW_RXC                  = 0x03,  //!< received on Class C RX unicast
    MODEM_E_DOWNLINK_WINDOW_RXC_MULTICAST_GROUP0 = 0x04,  //!< received on Class C Multicast RX for group 0
    MODEM_E_DOWNLINK_WINDOW_RXC_MULTICAST_GROUP1 = 0x05,  //!< received on Class C Multicast RX for group 1
    MODEM_E_DOWNLINK_WINDOW_RXC_MULTICAST_GROUP2 = 0x06,  //!< received on Class C Multicast RX for group 2
    MODEM_E_DOWNLINK_WINDOW_RXC_MULTICAST_GROUP3 = 0x07,  //!< received on Class C Multicast RX for group 3
    MODEM_E_DOWNLINK_WINDOW_RXB                  = 0x08,  //!< received on Class B RX unicast
    MODEM_E_DOWNLINK_WINDOW_RXB_MULTICAST_GROUP0 = 0x09,  //!< received on Class B Multicast RX for group 0
    MODEM_E_DOWNLINK_WINDOW_RXB_MULTICAST_GROUP1 = 0x0A,  //!< received on Class B Multicast RX for group 1
    MODEM_E_DOWNLINK_WINDOW_RXB_MULTICAST_GROUP2 = 0x0B,  //!< received on Class B Multicast RX for group 2
    MODEM_E_DOWNLINK_WINDOW_RXB_MULTICAST_GROUP3 = 0x0C,  //!< received on Class B Multicast RX for group 3
    MODEM_E_DOWNLINK_WINDOW_RXBEACON             = 0x0D,  //!< received a Class B beacon
    MODEM_E_DOWNLINK_WINDOW_RXRELAY              = 0x0E,  //!< received on Relay window
} modem_e_downlink_window_t;

/*!
 * @brief class b ping slot status
 */
typedef enum
{
    MODEM_E_CLASS_B_PING_SLOT_1_S   = 0x00,  //!< 1 second ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_2_S   = 0x01,  //!< 2 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_4_S   = 0x02,  //!< 4 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_8_S   = 0x03,  //!< 8 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_16_S  = 0x04,  //!< 16 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_32_S  = 0x05,  //!< 32 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_64_S  = 0x06,  //!< 64 seconds ping-slot periodicity
    MODEM_E_CLASS_B_PING_SLOT_128_S = 0x07,  //!< 128 seconds ping-slot periodicity
} modem_e_class_b_ping_slot_t;

/*!
 * @brief Select the source of battery level value to use for DevStatusAns MAC command
 *
 */
typedef enum
{
    MODEM_E_BATTERY_LEVEL_FROM_INTERNAL_VALUE =
        0x00,  //!< Battery level is obtained from Modem-E VBat (see @ref modem_e_system_get_vbat)
    MODEM_E_BATTERY_LEVEL_FROM_USER_VALUE = 0x01,  //!< Battery level is provided by user application
} modem_e_battery_level_source_value_t;

/*!
 * @brief Join EUI type
 */
typedef uint8_t modem_e_otaa_join_eui_t[MODEM_E_OTAA_JOIN_EUI_LENGTH];

/*!
 * @brief Device EUI type
 */
typedef uint8_t modem_e_otaa_dev_eui_t[MODEM_E_OTAA_DEV_EUI_LENGTH];

/*!
 * @brief Application key type
 */
typedef uint8_t modem_e_otaa_app_key_t[MODEM_E_OTAA_APP_KEY_LENGTH];

/*!
 * @brief Application key type
 */
typedef uint8_t modem_e_otaa_nwk_key_t[MODEM_E_OTAA_NWK_KEY_LENGTH];

/*!
 * @brief ABP device address type
 */
typedef uint8_t modem_e_abp_dev_addr_t[MODEM_E_ABP_DEV_ADDR_LEGNTH];

/*!
 * @brief ABP network session key type
 */
typedef uint8_t modem_e_abp_nkw_skey_t[MODEM_E_ABP_SKEYS_LEGNTH];

/*!
 * @brief ABP application session key type
 */
typedef uint8_t modem_e_abp_app_skey_t[MODEM_E_ABP_SKEYS_LEGNTH];

/**
 * @brief Channel mask type
 */
typedef uint8_t modem_e_channel_mask_t[MODEM_CHANNEL_MASK_LENGTH];

/**
 * @brief Channel mask control type
 *
 * Refer to LoRaWAN Regional Parameters for values meaning.
 *
 */
typedef uint8_t channel_mask_control_t;

/*!
 * @brief modem downlink metadata structure
 *
 * The Signal to Noise Ratio (SNR) is returned as an integer part, and a decimal part in 0.25 dB.
 * The SNR in dB is therefore obtained by: \f$ SNR_{dB} = snr\_integer + 0.25 \times snr\_quarter \f$
 */
typedef struct
{
    uint8_t                   stack_id;      //!< The stack identifier that receives the downlink
    int16_t                   rssi;          //!< The RSSI of the received downlink in dBm
    int8_t                    snr_integer;   //!< Signal to Noise Ratio of the received downlink (integer part, in dB)
    uint8_t                   snr_quarter;   //!< Signal to Noise Ratio of the received downlink (0.25dB counts)
    modem_e_downlink_window_t window;        //!< Rx window of the received downlink
    uint8_t                   fport;         //!< LoRaWAN port of the received downlink
    uint8_t                   fpending_bit;  //!< Frame pending bit of the received downlink
    uint32_t                  frequency_hz;  //!< RF frequency of the received downlink
    uint8_t                   datarate;      //!< Datarate of the received downlink
} modem_e_downlink_metadata_t;

/*!
 * @brief multicast class c status structure
 */
typedef struct
{
    uint8_t  is_session_started;  //!< Indicate if the multicast class C session is started
    uint32_t downlink_frequency;  //!< Downlink frequency of the multicast class C session
    uint8_t  downlink_datarate;   //!< Datarate of the multicast class C session
} modem_e_multicast_class_c_status_t;

/*!
 * @brief multicast class b status structure
 */
typedef struct
{
    uint8_t  is_session_started;             //!< Indicate if the multicast class B session is started
    uint32_t downlink_frequency;             //!< Downlink frequency of the multicast class B session
    uint8_t  downlink_datarate;              //!< Datarate of the multicast class B session
    uint8_t  is_session_waiting_for_beacon;  //!< Indicates whether the multicast class B session have received a beacon
    uint8_t  ping_slot_periodicity;          //!< Ping slot periodicity of the multicast class B session
} modem_e_multicast_class_b_status_t;

/*!
 * @brief Modem-E LoRaWAN version structure
 */
typedef struct
{
    uint8_t lorawan_major;     //!< Major number of the LoRaWAN standard implemented
    uint8_t lorawan_minor;     //!< Minor number of the LoRaWAN standard implemented
    uint8_t lorawan_patch;     //!< Patch number of the LoRaWAN standard implemented
    uint8_t lorawan_revision;  //!< Revision number of the LoRaWAN standard implemented
    uint8_t rp_major;          //!< Major number of the regional parameters implemented
    uint8_t rp_minor;          //!< Minor number of the regional parameters implemented
    uint8_t rp_patch;          //!< Patch number of the regional parameters implemented
    uint8_t rp_revision;       //!< Revision number of the regional parameters implemented
} modem_e_lorawan_version_t;

/**
 * @brief Channel mask configuration structure
 *
 * Refer to LoRaWAN Regional Parameters for details.
 *
 */
typedef struct modem_e_channel_mask_configuration_s
{
    channel_mask_control_t channel_mask_control;
    uint8_t                channel_mask[2];
} modem_e_channel_mask_configuration_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_LORAWAN_TYPES_H

/* --- EOF ------------------------------------------------------------------ */
