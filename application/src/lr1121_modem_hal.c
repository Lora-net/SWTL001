/*!
 * @file      lr1121_modem_hal.c
 *
 * @brief     Hardware Abstraction Layer (HAL) implementation for lr1121
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2024. All rights reserved.
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

#include <stdlib.h>
#include <stdint.h>
#include "lr1121_hal.h"
#include "lr1121_modem_hal.h"
#include "lr1121_modem_system.h"
#include "system.h"
#include "system_time.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define LR1121_MODEM_RESET_TIMEOUT 3000

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

/*!
 * @brief Function to wait that the lr1121 transceiver busy line raise to high
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1121_hal_status_t
 */
static lr1121_hal_status_t lr1121_hal_wait_on_busy( const void* context, uint32_t timeout_ms );

/*!
 * @brief Function to wait that the lr1121 modem-e busy line fall to low
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1121_hal_status_t
 */
static lr1121_modem_hal_status_t lr1121_modem_hal_wait_on_busy( const void* context, uint32_t timeout_ms );

/*!
 * @brief Function to wait the that lr1121 modem-e busy line raise to high
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1121_hal_status_t
 */
static lr1121_modem_hal_status_t lr1121_modem_hal_wait_on_unbusy( const void* context, uint32_t timeout_ms );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/*!
 * @brief lr1121_modem_hal.h API implementation
 */

lr1121_modem_hal_status_t lr1121_modem_hal_write( const void* context, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{
    if( lr1121_modem_hal_wakeup( context ) == LR1121_MODEM_HAL_STATUS_OK )
    {
        radio_t*                  radio_local  = ( radio_t* ) context;
        uint8_t                   crc          = 0;
        uint8_t                   crc_received = 0;
        lr1121_modem_hal_status_t status;

        /* NSS low */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        /* Send CMD */
        system_spi_write( radio_local->spi, command, command_length );
        /* Send Data */
        system_spi_write( radio_local->spi, data, data_length );
        /* Compute and send CRC */
        crc = lr1121_modem_compute_crc( 0xFF, command, command_length );
        crc = lr1121_modem_compute_crc( crc, data, data_length );
        /* Send CRC */
        system_spi_write( radio_local->spi, &crc, 1 );

        /* NSS high */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        /* Wait on busy pin up to 1000 ms */
        if( lr1121_modem_hal_wait_on_busy( context, 1000 ) != LR1121_MODEM_HAL_STATUS_OK )
        {
            return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
        }

        /* Send dummy byte to retrieve RC & CRC */

        /* NSS low */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        /* read RC */
        system_spi_read_with_dummy_byte( radio_local->spi, ( uint8_t* ) &status, 1, 0x00 );
        system_spi_read_with_dummy_byte( radio_local->spi, ( uint8_t* ) &crc_received, 1, 0x00 );
        /* Compute response crc */
        crc = lr1121_modem_compute_crc( 0xFF, ( uint8_t* ) &status, 1 );

        /* NSS high */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        if( crc != crc_received )
        {
            /* change the response code */
            status = LR1121_MODEM_HAL_STATUS_BAD_FRAME;
        }

        /* Wait on busy pin up to 1000 ms */
        if( lr1121_modem_hal_wait_on_unbusy( context, 1000 ) != LR1121_MODEM_HAL_STATUS_OK )
        {
            return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
        }

        return status;
    }

    return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
}

lr1121_modem_hal_status_t lr1121_modem_hal_write_without_rc( const void* context, const uint8_t* command,
                                                             const uint16_t command_length, const uint8_t* data,
                                                             const uint16_t data_length )
{
    if( lr1121_modem_hal_wakeup( context ) == LR1121_MODEM_HAL_STATUS_OK )
    {
        radio_t*                  radio_local = ( radio_t* ) context;
        uint8_t                   crc         = 0;
        lr1121_modem_hal_status_t status      = LR1121_MODEM_HAL_STATUS_OK;

        /* NSS low */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        /* Send CMD */
        system_spi_write( radio_local->spi, command, command_length );
        /* Send Data */
        system_spi_write( radio_local->spi, data, data_length );
        /* Compute and send CRC */
        crc = lr1121_modem_compute_crc( 0xFF, command, command_length );
        crc = lr1121_modem_compute_crc( crc, data, data_length );
        /* Send CRC */
        system_spi_write( radio_local->spi, &crc, 1 );

        /* NSS high */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        return status;
    }

    return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
}

lr1121_modem_hal_status_t lr1121_modem_hal_read( const void* context, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
    if( lr1121_modem_hal_wakeup( context ) == LR1121_MODEM_HAL_STATUS_OK )
    {
        radio_t*                  radio_local  = ( radio_t* ) context;
        uint8_t                   crc          = 0;
        uint8_t                   crc_received = 0;
        lr1121_modem_hal_status_t status;

        /* NSS low */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        /* Send CMD */
        system_spi_write( radio_local->spi, command, command_length );
        /* Compute and send CRC */
        crc = lr1121_modem_compute_crc( 0xFF, command, command_length );
        /* Send CRC */
        system_spi_write( radio_local->spi, &crc, 1 );

        /* NSS high */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        /* Wait on busy pin up to 1000 ms */
        if( lr1121_modem_hal_wait_on_busy( context, 1000 ) != LR1121_MODEM_HAL_STATUS_OK )
        {
            return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
        }

        /* Send dummy byte to retrieve RC & CRC */

        /* NSS low */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        /* read RC */
        system_spi_read_with_dummy_byte( radio_local->spi, ( uint8_t* ) &status, 1, 0x00 );

        if( status == LR1121_MODEM_HAL_STATUS_OK )
        {
            system_spi_read_with_dummy_byte( radio_local->spi, data, data_length, 0x00 );
        }

        system_spi_read_with_dummy_byte( radio_local->spi, ( uint8_t* ) &crc_received, 1, 0x00 );

        /* NSS high */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        /* Compute response crc */
        crc = lr1121_modem_compute_crc( 0xFF, ( uint8_t* ) &status, 1 );
        if( status == LR1121_MODEM_HAL_STATUS_OK )
        {
            crc = lr1121_modem_compute_crc( crc, data, data_length );
        }

        if( crc != crc_received )
        {
            /* change the response code */
            status = LR1121_MODEM_HAL_STATUS_BAD_FRAME;
        }

        /* Wait on busy pin up to 1000 ms */
        if( lr1121_modem_hal_wait_on_unbusy( context, 1000 ) != LR1121_MODEM_HAL_STATUS_OK )
        {
            return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
        }

        return status;
    }

    return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
}

lr1121_modem_hal_status_t lr1121_modem_hal_reset( const void* context )
{
    radio_t* radio_local = ( radio_t* ) context;

    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 1 );
    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1121_MODEM_HAL_STATUS_OK;
}

void lr1121_modem_hal_enter_dfu( const void* context )
{
}

lr1121_modem_hal_status_t lr1121_modem_hal_wakeup( const void* context )
{
    if( lr1121_modem_hal_wait_on_busy( context, 10000 ) == LR1121_MODEM_HAL_STATUS_OK )
    {
        radio_t* radio_local = ( radio_t* ) context;
        /* Wakeup radio */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
    }
    else
    {
        return LR1121_MODEM_HAL_STATUS_BUSY_TIMEOUT;
    }

    /* Wait on busy pin for 1000 ms */
    return lr1121_modem_hal_wait_on_unbusy( context, 1000 );
}

/*!
 * @brief Bootstrap bootloader and SPI bootloader API implementation
 */

lr1121_hal_status_t lr1121_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
    if( lr1121_hal_wakeup( context ) == LR1121_HAL_STATUS_OK )
    {
        radio_t* radio_local = ( radio_t* ) context;
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
        system_spi_write( radio_local->spi, command, command_length );
        system_spi_write( radio_local->spi, data, data_length );
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        return lr1121_hal_wait_on_busy( context, 5000 );
    }
    return LR1121_HAL_STATUS_ERROR;
}

lr1121_hal_status_t lr1121_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    if( lr1121_hal_wakeup( context ) == LR1121_HAL_STATUS_OK )
    {
        radio_t* radio_local = ( radio_t* ) context;
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        system_spi_write( radio_local->spi, command, command_length );

        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        if( lr1121_hal_wait_on_busy( context, 5000 ) != LR1121_HAL_STATUS_OK )
        {
            return LR1121_HAL_STATUS_ERROR;
        }

        /* Send dummy byte */
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );

        const uint8_t dummy_byte = 0;
        system_spi_write( radio_local->spi, &dummy_byte, 1 );
        system_spi_read_with_dummy_byte( radio_local->spi, data, data_length, 0x00 );
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

        return lr1121_hal_wait_on_busy( context, 5000 );
    }
    return LR1121_HAL_STATUS_ERROR;
}

lr1121_hal_status_t lr1121_hal_wakeup( const void* context )
{
    radio_t* radio_local = ( radio_t* ) context;
    /* Wakeup radio */
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    /* Wait on busy pin for 5000 ms */
    return lr1121_hal_wait_on_busy( context, 5000 );
}

lr1121_hal_status_t lr1121_hal_reset( const void* context )
{
    radio_t* radio_local = ( radio_t* ) context;
    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 1 );
    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1121_HAL_STATUS_OK;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static lr1121_hal_status_t lr1121_hal_wait_on_busy( const void* context, uint32_t timeout_ms )
{
    radio_t* radio_local = ( radio_t* ) context;
    uint32_t start       = system_time_GetTicker( );
    while( system_gpio_get_pin_state( radio_local->busy ) == SYSTEM_GPIO_PIN_STATE_HIGH )
    {
        if( ( int32_t )( system_time_GetTicker( ) - start ) > ( int32_t ) timeout_ms )
        {
            return LR1121_HAL_STATUS_ERROR;
        }
    }
    return LR1121_HAL_STATUS_OK;
}

static lr1121_modem_hal_status_t lr1121_modem_hal_wait_on_busy( const void* context, uint32_t timeout_ms )
{
    radio_t* radio_local = ( radio_t* ) context;
    uint32_t start       = system_time_GetTicker( );
    uint32_t current     = 0;
    while( system_gpio_get_pin_state( radio_local->busy ) == SYSTEM_GPIO_PIN_STATE_LOW )
    {
        current = system_time_GetTicker( );
        if( ( int32_t )( current - start ) > ( int32_t ) timeout_ms )
        {
            return LR1121_MODEM_HAL_STATUS_ERROR;
        }
    }
    return LR1121_MODEM_HAL_STATUS_OK;
}

static lr1121_modem_hal_status_t lr1121_modem_hal_wait_on_unbusy( const void* context, uint32_t timeout_ms )
{
    radio_t* radio_local = ( radio_t* ) context;
    uint32_t start       = system_time_GetTicker( );
    uint32_t current     = 0;
    while( system_gpio_get_pin_state( radio_local->busy ) == SYSTEM_GPIO_PIN_STATE_HIGH )
    {
        current = system_time_GetTicker( );
        if( ( int32_t )( current - start ) > ( int32_t ) timeout_ms )
        {
            return LR1121_MODEM_HAL_STATUS_ERROR;
        }
    }
    return LR1121_MODEM_HAL_STATUS_OK;
}

/* --- EOF ------------------------------------------------------------------ */
