/*
 * SPDX-License-Identifier: MIT
 *
 * Turns INPUT_BTN_TOUCH into a single short click of mouse button param1.
 * The BTN_TOUCH event is neutralised (code -> INPUT_KEY_RESERVED) so the
 * listener never holds a button, but processing CONTINUEs so sync still
 * flushes movement.
 *
 * Concurrency: press runs in the input thread, release in the system
 * workqueue; both touch the shared HID mouse report.
 */

#define DT_DRV_COMPAT zmk_input_processor_tap_click

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/input_processor.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct tc_config {
    uint32_t click_ms;
};

struct tc_data {
    bool last;
    bool pressed;
    zmk_mouse_button_t button;
    struct k_work_delayable release_work;
};

static void tc_release(struct k_work *work) {
    struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct tc_data *data = CONTAINER_OF(dwork, struct tc_data, release_work);

    if (!data->pressed) {
        return;
    }
    data->pressed = false;
    zmk_hid_mouse_button_release(data->button);
    zmk_endpoint_send_mouse_report();
}

static int tc_handle_event(const struct device *dev, struct input_event *event, uint32_t param1,
                           uint32_t param2, struct zmk_input_processor_state *state) {
    const struct tc_config *cfg = dev->config;
    struct tc_data *data = dev->data;

    if (event->type != INPUT_EV_KEY || event->code != INPUT_BTN_TOUCH) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    bool now = event->value > 0;

    if (now && !data->last) {
        if (data->pressed && data->button != (zmk_mouse_button_t)param1) {
            zmk_hid_mouse_button_release(data->button);
        }
        data->button = (zmk_mouse_button_t)param1;
        data->pressed = true;
        zmk_hid_mouse_button_press(data->button);
        zmk_endpoint_send_mouse_report();
        k_work_reschedule(&data->release_work, K_MSEC(cfg->click_ms));
        LOG_DBG("tap-click button %d", data->button);
    }

    data->last = now;
    event->code = INPUT_KEY_RESERVED;

    return ZMK_INPUT_PROC_CONTINUE;
}

static int tc_init(const struct device *dev) {
    struct tc_data *data = dev->data;
    k_work_init_delayable(&data->release_work, tc_release);
    return 0;
}

static struct zmk_input_processor_driver_api tc_driver_api = {
    .handle_event = tc_handle_event,
};

#define TC_INST(n)                                                                                 \
    static struct tc_data tc_data_##n = {};                                                        \
    static const struct tc_config tc_config_##n = {                                                \
        .click_ms = DT_INST_PROP(n, click_ms),                                                     \
    };                                                                                             \
    DEVICE_DT_INST_DEFINE(n, tc_init, NULL, &tc_data_##n, &tc_config_##n, POST_KERNEL,             \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &tc_driver_api);

DT_INST_FOREACH_STATUS_OKAY(TC_INST)
