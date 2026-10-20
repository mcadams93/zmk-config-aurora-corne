/*
 * Compat shim: ZMK v0.3 Zephyr provides a 2-arg INPUT_CALLBACK_DEFINE,
 * but the tokyo2006 cirque module uses the 3-arg form (with user_data).
 * This wrapper accepts both forms.
 */
#include_next <zephyr/input/input.h>

#undef INPUT_CALLBACK_DEFINE
#define INPUT_CALLBACK_DEFINE(_dev, _callback, ...)                            \
    static const STRUCT_SECTION_ITERABLE(input_listener,                       \
                 _input_listener__##_callback) = {                             \
        .dev = _dev,                                                           \
        .callback = (void (*)(struct input_event *))(_callback),               \
    }
