/*!
 * @file      modem_e_system_types.h
 *
 * @brief     System driver types
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

#ifndef MODEM_E_SYSTEM_TYPES_H
#define MODEM_E_SYSTEM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*!
 * @brief Length of the Modem-E Unique Identifier in bytes
 *
 * The Modem-E Unique Identifiers is an 8 byte long buffer
 */
#define MODEM_E_SYSTEM_UID_LENGTH ( 8 )

/**
 * @brief Length of Join Unique Identifier in bytes
 */
#define MODEM_E_SYSTEM_JOIN_EUI_LENGTH ( 8 )

/**
 * @brief Length of PIN number in bytes
 */
#define MODEM_E_SYSTEM_PIN_LENGTH ( 4 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * @brief Fixed-length array to store a UID
 */
typedef uint8_t modem_e_system_uid_t[MODEM_E_SYSTEM_UID_LENGTH];

/**
 * @brief Fixed-length array to store a joinEUI
 */
typedef uint8_t modem_e_system_join_eui_t[MODEM_E_SYSTEM_JOIN_EUI_LENGTH];

/**
 * @brief Fixed-length array to store a PIN
 */
typedef uint8_t modem_e_system_pin_t[MODEM_E_SYSTEM_PIN_LENGTH];

/**
 * @brief Type to store system interrupt flags
 */
typedef uint32_t modem_e_system_irq_mask_t;

/**
 * @brief Interrupt flags
 */
enum modem_e_system_irq_e
{
    MODEM_E_SYSTEM_IRQ_NONE                   = ( 0 << 0 ),
    MODEM_E_SYSTEM_IRQ_TX_DONE                = ( 1 << 2 ),
    MODEM_E_SYSTEM_IRQ_RX_DONE                = ( 1 << 3 ),
    MODEM_E_SYSTEM_IRQ_PREAMBLE_DETECTED      = ( 1 << 4 ),
    MODEM_E_SYSTEM_IRQ_SYNC_WORD_HEADER_VALID = ( 1 << 5 ),
    MODEM_E_SYSTEM_IRQ_HEADER_ERROR           = ( 1 << 6 ),
    MODEM_E_SYSTEM_IRQ_CRC_ERROR              = ( 1 << 7 ),
    MODEM_E_SYSTEM_IRQ_CAD_DONE               = ( 1 << 8 ),
    MODEM_E_SYSTEM_IRQ_CAD_DETECTED           = ( 1 << 9 ),
    MODEM_E_SYSTEM_IRQ_TIMEOUT                = ( 1 << 10 ),
    MODEM_E_SYSTEM_IRQ_LR_FHSS_INTRA_PKT_HOP  = ( 1 << 11 ),
    MODEM_E_SYSTEM_IRQ_EOL                    = ( 1 << 21 ),
    MODEM_E_SYSTEM_IRQ_CMD_ERROR              = ( 1 << 22 ),
    MODEM_E_SYSTEM_IRQ_ERROR                  = ( 1 << 23 ),
    MODEM_E_SYSTEM_IRQ_FSK_LEN_ERROR          = ( 1 << 24 ),
    MODEM_E_SYSTEM_IRQ_FSK_ADDR_ERROR         = ( 1 << 25 ),
    MODEM_E_SYSTEM_IRQ_LORA_RX_TIMESTAMP      = ( 1 << 27 ),
    MODEM_E_SYSTEM_IRQ_ALL_MASK =
        MODEM_E_SYSTEM_IRQ_TX_DONE | MODEM_E_SYSTEM_IRQ_RX_DONE | MODEM_E_SYSTEM_IRQ_PREAMBLE_DETECTED |
        MODEM_E_SYSTEM_IRQ_SYNC_WORD_HEADER_VALID | MODEM_E_SYSTEM_IRQ_HEADER_ERROR | MODEM_E_SYSTEM_IRQ_CRC_ERROR |
        MODEM_E_SYSTEM_IRQ_CAD_DONE | MODEM_E_SYSTEM_IRQ_CAD_DETECTED | MODEM_E_SYSTEM_IRQ_TIMEOUT |
        MODEM_E_SYSTEM_IRQ_LR_FHSS_INTRA_PKT_HOP | MODEM_E_SYSTEM_IRQ_EOL | MODEM_E_SYSTEM_IRQ_CMD_ERROR |
        MODEM_E_SYSTEM_IRQ_ERROR | MODEM_E_SYSTEM_IRQ_FSK_LEN_ERROR | MODEM_E_SYSTEM_IRQ_FSK_ADDR_ERROR |
        MODEM_E_SYSTEM_IRQ_LORA_RX_TIMESTAMP,
};

/**
 * @brief Calibration flags
 */
enum modem_e_system_calibration_e
{
    MODEM_E_SYSTEM_CALIB_LF_RC_MASK  = ( 1 << 0 ),
    MODEM_E_SYSTEM_CALIB_HF_RC_MASK  = ( 1 << 1 ),
    MODEM_E_SYSTEM_CALIB_PLL_MASK    = ( 1 << 2 ),
    MODEM_E_SYSTEM_CALIB_ADC_MASK    = ( 1 << 3 ),
    MODEM_E_SYSTEM_CALIB_IMG_MASK    = ( 1 << 4 ),
    MODEM_E_SYSTEM_CALIB_PLL_TX_MASK = ( 1 << 5 ),
};

/**
 * @brief Type for calibration mask
 *
 * @see modem_e_system_calibration_e
 */
typedef uint8_t modem_e_system_cal_mask_t;

/**
 * @brief Error flags
 */
enum modem_e_system_errors_e
{
    MODEM_E_SYSTEM_ERRORS_LF_RC_CALIB_MASK   = ( 1 << 0 ),
    MODEM_E_SYSTEM_ERRORS_HF_RC_CALIB_MASK   = ( 1 << 1 ),
    MODEM_E_SYSTEM_ERRORS_ADC_CALIB_MASK     = ( 1 << 2 ),
    MODEM_E_SYSTEM_ERRORS_PLL_CALIB_MASK     = ( 1 << 3 ),
    MODEM_E_SYSTEM_ERRORS_IMG_CALIB_MASK     = ( 1 << 4 ),
    MODEM_E_SYSTEM_ERRORS_HF_XOSC_START_MASK = ( 1 << 5 ),
    MODEM_E_SYSTEM_ERRORS_LF_XOSC_START_MASK = ( 1 << 6 ),
    MODEM_E_SYSTEM_ERRORS_PLL_LOCK_MASK      = ( 1 << 7 ),
};

/**
 * @brief Type for system errors mask
 *
 * @see modem_e_system_errors_e
 */
typedef uint16_t modem_e_system_errors_t;

/**
 * @brief Chip modes
 */
typedef enum
{
    MODEM_E_SYSTEM_CHIP_MODE_SLEEP     = 0x00,
    MODEM_E_SYSTEM_CHIP_MODE_STBY_RC   = 0x01,
    MODEM_E_SYSTEM_CHIP_MODE_STBY_XOSC = 0x02,
    MODEM_E_SYSTEM_CHIP_MODE_FS        = 0x03,
    MODEM_E_SYSTEM_CHIP_MODE_RX        = 0x04,
    MODEM_E_SYSTEM_CHIP_MODE_TX        = 0x05,
    MODEM_E_SYSTEM_CHIP_MODE_LOC       = 0x06,
} modem_e_system_chip_modes_t;

/**
 * @brief Reset status
 */
typedef enum
{
    MODEM_E_SYSTEM_RESET_STATUS_CLEARED      = 0x00,
    MODEM_E_SYSTEM_RESET_STATUS_ANALOG       = 0x01,
    MODEM_E_SYSTEM_RESET_STATUS_EXTERNAL     = 0x02,
    MODEM_E_SYSTEM_RESET_STATUS_SYSTEM       = 0x03,
    MODEM_E_SYSTEM_RESET_STATUS_WATCHDOG     = 0x04,
    MODEM_E_SYSTEM_RESET_STATUS_IOCD_RESTART = 0x05,
    MODEM_E_SYSTEM_RESET_STATUS_RTC_RESTART  = 0x06,
} modem_e_system_reset_status_t;

/**
 * @brief Command status
 */
typedef enum
{
    MODEM_E_SYSTEM_CMD_STATUS_FAIL = 0x00,
    MODEM_E_SYSTEM_CMD_STATUS_PERR = 0x01,
    MODEM_E_SYSTEM_CMD_STATUS_OK   = 0x02,
    MODEM_E_SYSTEM_CMD_STATUS_DATA = 0x03,
} modem_e_system_command_status_t;

/**
 * @brief Low-frequency clock modes
 */
typedef enum modem_e_system_lfclk_cfg_e
{
    MODEM_E_SYSTEM_LFCLK_RC   = 0x00,  //!<  (Default)
    MODEM_E_SYSTEM_LFCLK_XTAL = 0x01,
    MODEM_E_SYSTEM_LFCLK_EXT  = 0x02
} modem_e_system_lfclk_cfg_t;

/**
 * @brief Regulator modes
 */
typedef enum
{
    MODEM_E_SYSTEM_REG_MODE_LDO  = 0x00,  //!< (Default)
    MODEM_E_SYSTEM_REG_MODE_DCDC = 0x01,
} modem_e_system_reg_mode_t;

/**
 * @brief Info page ID
 */
typedef enum
{
    MODEM_E_SYSTEM_INFOPAGE_0 = 0x00,  //!< Info page #0
    MODEM_E_SYSTEM_INFOPAGE_1 = 0x01,  //!< Info page #1
} modem_e_system_infopage_id_t;

/**
 * @brief RF switch configuration pin
 */
enum modem_e_system_rfswitch_cfg_pin_e
{
    MODEM_E_SYSTEM_RFSW0_HIGH = ( 1 << 0 ),
    MODEM_E_SYSTEM_RFSW1_HIGH = ( 1 << 1 ),
    MODEM_E_SYSTEM_RFSW2_HIGH = ( 1 << 2 ),
    MODEM_E_SYSTEM_RFSW3_HIGH = ( 1 << 3 ),
    MODEM_E_SYSTEM_RFSW4_HIGH = ( 1 << 4 ),
};

/**
 * @brief RF switch configuration structure definition
 */
typedef struct modem_e_system_rfswitch_cfg_s
{
    uint8_t enable;   //!< Bitmask for DIO to control as RF switches
    uint8_t standby;  //!< Bitmask for DIO state while chip is in standby mode
    uint8_t rx;       //!< Bitmask for DIO state while chip is in reception mode
    uint8_t tx;       //!< Bitmask for DIO state while chip is in transmission mode
    uint8_t tx_hp;    //!< Bitmask for DIO state while chip is in high power transmission mode
    uint8_t tx_hf;    //!< Bitmask for DIO state while chip is in high frequency transmission mode
} modem_e_system_rfswitch_cfg_t;

/**
 * @brief Stand by configuration values
 */
typedef enum
{
    MODEM_E_SYSTEM_STANDBY_CFG_RC   = 0x00,
    MODEM_E_SYSTEM_STANDBY_CFG_XOSC = 0x01
} modem_e_system_standby_cfg_t;

/**
 * @brief TCXO supply voltage values
 */
typedef enum
{
    MODEM_E_SYSTEM_TCXO_CTRL_1_6V = 0x00,  //!< Supply voltage = 1.6v
    MODEM_E_SYSTEM_TCXO_CTRL_1_7V = 0x01,  //!< Supply voltage = 1.7v
    MODEM_E_SYSTEM_TCXO_CTRL_1_8V = 0x02,  //!< Supply voltage = 1.8v
    MODEM_E_SYSTEM_TCXO_CTRL_2_2V = 0x03,  //!< Supply voltage = 2.2v
    MODEM_E_SYSTEM_TCXO_CTRL_2_4V = 0x04,  //!< Supply voltage = 2.4v
    MODEM_E_SYSTEM_TCXO_CTRL_2_7V = 0x05,  //!< Supply voltage = 2.7v
    MODEM_E_SYSTEM_TCXO_CTRL_3_0V = 0x06,  //!< Supply voltage = 3.0v
    MODEM_E_SYSTEM_TCXO_CTRL_3_3V = 0x07,  //!< Supply voltage = 3.3v
} modem_e_system_tcxo_supply_voltage_t;

/**
 * @brief Status register 1 structure definition
 */
typedef struct modem_e_system_stat1_s
{
    modem_e_system_command_status_t command_status;       //!< Status of last command
    bool                            is_interrupt_active;  //!< Indicates at least one interrupt is active
} modem_e_system_stat1_t;

/**
 * @brief Status register 2 structure definition
 */
typedef struct modem_e_system_stat2_s
{
    modem_e_system_reset_status_t reset_status;  //!< Source of reset
    modem_e_system_chip_modes_t   chip_mode;     //!< Current mode the chip is running
    bool is_running_from_flash;                  //!< Flag indicating if the chip is currently running from flash
} modem_e_system_stat2_t;

/**
 * @brief Chip type values
 */
typedef enum
{
    MODEM_E_SYSTEM_VERSION_TYPE_LR1110 = 0x01,
    MODEM_E_SYSTEM_VERSION_TYPE_LR1121 = 0x03,
} modem_e_system_version_type_t;

/**
 * @brief Version structure definition
 */
typedef struct modem_e_system_version_s
{
    uint8_t                       hw;    //!< Hardware field of system version
    modem_e_system_version_type_t type;  //!< Type field of system version
    uint16_t                      fw;    //!< Software field of system version
} modem_e_system_version_t;

/**
 * @brief Sleep configuration structure definition
 */
typedef struct modem_e_system_sleep_cfg_s
{
    bool is_warm_start;   //!< Keep configuration and state in retention memory, allowing warm start
    bool is_rtc_timeout;  //!< Enable automatic wake-up after a configurable duration
} modem_e_system_sleep_cfg_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MODEM_E_SYSTEM_TYPES_H

/* --- EOF ------------------------------------------------------------------ */
