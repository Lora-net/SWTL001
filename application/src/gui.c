/*!
 * @file      gui.c
 *
 * @brief     LR1110 firmware update implementation
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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

#include "semtech_logo.h"
#include "stdio.h"
#include "version.h"
#include "gui.h"

static lv_obj_t* screen;
static lv_obj_t* icon;
static lv_obj_t* lbl_title;
static lv_obj_t* lbl_version;
static lv_obj_t* preload;
static lv_obj_t* lbl_status;

static lv_style_t screen_style;
static lv_style_t title_style;
static lv_style_t preloader_style;

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

void gui_init( lr11xx_fw_update_t update, uint32_t fw_expected )
{
    char buffer[30] = { 0 };

    lv_style_copy( &( screen_style ), &lv_style_scr );
    screen_style.body.main_color = LV_COLOR_BLACK;
    screen_style.body.grad_color = LV_COLOR_BLACK;
    screen_style.text.color      = LV_COLOR_WHITE;

    lv_style_copy( &( title_style ), &lv_style_scr );
    title_style.body.main_color = LV_COLOR_BLACK;
    title_style.body.grad_color = LV_COLOR_BLACK;
    title_style.text.color      = LV_COLOR_WHITE;
    title_style.text.font       = &lv_font_roboto_22;

    screen = lv_obj_create( NULL, NULL );
    lv_obj_set_style( screen, &( screen_style ) );

    icon = lv_img_create( screen, NULL );
    lv_obj_align( icon, NULL, LV_ALIGN_IN_TOP_LEFT, 17, 0 );
    lv_img_set_src( icon, &semtech_logo );

    sprintf( buffer, "LR11XX\nUPDATER TOOL\n%s", DEMO_VERSION );

    lv_obj_t* lbl_title = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_title, &( title_style ) );
    lv_label_set_long_mode( lbl_title, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_title, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_title, buffer );
    lv_obj_set_width( lbl_title, 240 );
    lv_obj_align( lbl_title, NULL, LV_ALIGN_CENTER, 0, -30 );

    switch( update )
    {
    case LR1110_FIRMWARE_UPDATE_TO_TRX:
    {
        sprintf( buffer, "Update LR1110 to FW 0x%04x", fw_expected );
        break;
    }
    case LR1110_FIRMWARE_UPDATE_TO_MODEM_V1:
    {
        sprintf( buffer, "Update Modem-E to FW 0x%06x", fw_expected );
        break;
    }
    case LR1120_FIRMWARE_UPDATE_TO_TRX:
    {
        sprintf( buffer, "Update LR1120 to FW 0x%04x", fw_expected );
        break;
    }
    case LR1121_FIRMWARE_UPDATE_TO_TRX:
    {
        sprintf( buffer, "Update LR1121 to FW 0x%04x", fw_expected );
        break;
    }
    case LR1121_FIRMWARE_UPDATE_TO_MODEM_V2:
    {
        sprintf( buffer, "Update Modem-E to FW 0x%06x", fw_expected );
        break;
    }
    default:
    break;
    }

    lv_obj_t* lbl_fw = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_fw, &( screen_style ) );
    lv_label_set_long_mode( lbl_fw, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_fw, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_fw, buffer );
    lv_obj_set_width( lbl_fw, 240 );
    lv_obj_align( lbl_fw, NULL, LV_ALIGN_CENTER, 0, 30 );

    lbl_status = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_status, &( screen_style ) );
    lv_label_set_long_mode( lbl_status, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_status, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_status, "UPDATE ON GOING..." );
    lv_obj_set_width( lbl_status, 240 );
    lv_obj_align( lbl_status, NULL, LV_ALIGN_CENTER, 0, 80 );

    lv_scr_load( screen );
}

void gui_update( const char* txt )
{
    lv_label_set_text( lbl_status, txt );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
