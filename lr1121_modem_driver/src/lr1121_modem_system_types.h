/*!
 * @file      lr1121_modem_system_types.h
 *
 * @brief     System driver types for LR1121
 *
 * @copyright Copyright Semtech Corporation 2024. All rights reserved.
 *
 * @license{The Clear BSD License
 * @par
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
 * @par
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
 * POSSIBILITY OF SUCH DAMAGE.}
 */

#ifndef LR1121_MODEM_SYSTEM_TYPES_H
#define LR1121_MODEM_SYSTEM_TYPES_H

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
 * @brief Length of the LR1121 Unique Identifier in bytes
 *
 * The LR1121 Unique Identifiers is an 8 byte long buffer
 */
#define LR1121_MODEM_SYSTEM_UID_LENGTH ( 8 )

/**
 * @brief Length of Join Unique Identifier in bytes
 */
#define LR1121_MODEM_SYSTEM_JOIN_EUI_LENGTH ( 8 )

/**
 * @brief Length of PIN number in bytes
 */
#define LR1121_MODEM_SYSTEM_PIN_LENGTH ( 4 )

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
typedef uint8_t lr1121_modem_system_uid_t[LR1121_MODEM_SYSTEM_UID_LENGTH];

/**
 * @brief Fixed-length array to store a joinEUI
 */
typedef uint8_t lr1121_modem_system_join_eui_t[LR1121_MODEM_SYSTEM_JOIN_EUI_LENGTH];

/**
 * @brief Fixed-length array to store a PIN
 */
typedef uint8_t lr1121_modem_system_pin_t[LR1121_MODEM_SYSTEM_PIN_LENGTH];

/**
 * @brief Type to store system interrupt flags
 */
typedef uint32_t lr1121_modem_system_irq_mask_t;

/**
 * @brief Interrupt flags
 */
enum lr1121_modem_system_irq_e
{
    LR1121_MODEM_SYSTEM_IRQ_NONE                   = ( 0 << 0 ),
    LR1121_MODEM_SYSTEM_IRQ_TX_DONE                = ( 1 << 2 ),
    LR1121_MODEM_SYSTEM_IRQ_RX_DONE                = ( 1 << 3 ),
    LR1121_MODEM_SYSTEM_IRQ_PREAMBLE_DETECTED      = ( 1 << 4 ),
    LR1121_MODEM_SYSTEM_IRQ_SYNC_WORD_HEADER_VALID = ( 1 << 5 ),
    LR1121_MODEM_SYSTEM_IRQ_HEADER_ERROR           = ( 1 << 6 ),
    LR1121_MODEM_SYSTEM_IRQ_CRC_ERROR              = ( 1 << 7 ),
    LR1121_MODEM_SYSTEM_IRQ_CAD_DONE               = ( 1 << 8 ),
    LR1121_MODEM_SYSTEM_IRQ_CAD_DETECTED           = ( 1 << 9 ),
    LR1121_MODEM_SYSTEM_IRQ_TIMEOUT                = ( 1 << 10 ),
    LR1121_MODEM_SYSTEM_IRQ_LR_FHSS_INTRA_PKT_HOP  = ( 1 << 11 ),
    LR1121_MODEM_SYSTEM_IRQ_EOL                    = ( 1 << 21 ),
    LR1121_MODEM_SYSTEM_IRQ_CMD_ERROR              = ( 1 << 22 ),
    LR1121_MODEM_SYSTEM_IRQ_ERROR                  = ( 1 << 23 ),
    LR1121_MODEM_SYSTEM_IRQ_FSK_LEN_ERROR          = ( 1 << 24 ),
    LR1121_MODEM_SYSTEM_IRQ_FSK_ADDR_ERROR         = ( 1 << 25 ),
    LR1121_MODEM_SYSTEM_IRQ_LORA_RX_TIMESTAMP      = ( 1 << 27 ),
    LR1121_MODEM_SYSTEM_IRQ_ALL_MASK =
        LR1121_MODEM_SYSTEM_IRQ_TX_DONE | LR1121_MODEM_SYSTEM_IRQ_RX_DONE | LR1121_MODEM_SYSTEM_IRQ_PREAMBLE_DETECTED |
        LR1121_MODEM_SYSTEM_IRQ_SYNC_WORD_HEADER_VALID | LR1121_MODEM_SYSTEM_IRQ_HEADER_ERROR |
        LR1121_MODEM_SYSTEM_IRQ_CRC_ERROR | LR1121_MODEM_SYSTEM_IRQ_CAD_DONE | LR1121_MODEM_SYSTEM_IRQ_CAD_DETECTED |
        LR1121_MODEM_SYSTEM_IRQ_TIMEOUT | LR1121_MODEM_SYSTEM_IRQ_LR_FHSS_INTRA_PKT_HOP | LR1121_MODEM_SYSTEM_IRQ_EOL |
        LR1121_MODEM_SYSTEM_IRQ_CMD_ERROR | LR1121_MODEM_SYSTEM_IRQ_ERROR | LR1121_MODEM_SYSTEM_IRQ_FSK_LEN_ERROR |
        LR1121_MODEM_SYSTEM_IRQ_FSK_ADDR_ERROR | LR1121_MODEM_SYSTEM_IRQ_LORA_RX_TIMESTAMP,
};

/**
 * @brief Calibration flags
 */
enum lr1121_modem_system_calibration_e
{
    LR1121_MODEM_SYSTEM_CALIB_LF_RC_MASK  = ( 1 << 0 ),
    LR1121_MODEM_SYSTEM_CALIB_HF_RC_MASK  = ( 1 << 1 ),
    LR1121_MODEM_SYSTEM_CALIB_PLL_MASK    = ( 1 << 2 ),
    LR1121_MODEM_SYSTEM_CALIB_ADC_MASK    = ( 1 << 3 ),
    LR1121_MODEM_SYSTEM_CALIB_IMG_MASK    = ( 1 << 4 ),
    LR1121_MODEM_SYSTEM_CALIB_PLL_TX_MASK = ( 1 << 5 ),
};

/**
 * @brief Type for calibration mask
 *
 * @see lr1121_modem_system_calibration_e
 */
typedef uint8_t lr1121_modem_system_cal_mask_t;

/**
 * @brief Error flags
 */
enum lr1121_modem_system_errors_e
{
    LR1121_MODEM_SYSTEM_ERRORS_LF_RC_CALIB_MASK   = ( 1 << 0 ),
    LR1121_MODEM_SYSTEM_ERRORS_HF_RC_CALIB_MASK   = ( 1 << 1 ),
    LR1121_MODEM_SYSTEM_ERRORS_ADC_CALIB_MASK     = ( 1 << 2 ),
    LR1121_MODEM_SYSTEM_ERRORS_PLL_CALIB_MASK     = ( 1 << 3 ),
    LR1121_MODEM_SYSTEM_ERRORS_IMG_CALIB_MASK     = ( 1 << 4 ),
    LR1121_MODEM_SYSTEM_ERRORS_HF_XOSC_START_MASK = ( 1 << 5 ),
    LR1121_MODEM_SYSTEM_ERRORS_LF_XOSC_START_MASK = ( 1 << 6 ),
    LR1121_MODEM_SYSTEM_ERRORS_PLL_LOCK_MASK      = ( 1 << 7 ),
};

/**
 * @brief Type for system errors mask
 *
 * @see lr1121_modem_system_errors_e
 */
typedef uint16_t lr1121_modem_system_errors_t;

/**
 * @brief Chip modes
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_CHIP_MODE_SLEEP     = 0x00,
    LR1121_MODEM_SYSTEM_CHIP_MODE_STBY_RC   = 0x01,
    LR1121_MODEM_SYSTEM_CHIP_MODE_STBY_XOSC = 0x02,
    LR1121_MODEM_SYSTEM_CHIP_MODE_FS        = 0x03,
    LR1121_MODEM_SYSTEM_CHIP_MODE_RX        = 0x04,
    LR1121_MODEM_SYSTEM_CHIP_MODE_TX        = 0x05,
    LR1121_MODEM_SYSTEM_CHIP_MODE_LOC       = 0x06,
} lr1121_modem_system_chip_modes_t;

/**
 * @brief Reset status
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_RESET_STATUS_CLEARED      = 0x00,
    LR1121_MODEM_SYSTEM_RESET_STATUS_ANALOG       = 0x01,
    LR1121_MODEM_SYSTEM_RESET_STATUS_EXTERNAL     = 0x02,
    LR1121_MODEM_SYSTEM_RESET_STATUS_SYSTEM       = 0x03,
    LR1121_MODEM_SYSTEM_RESET_STATUS_WATCHDOG     = 0x04,
    LR1121_MODEM_SYSTEM_RESET_STATUS_IOCD_RESTART = 0x05,
    LR1121_MODEM_SYSTEM_RESET_STATUS_RTC_RESTART  = 0x06,
} lr1121_modem_system_reset_status_t;

/**
 * @brief Command status
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_CMD_STATUS_FAIL = 0x00,
    LR1121_MODEM_SYSTEM_CMD_STATUS_PERR = 0x01,
    LR1121_MODEM_SYSTEM_CMD_STATUS_OK   = 0x02,
    LR1121_MODEM_SYSTEM_CMD_STATUS_DATA = 0x03,
} lr1121_modem_system_command_status_t;

/**
 * @brief Low-frequency clock modes
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_LFCLK_RC   = 0x00,  //!<  (Default)
    LR1121_MODEM_SYSTEM_LFCLK_XTAL = 0x01,
    LR1121_MODEM_SYSTEM_LFCLK_EXT  = 0x02
} lr1121_modem_system_lfclk_cfg_t;

/**
 * @brief Regulator modes
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_REG_MODE_LDO  = 0x00,  //!< (Default)
    LR1121_MODEM_SYSTEM_REG_MODE_DCDC = 0x01,
} lr1121_modem_system_reg_mode_t;

/**
 * @brief Info page ID
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_INFOPAGE_0 = 0x00,  //!< Info page #0
    LR1121_MODEM_SYSTEM_INFOPAGE_1 = 0x01,  //!< Info page #1
} lr1121_modem_system_infopage_id_t;

/**
 * @brief RF switch configuration pin
 */
enum lr1121_modem_system_rfswitch_cfg_pin_e
{
    LR1121_MODEM_SYSTEM_RFSW0_HIGH = ( 1 << 0 ),
    LR1121_MODEM_SYSTEM_RFSW1_HIGH = ( 1 << 1 ),
    LR1121_MODEM_SYSTEM_RFSW2_HIGH = ( 1 << 2 ),
    LR1121_MODEM_SYSTEM_RFSW3_HIGH = ( 1 << 3 ),
    LR1121_MODEM_SYSTEM_RFSW4_HIGH = ( 1 << 4 ),
};

/**
 * @brief RF switch configuration structure definition
 */
typedef struct lr1121_modem_system_rfswitch_cfg_s
{
    uint8_t enable;   //!< Bitmask for DIO to control as RF switches
    uint8_t standby;  //!< Bitmask for DIO state while chip is in standby mode
    uint8_t rx;       //!< Bitmask for DIO state while chip is in reception mode
    uint8_t tx;       //!< Bitmask for DIO state while chip is in transmission mode
    uint8_t tx_hp;    //!< Bitmask for DIO state while chip is in high power transmission mode
    uint8_t tx_hf;    //!< Bitmask for DIO state while chip is in high frequency transmission mode
} lr1121_modem_system_rfswitch_cfg_t;

/**
 * @brief Stand by configuration values
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_STANDBY_CFG_RC   = 0x00,
    LR1121_MODEM_SYSTEM_STANDBY_CFG_XOSC = 0x01
} lr1121_modem_system_standby_cfg_t;

/**
 * @brief TCXO supply voltage values
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_TCXO_CTRL_1_6V = 0x00,  //!< Supply voltage = 1.6v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_1_7V = 0x01,  //!< Supply voltage = 1.7v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_1_8V = 0x02,  //!< Supply voltage = 1.8v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_2_2V = 0x03,  //!< Supply voltage = 2.2v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_2_4V = 0x04,  //!< Supply voltage = 2.4v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_2_7V = 0x05,  //!< Supply voltage = 2.7v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_3_0V = 0x06,  //!< Supply voltage = 3.0v
    LR1121_MODEM_SYSTEM_TCXO_CTRL_3_3V = 0x07,  //!< Supply voltage = 3.3v
} lr1121_modem_system_tcxo_supply_voltage_t;

/**
 * @brief Status register 1 structure definition
 */
typedef struct lr1121_modem_system_stat1_s
{
    lr1121_modem_system_command_status_t command_status;       //!< Status of last command
    bool                                 is_interrupt_active;  //!< Indicates at least one interrupt is active
} lr1121_modem_system_stat1_t;

/**
 * @brief Status register 2 structure definition
 */
typedef struct lr1121_modem_system_stat2_s
{
    lr1121_modem_system_reset_status_t reset_status;  //!< Source of reset
    lr1121_modem_system_chip_modes_t   chip_mode;     //!< Current mode the chip is running
    bool is_running_from_flash;                       //!< Flag indicating if the chip is currently running from flash
} lr1121_modem_system_stat2_t;

/**
 * @brief Chip type values
 */
typedef enum
{
    LR1121_MODEM_SYSTEM_VERSION_TYPE_LR1121 = 0x03,
} lr1121_modem_system_version_type_t;

/**
 * @brief Version structure definition
 */
typedef struct lr1121_modem_system_version_s
{
    uint8_t                            hw;    //!< Hardware field of system version
    lr1121_modem_system_version_type_t type;  //!< Type field of system version
    uint16_t                           fw;    //!< Software field of system version
} lr1121_modem_system_version_t;

/**
 * @brief Sleep configuration structure definition
 */
typedef struct lr1121_modem_system_sleep_cfg_s
{
    bool is_warm_start;  //!< Keep configuration and state in retention memory, allowing warm start
} lr1121_modem_system_sleep_cfg_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // LR1121_MODEM_SYSTEM_TYPES_H

/* --- EOF ------------------------------------------------------------------ */
