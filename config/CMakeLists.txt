# Compat shim: force-include a header that redefines INPUT_CALLBACK_DEFINE
# to accept the 3-arg form used by tokyo2006/cirque-input-module.
# Harmless on both sides; only affects the central side where the code path
# is actually compiled.
zephyr_compile_options(-include ${CMAKE_CURRENT_SOURCE_DIR}/compat_module/compat_input.h)
