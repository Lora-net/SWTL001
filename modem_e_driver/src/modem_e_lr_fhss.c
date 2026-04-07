/*!
 * @file      modem_e_lr_fhss.c
 *
 * @brief     LR_FHSS driver implementation
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

#include "modem_e_lr_fhss.h"
#include "modem_e_modem_hal.h"
#include "modem_e_radio.h"
#include "modem_e_radio_types.h"
#include "lr_fhss_v1_base_types.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define MODEM_E_LR_FHSS_BUILD_FRAME_LENGTH ( 2u + 9u )
#define MODEM_E_LR_FHSS_HEADER_BITS ( 114u )
#define MODEM_E_LR_FHSS_FRAG_BITS ( 48u )
#define MODEM_E_LR_FHSS_BLOCK_PREAMBLE_BITS ( 2u )
#define MODEM_E_LR_FHSS_BLOCK_BITS ( MODEM_E_LR_FHSS_FRAG_BITS + MODEM_E_LR_FHSS_BLOCK_PREAMBLE_BITS )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief Operating codes for radio-related operations
 */
enum
{
    MODEM_E_LR_FHSS_BUILD_FRAME_OC = 0x022C,
};

/*!
 * @brief Hopping enable/disabled enumerations for \ref modem_e_lr_fhss_build_frame
 */
typedef enum
{
    MODEM_E_LR_FHSS_HOPPING_DISABLE = 0x00,
    MODEM_E_LR_FHSS_HOPPING_ENABLE  = 0x01,
} modem_e_lr_fhss_hopping_configuration_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Get the bit count and block count for a LR-FHSS frame
 *
 * @param  [in] params         Parameter structure
 * @param  [in] payload_length Length of physical payload, in bytes
 *
 * @returns                    Length of physical payload, in bits
 */

static uint16_t modem_e_lr_fhss_get_nb_bits( const lr_fhss_v1_params_t* params, uint16_t payload_length );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

modem_e_response_code_t modem_e_lr_fhss_init( const void* context )
{
    const modem_e_response_code_t set_packet_type_status =
        modem_e_radio_set_pkt_type( context, MODEM_E_RADIO_PKT_TYPE_LR_FHSS );
    if( set_packet_type_status != MODEM_E_RESPONSE_CODE_OK )
    {
        return set_packet_type_status;
    }

    const modem_e_radio_mod_params_lr_fhss_t mod_lr_fhss = {
        .br_in_bps   = MODEM_E_RADIO_LR_FHSS_BITRATE_488_BPS,
        .pulse_shape = MODEM_E_RADIO_LR_FHSS_PULSE_SHAPE_BT_1,
    };

    const modem_e_response_code_t set_modulation_param_status =
        modem_e_radio_set_lr_fhss_mod_params( context, &mod_lr_fhss );
    return set_modulation_param_status;
}

uint16_t modem_e_lr_fhss_get_bit_delay_in_us( const modem_e_lr_fhss_params_t* params, uint16_t payload_length )
{
    const uint16_t nb_bits = modem_e_lr_fhss_get_nb_bits( &( params->lr_fhss_params ), payload_length );

    const unsigned int nb_padding_bits = 1u + ( ( 32768u - nb_bits ) & 0x07u );

    return ( uint16_t ) ( 1600u + nb_padding_bits * 2048u );
}

modem_e_response_code_t modem_e_lr_fhss_build_frame( const void*                     context,
                                                     const modem_e_lr_fhss_params_t* lr_fhss_params,
                                                     uint16_t hop_sequence_id, const uint8_t* payload,
                                                     uint8_t payload_length )
{
    // Since the build_frame command is last, it is possible to check status through stat1

    modem_e_response_code_t status =
        modem_e_radio_set_lr_fhss_sync_word( context, lr_fhss_params->lr_fhss_params.sync_word );
    if( status != MODEM_E_RESPONSE_CODE_OK )
    {
        return status;
    }

    const uint8_t cbuffer[MODEM_E_LR_FHSS_BUILD_FRAME_LENGTH] = {
        ( uint8_t ) ( MODEM_E_LR_FHSS_BUILD_FRAME_OC >> 8 ),
        ( uint8_t ) ( MODEM_E_LR_FHSS_BUILD_FRAME_OC >> 0 ),
        ( uint8_t ) lr_fhss_params->lr_fhss_params.header_count,
        ( uint8_t ) lr_fhss_params->lr_fhss_params.cr,
        ( uint8_t ) lr_fhss_params->lr_fhss_params.modulation_type,
        ( uint8_t ) lr_fhss_params->lr_fhss_params.grid,
        ( uint8_t ) ( lr_fhss_params->lr_fhss_params.enable_hopping ? MODEM_E_LR_FHSS_HOPPING_ENABLE
                                                                    : MODEM_E_LR_FHSS_HOPPING_DISABLE ),
        ( uint8_t ) lr_fhss_params->lr_fhss_params.bw,
        ( uint8_t ) ( hop_sequence_id >> 8 ),
        ( uint8_t ) ( hop_sequence_id >> 0 ),
        ( uint8_t ) lr_fhss_params->device_offset,
    };

    return ( modem_e_response_code_t ) modem_e_modem_hal_write( context, cbuffer, MODEM_E_LR_FHSS_BUILD_FRAME_LENGTH,
                                                                payload, payload_length );
}

uint32_t modem_e_lr_fhss_get_time_on_air_in_ms( const modem_e_lr_fhss_params_t* params, uint16_t payload_length )
{
    // Multiply by 1000 / 488.28125, or equivalently 256/125, rounding up
    return ( ( modem_e_lr_fhss_get_nb_bits( &params->lr_fhss_params, payload_length ) << 8u ) + 124u ) / 125u;
}

unsigned int modem_e_lr_fhss_get_hop_sequence_count( const modem_e_lr_fhss_params_t* lr_fhss_params )
{
    if( ( lr_fhss_params->lr_fhss_params.grid == LR_FHSS_V1_GRID_25391_HZ ) ||
        ( ( lr_fhss_params->lr_fhss_params.grid == LR_FHSS_V1_GRID_3906_HZ ) &&
          ( lr_fhss_params->lr_fhss_params.bw < LR_FHSS_V1_BW_335938_HZ ) ) )
    {
        return 384;
    }
    return 512;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION ---------------------------------------------
 */

uint16_t modem_e_lr_fhss_get_nb_bits( const lr_fhss_v1_params_t* params, uint16_t payload_length )
{
    unsigned int length_bits = ( payload_length + 2u ) * 8u + 6u;
    switch( params->cr )
    {
    case LR_FHSS_V1_CR_5_6:
        length_bits = ( ( length_bits * 6u ) + 4u ) / 5u;
        break;

    case LR_FHSS_V1_CR_2_3:
        length_bits = length_bits * 3u / 2u;
        break;

    case LR_FHSS_V1_CR_1_2:
        length_bits = length_bits * 2u;
        break;

    case LR_FHSS_V1_CR_1_3:
        length_bits = length_bits * 3u;
        break;

    default:
    {
        // Empty on purpose
    }
    }

    unsigned int payload_bits    = ( length_bits / MODEM_E_LR_FHSS_FRAG_BITS ) * MODEM_E_LR_FHSS_BLOCK_BITS;
    unsigned     last_block_bits = length_bits % MODEM_E_LR_FHSS_FRAG_BITS;
    if( last_block_bits > 0 )
    {
        payload_bits += last_block_bits + 2u;
    }

    return ( uint16_t ) ( MODEM_E_LR_FHSS_HEADER_BITS * params->header_count + payload_bits );
}
