/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 * Adapted for zmk-config-aurora-corne from zmkfirmware/zmk
 * app/boards/shields/nice_view@cb615aee9a7c427e251f2ab62080c1a44a165da9
 * (adds split peripheral battery display).
 *
 */

#include "widgets/status.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_ZMK_CORNE_STATUS_SCREEN)
static struct zmk_widget_status status_widget;
#endif

lv_obj_t *zmk_display_status_screen() {

    lv_obj_t *screen;
    screen = lv_obj_create(NULL);

#if IS_ENABLED(CONFIG_ZMK_CORNE_STATUS_SCREEN)
    zmk_widget_status_init(&status_widget, screen);
    lv_obj_align(zmk_widget_status_obj(&status_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

    return screen;
}
