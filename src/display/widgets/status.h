/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 * Adapted for zmk-config-aurora-corne from zmkfirmware/zmk
 * app/boards/shields/nice_view@cb615aee9a7c427e251f2ab62080c1a44a165da9
 * (adds split peripheral battery display).
 *
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

struct zmk_widget_status {
    sys_snode_t node;
    lv_obj_t *obj;
    uint8_t cbuf[CANVAS_BUF_SIZE];
    uint8_t cbuf2[CANVAS_BUF_SIZE];
    uint8_t cbuf3[CANVAS_BUF_SIZE];
    struct status_state state;
};

int zmk_widget_status_init(struct zmk_widget_status *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_status_obj(struct zmk_widget_status *widget);
