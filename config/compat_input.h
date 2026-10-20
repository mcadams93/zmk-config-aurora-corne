/*
 * Compat shim for INPUT_CALLBACK_DEFINE.
 *
 * Force-included via -include before any source file. Includes the real
 * Zephyr input.h (setting its include guard), then redefines the macro
 * to accept an optional 3rd user_data argument. When the source file
 * later does #include <zephyr/input/input.h>, the guard prevents
 * re-inclusion and the variadic version survives.
 *
 * Needed because tokyo2006/cirque-input-module uses the 3-arg form
 * from newer Zephyr, but ZMK v0.3 Zephyr only has the 2-arg form.
 */
#include <zephyr/input/input.h>

#undef INPUT_CALLBACK_DEFINE
#define INPUT_CALLBACK_DEFINE(_dev, _callback, ...)                            \
    static const STRUCT_SECTION_ITERABLE(input_listener,                       \
                 _input_listener__##_callback) = {                             \
        .dev = _dev,                                                           \
        .callback = (void (*)(struct input_event *))(_callback),               \
    }
